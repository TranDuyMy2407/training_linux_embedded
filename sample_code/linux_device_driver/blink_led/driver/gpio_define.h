//base address
#define ADDR_BASE 0x3F200000

//pin value
#define LOW 	0
#define HIGH	1


//register
#define GPFSEL0 0
#define GPFSEL1 1
#define GPFSEL2 2
#define GPFSEL3 3
#define GPFSEL4 4
#define GPFSEL5 5

#define GPSET0	7
#define GPSET1	8

#define GPCLR0	10
#define GPCLR1	11

#define GPLEV0	13
#define GPLEV1	14

#define GPEDS0	16
#define GPEDS1	17

#define GPREN0 	19
#define GPREN1	20

#define GPFEN0	22
#define GPFEN1	23


//prototype 

int pin_mode(unsigned int *gpio_base, unsigned int pin , unsigned int mode );
int pin_set(unsigned int *gpio_base,unsigned int pin, unsigned int value);
unsigned int pin_level(unsigned int *gpio_base, unsigned int pin);
