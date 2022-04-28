#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>

#define DEV_MEM_SIZE 512

struct cdev *my_cdev;
struct class *my_class;
struct device *my_device;
dev_t dev_num;
char buffer[DEV_MEM_SIZE];



static int my_open(struct inode *inode, struct file *filp)
{
	pr_info("the file is open !!! \n");
	return 0;
}


static int my_release(struct inode *inode ,struct file *filp)
{
	pr_info("the file is closed !!! \n");
	return 0;
}


static ssize_t my_read(struct file *filp , char __user *buf , size_t count, loff_t *f_pos)
{
	if(count > (DEV_MEM_SIZE - *f_pos))
		count = DEV_MEM_SIZE - *f_pos;

	if(copy_to_user(buf,buffer,count))
        	        return  -EFAULT;

	*f_pos += count;	

	return count;

}

static ssize_t my_write(struct file *filp ,const  char *buf, size_t count, loff_t *f_pos)
{
	if((DEV_MEM_SIZE - *f_pos) < count)
		count = DEV_MEM_SIZE - *f_pos;

	if(copy_from_user(buffer,buf,count))
		return  -EFAULT;

	*f_pos += count;

	return count;
}




static struct file_operations fops =
{
        .open = my_open,
        .release = my_release,
        .read = my_read,
        .write = my_write,
	.owner = THIS_MODULE
};



static int __init char_driver_init(void)
{

	int ret = alloc_chrdev_region(&dev_num,0,1,"my_device");
	if(ret < 0)
	{
		pr_err("allocate chrdev failed !! \n");
		goto out;
	}
	
	my_cdev = cdev_alloc();
	my_cdev->ops = &fops;

	ret = cdev_add(my_cdev,dev_num,1);
	if(ret < 0)
	{
		pr_err("cdev add failed !! \n");
		goto unregister_dev_num;
	}

	my_class = class_create(THIS_MODULE,"my_class_device");
	if(IS_ERR(my_class))
	{
		pr_err("class create failed !! \n");
		ret = PTR_ERR(my_class);
		goto cdev_unregister;
	}

	my_device = device_create(my_class,NULL,dev_num,NULL,"my_device");
	if(IS_ERR(my_device))
	{
		pr_err("create device file failed !!! \n");
		ret = PTR_ERR(my_device);
		goto class_del;
	}

	return 0;

class_del:
	class_destroy(my_class);

cdev_unregister:
	cdev_del(my_cdev);

unregister_dev_num:
	unregister_chrdev_region(dev_num,1);

out:
	pr_err("module create failed !! \n");
	return ret;
}


static void __exit char_driver_exit(void)
{
	device_destroy(my_class,dev_num);
	class_destroy(my_class);
	cdev_del(my_cdev);
	unregister_chrdev_region(dev_num,1);
	pr_info("the module has  removed !!! \n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TRAN DUY MY");
