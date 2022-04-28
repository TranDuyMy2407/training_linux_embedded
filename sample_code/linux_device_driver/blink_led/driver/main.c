#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/ioctl.h>
#include<linux/io.h>
#include<linux/uaccess.h>
#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/jiffies.h>
#include<linux/gpio.h>
#include "gpio_define.h"

#define DEV_MEM_SIZE 512


struct cdev *my_cdev;
struct class *my_class;
dev_t dev;


char kernel_buffer[1000];
unsigned int *gpio_base;
unsigned int pin_led = 4;


static int my_open(struct inode *inode , struct file *filp)
{
	pr_info("the file is opened !!! \n");
	return 0;
}

static int my_release(struct inode *inode , struct file *filp)
{
	pr_info("the file i closed !!! \n");
	return 0;
}

static ssize_t my_read(struct file *filp , char __user *buf, size_t len, loff_t *f_pos)
{
	if( (DEV_MEM_SIZE - *f_pos) < len )
		len = DEV_MEM_SIZE - *f_pos;

	if(copy_to_user(buf,kernel_buffer,len))
		return -EFAULT;

	*f_pos += len;


	return len;
}

static ssize_t my_write(struct file *filp, const char *buf, size_t len, loff_t *f_pos)
{
	memset(kernel_buffer,0,sizeof(kernel_buffer));
	pin_mode(gpio_base,pin_led,1);


	if(buf[0] > 48)
		pin_set(gpio_base,pin_led,HIGH);


	if(buf[0] == 48)
		pin_set(gpio_base,pin_led,LOW);


	if((DEV_MEM_SIZE - *f_pos) < len)
		len = DEV_MEM_SIZE - *f_pos;

	if(copy_from_user(kernel_buffer,buf,len))
		return -EFAULT;
	*f_pos += len;

	return len;
}

static long my_ioctl(struct file *filp, unsigned int cmd , unsigned long arg )
{
	int count = 1;
	int temp = arg;
	int  i;
	int ret;

	pin_led = cmd;
	ret = pin_mode(gpio_base,pin_led,1);
	pr_info("pin_led: %d \n",pin_led);
	pr_info("mode of pin %d : %d \n",pin_led,ret);

	if(arg > 0)
		pin_set(gpio_base,pin_led,HIGH);

	if(arg == 0)
		pin_set(gpio_base,pin_led,LOW);

	memset(kernel_buffer,0,sizeof(kernel_buffer));	
	
	while(temp / 10)
	{
		count++;
		temp/=10;
	}

	pr_info("count: %d\n",count);
	temp = arg;
	for(i = count-1;i>=0;i--)
	{
		kernel_buffer[i] = temp%10+48;
		temp/=10;
		pr_info("temp: %d \n",temp);
	}

	kernel_buffer[count] = '\n';
	return 0;
}



struct file_operations my_ops =
{
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl = my_ioctl
};

static int __init char_driver_init(void)
{
	if(alloc_chrdev_region(&dev,0,1,"my_device") < 0)
	{
		pr_info("cant allocate the major number !!! \n");
		return -EAGAIN;
	}
	
	my_cdev = cdev_alloc();
	my_cdev->owner = THIS_MODULE;
	my_cdev->ops = &my_ops;

	if(cdev_add(my_cdev,dev,1)<0)
	{
		pr_info("cant add the device into system !!! \n");
		goto r_class;
	}

	if((my_class = class_create(THIS_MODULE,"my_class"))== NULL)
	{
		pr_info("cant create the class !! \n");
		goto r_class;
	}

	if(device_create(my_class,NULL,dev,NULL,"my_device") == NULL)
	{
		pr_info("cant create the device file !!! \n");
		goto r_device;
	}

	gpio_base = ioremap(ADDR_BASE,100);
	
	return 0;

r_device:
	class_destroy(my_class);

r_class:
	unregister_chrdev_region(dev,1);

	return -1;

}

static void __exit char_driver_exit(void)
{
	pin_set(gpio_base,pin_led,LOW);
	cdev_del(my_cdev);
	device_destroy(my_class,dev);
	class_destroy(my_class);
	unregister_chrdev_region(dev,1);
	iounmap(gpio_base);

	pr_info("the driver has removed !!! \n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
MODULE_LICENSE("GPL");
