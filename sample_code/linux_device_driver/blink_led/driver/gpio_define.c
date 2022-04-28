#include"gpio_define.h"
#include<linux/module.h>

int pin_mode(unsigned int *gpio_base , unsigned int pin , unsigned int mode)
{
	unsigned int *set_mode = gpio_base + pin/10;
	if(pin < 0 || pin > 40)
	{
		pr_info("the pin number is invalid !!! \n");
		return EINVAL;
	}

	else
	{

		*set_mode &= ~(0x7<<(pin%10*3));
		*set_mode |= mode << (pin%10*3);
		
	}
	
	return (*set_mode>>(pin%10*3)) & 1;


}

int pin_set(unsigned int *gpio_base, unsigned int pin ,unsigned int value)
{

	unsigned int *set_value;
	if(pin > 40 || pin < 0)
	{
		pr_info("the pin number is invalid !!! \n");
		return EINVAL;
	}

	else
	{
		if(value)
		{
			

			if( pin < 32)
			{
				set_value = gpio_base + GPSET0;
				pr_info("value of pin %d on GPSET before: %d \n",pin,*set_value>>pin & 1);
				*set_value |= 1<<pin;
				pr_info("value of pin %d on GPSET after: %d \n",pin,*set_value>>pin & 1);
			}

			else
			{
				set_value = gpio_base + GPSET1;
				*set_value |= 1<<pin;
			}
		}

		else if(!value)
		{
			if(pin < 32)
			{
				set_value = gpio_base + GPCLR0;
				pr_info("value of pin %d on GPCLR before: %d \n",pin,*set_value>>pin & 1);
				*set_value |= 1<<pin;
				pr_info("value of pin %d on GPCLR after: %d \n",pin,*set_value>>pin & 1);
			}
			else
			{
				set_value = gpio_base + GPCLR1;
				*set_value |= 1<<pin;
			}
		}
	}

	
	return 0;

}


unsigned int pin_level(unsigned int *gpio_base, unsigned int pin)
{
	unsigned int *level;
	if(pin < 0 || pin > 40)
	{
		pr_info("the pin number is invalid !!! \n");
		return EINVAL;
	}

	else
	{
		if(pin < 32)
			level = gpio_base + GPLEV0;

		else
			level = gpio_base + GPLEV1;
	
	
		return (*level>>pin) & 1;
	}
}
