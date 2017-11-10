//This program uses the OSS library.
//http://digitalsoundandmusic.com/2-3-12-modeling-sound-in-c-under-linux/
#include <sys/ioctl.h> //for ioctl()
#include <math.h> //sin(), floor(), and pow()
#include <stdio.h> //perror
#include <fcntl.h> //open, O_WRONLY
#include <linux/soundcard.h> //SOUND_PCM*
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <mutex>
using namespace std;

#define TYPE char
#define LENGTH 1 //number of seconds per frequency
#define RATE 44100 //sampling rate
#define SIZE sizeof(TYPE) //size of sample, in bytes
#define CHANNELS 1 //number of audio channels
#define PI 3.14159
#define NUM_FREQS 3 //total number of frequencies
//#define BUFFSIZE (int) (NUM_FREQS*LENGTH*RATE*SIZE*CHANNELS) //bytes sent to audio device
#define BUFFSIZE 10000
#define ARRAYSIZE (int) (NUM_FREQS*LENGTH*RATE*CHANNELS) //total number of samples
#define SAMPLE_MAX (pow(2,SIZE*8 - 1) - 1) 
std::mutex keyLock;
void writeToSoundDevice(TYPE buf[], int deviceID, int status) {
	//	status = write(deviceID, buf, BUFFSIZE);
	//	if (status != BUFFSIZE)
	//		perror("Wrote wrong number of bytes\n");
	status = ioctl(deviceID, SNDCTL_DSP_SYNC, 0);
	if (status == -1)
		perror("SNDCTL_DSP_SYNC failed\n");
}

void initializeThread(int deviceID)
{
	TYPE soundBuf[BUFFSIZE];
	std::ofstream outfile;
	outfile.open("values.txt", std::ios_base::app);

	for (int t = 0; t < BUFFSIZE; ++t) 
	{
		soundBuf[t] = 2*sin(880*PI*t/RATE);
		//std::cout << (int)soundBuf[t] << std::endl;
		outfile << sin(880*PI*t/RATE) <<"\n";
	}

	int status;
	status = write(deviceID, soundBuf, BUFFSIZE);
	printf("thread print line \n");
	while(true)
	{
//		frequency = 440;
		for (int t = 0; t < BUFFSIZE; ++t) 
		{
			//soundBuf[t] = 2*sin(frequency*2*PI*t/RATE);
			soundBuf[t] = 2*sin(880*PI*t/RATE) +  2*sin(1100*PI*t/RATE) + 2*sin(1320*PI*t/RATE);;;
			//std::cout << (int)soundBuf[t] << std::endl;
			outfile << sin(880*PI*t/RATE) <<"\n";
		}

		int status;
		status = write(deviceID, soundBuf, BUFFSIZE);
		keyLock.lock();
		writeToSoundDevice(soundBuf, deviceID, status);
		keyLock.unlock();
	}
}

int main() {
	//for (i = 0; i < NUM_FREQS; ++i) {
	//		switch (i)
	//
	// conceptual idea:: 2 threads, one buffering while the other is playing. 
	//
	int deviceID, arg, status, f, t, a, i;
	TYPE buf[ARRAYSIZE];
	deviceID = open("/dev/dsp", O_WRONLY, 0);
	if (deviceID < 0)
		perror("Opening /dev/dsp failed\n");
	// working
	arg = SIZE * 8;
	status = ioctl(deviceID, SNDCTL_DSP_SETFMT, &arg);
	if (status == -1)
		perror("Unable to set sample size\n");
	arg = CHANNELS;
	status = ioctl(deviceID, SNDCTL_DSP_CHANNELS, &arg);
	if (status == -1)
		perror("Unable to set number of channels\n");
	arg = RATE;
	status = ioctl(deviceID, SNDCTL_DSP_SPEED, &arg);
	if (status == -1)
		perror("Unable to set sampling rate\n");
	a = SAMPLE_MAX;
	std::cout << "A is " << a << std::endl; 
	std::thread t1 (initializeThread, deviceID);
	std::thread t2(initializeThread, deviceID);
	t1.join();
	t2.join();
	return 0;
}



