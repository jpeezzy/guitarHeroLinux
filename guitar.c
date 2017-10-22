#include <stdio.h>
#include <fcntl.h> //O_RDWR
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>
int open_joystick(char *device_name);
int read_event(int joystickfd, struct js_event *jse);
int main()
{
	//	int fd = open ("/dev/input/fs0",O_RDONLY); //device opened in blocking mode
	int fd; int result;
	struct js_event jse;
	//int result = open_joystick("/dev/input/by-id/usb-RedOctane_Inc_c_2006_Guitar_Hero_X-plorer_0651051-event-joystick");
	//int result = open_joystick("/dev/input/by-path/pci-0000:00:14.0-usb-0:2:1.0-event-joystick");
	fd = open_joystick("/dev/input/js0");

	printf("result was %d\n", result);

	while (1) {
		result = read_event(fd, &jse);
		usleep(1000);
	//	if (result == 1)
		//	printf("Event: time %8u, value %8hd, type: %3u, axis/button: %u\n", jse.time, jse.value, jse.type, jse.number);
		//printf("%i\n", jse.type);
		switch(jse.number)
		{
			case 1:
				{
					printf("gets to number1 \n");
					break;
				}
			case 2:
				{
					printf("gets to number2 \n");
					break;
				}
			case 3:
				{
					printf("gets to number3 \n");
					break;
				}
			case 4:
				{
					//printf("gets to number4 \n");
					break;
				}
			case 5:
				{
					printf("gets to number5 \n");
					break;
				}
			case 6:
				{
					printf("gets to number6 \n");
					break;
				}
		}
		
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

	if (bytes == -1) return 0;
	if (bytes == sizeof(*jse)) return 1;

	printf("Unexpected bytes from joystick:%d\n", bytes);

	return -1;
}
