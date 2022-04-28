obj-m += char_driver.o

char_driver-objs += main.o gpio_define.o

KDIR = /lib/modules/$(shell uname -r)/build 

all:	
	make -C $(KDIR) M=$(PWD) modules

insert: 
	sudo insmod char_driver.ko
	sudo chmod 777 /dev/my_device

remove:
	sudo rmmod char_driver



clean:
	make -C $(KDIR) M=$(PWD) clean
