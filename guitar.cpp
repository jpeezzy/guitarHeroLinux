#include <stdio.h>
#include <fcntl.h> //O_RDWR
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include "guitar.h"

void setButton(js_event *jse);
void playWav(std::string filename); //plays sound based of filename
int main()
{
	//playWav("wavFiles/High_D.wav");
	int fd; int cpFd, FDA; int result;
	struct js_event jse;
	struct JS_DATA_TYPE js;
	struct js_corr jsc; //holds axis information
	fd= open_joystick((char *)"/dev/input/js0"); //device opened in blocking mode
	FDA = open_joystick((char *)"/dev/input/js0");//this fd is used for finding axis number
	cpFd = open_joystick((char *)"/dev/input/js0");//this fd is used for detecting if flips work 
	//printf("result was %d\n", result);
	int prevNum =0; //holds previous button number
	int flip = 0;  //checks previous flip
	bool whammy; //checks if wammy is activated
	while (1) 
	{
		read_event(cpFd, &jse);
		read(fd, &js, JS_RETURN); //recieves value about what buttons are pressed on joystick
		read(FDA, &jsc, JS_RETURN); //recieves value about what buttons are pressed on joystick
		//printf("whammy state %i 	%i 		%i \n", jsc.coef, jsc.prec, jsc.type);
		whammy = jse.type == 3;
		//printf("whammy is either on or oof %i\n", jse.type);
		if(jse.number == 7 && js.buttons > 0) //THIS IS FOR THE FLIP
		{
			if((js.buttons == prevNum && jse.number  == flip) || jse.value == 0)
			{
				continue;
			}
				playWav(jButton.bTS[js.buttons]);
				//printf("jse number : %i", jse.type);
//				printf("flip detected ! %i \n", jse.number);
				prevNum = js.buttons;
		}
		flip = jse.number;
		//usleep(10000);
		if (jse.value == 128)
		printf("wammy hit %i\n", jse.value);
		//if (jse.type & JS_EVENT_INIT) continue;
	//	if (result == 1)
	//	printf("Event: time %8u, value %8hd, type: %3u, axis/button: %u\n", jse.time, jse.value, jse.type, jse.number);
	} 

}


int open_joystick(char *device_name)
{
	int joystick_fd = -1;

	if(device_name == NULL) return joystick_fd; 

	joystick_fd = open(device_name, O_RDONLY | O_NONBLOCK);

	if (joystick_fd < 0)
	{
		printf("Could not locate joystick device %s \n" , device_name);
		return joystick_fd;
	}

	return joystick_fd;
}

int read_event(int joystick_fd, struct js_event *jse) {
	int bytes;

	bytes = read(joystick_fd, jse, sizeof(*jse));
	//jse->value = 0;
	if (bytes == -1) return 0;
	if (bytes == sizeof(*jse)) return 1;

	printf("Unexpected bytes from joystick:%d\n", bytes);

	return -1;
}

