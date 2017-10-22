#ifndef GUITAR_H
#define GUITAR_H
#include <map>
#include "bass/bass.h"
//structure to hold if buttons are pressed
int open_joystick(char *device_name);
int read_event(int joystickfd, struct js_event *jse);
std::string findMusic(JS_DATA_TYPE &js); //finds the sound file name based of button number combination

struct buttons{ 
	//HOLDS THE VALUE FOR js.Buttons for the buttons on guitar hero controller
	int green = 1;
	int red = 2;
	int yellow=8;
	int blue = 4; 
	int orange = 16;
	int flip;
	std::map <int, std::string> bTS = 
	{ std::make_pair(1, "wavFiles/A.wav"),//green
		std::make_pair(2, "wavFiles/High_D.wav"),//red
		std::make_pair(8, "wavFiles/F#_LOW.wav"),//yellow
		std::make_pair(4, "wavFiles/High_E.wav"),//blue
		std::make_pair(16, "wavFiles/F#.wav")//orange
	};

} jButton;


void playWav(std::string filename) {
//	printf("does this evne work \n");
	int device = -1;
	int freq = 44100;
	const void* file = filename.c_str();
	HSAMPLE streamHandle;
	HCHANNEL channel;
	/* Initialize output device */

	BASS_Init(device, freq, BASS_DEVICE_DEFAULT, 0, NULL);
	streamHandle = BASS_SampleLoad(false, file, 0, 0, 1, BASS_SAMPLE_MONO | BASS_SAMPLE_OVER_POS | BASS_SAMPLE_OVER_VOL);
	channel = BASS_SampleGetChannel(streamHandle, FALSE);

//	printf("does this evne work \n");
	if (!filename.compare("stop.wav"))
	{
		BASS_Free();
		BASS_Init(device, freq, BASS_DEVICE_DEFAULT, 0, NULL);
		streamHandle = BASS_SampleLoad(false, file, 0, 0, 1, BASS_SAMPLE_MONO | BASS_SAMPLE_OVER_POS | BASS_SAMPLE_OVER_VOL);
		channel = BASS_SampleGetChannel(streamHandle, FALSE);
		BASS_ChannelPlay(channel, FALSE);
		usleep(1000);
		BASS_Free();
		return;
	}
	/* Once you are done with your sample you should free it, but we don't wnat to until the play sample is over, or else nothing happens */
	BASS_ChannelPlay(channel, FALSE);
	//place all files in same directory or else shit goes down just put all files in resouces even though you're not using anything
	//anything//http://www.cplusplus.com/forum/windows/26944/
	//26944/* Load sampleza and play it */
	//}
	//
}

//if (jse.type & JS_EVENT_INIT) continue;
#endif
