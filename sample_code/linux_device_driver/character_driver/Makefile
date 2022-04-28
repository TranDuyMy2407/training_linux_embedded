obj-m := char_driver.o
KDIR = /lib/modules/$(shell uname -r)/build 

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
help:
	make -C $(KDIR) M=$(PWD) help

insert:
	sudo insmod char_driver.ko
	sudo chmod 777 /dev/my_device

remove:
	sudo rmmod char_driver
