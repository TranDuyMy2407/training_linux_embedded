#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>

int main()
{
	int level,pin_led;
	printf("enter level : ");
	scanf("%d",&level);

	printf("enter pin number: ");
	scanf("%d",&pin_led);

	int fd = open("/dev/my_device", O_RDWR)	;
	if(fd < 0)
	{
		printf("cant open the file !!! \n");
		exit(-1);
	}

	ioctl(fd,pin_led,level);
	close(fd);
	return 0;
}


