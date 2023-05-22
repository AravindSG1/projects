#include "stm32f4xx.h"

void delay_for_msec(int unit);
void delay_for_usec(int unit);

#define GPIOAEN     (1U<<0)

#define PINA12EN    (1U<<12)
#define DHT11EN      PINA12EN

#define PINA12       12
#define DHT11        PINA12

void DHT11_config();
void DHT11_init();
unsigned char receive_bytes();
void gpio_output_mode(void);
void gpio_input_mode(void);
void delay_for_msec(int unit);
void delay_for_usec(int unit);

unsigned char dht11_data[8];    //new change

void DHT11_config()
{
	/*Configuring PA12 as GPIO for receiving data this steps are just before
	 * initialization of sensor as of datasheet*/
	/*Enabling clock to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Configuring mode to output*/
	gpio_output_mode();

	/*setting output high*/
	GPIOA->ODR |= DHT11EN;

	/*giving a delay before initialization starts*/
	delay_for_msec(100);
}

void DHT11_init()
{
	unsigned char a = 0;
	gpio_output_mode();
	GPIOA->ODR |=  DHT11EN;   //setting high
	GPIOA->ODR &= ~DHT11EN;  //clearing to low
	delay_for_msec(27);      //minimum 18ms so giving 25ms
	//value given based on logic analyzer

	GPIOA->ODR |= DHT11EN;   //setting high
	gpio_input_mode();      //changing to input

	/*Checking whether high, if yes stays there, because sensor doesn't respond
	 * with a low*/
	while(((GPIOA->IDR>>DHT11) & 1)==1);
	/*now the low came means sensor responding (54us low)*/
	while(((GPIOA->IDR>>DHT11) & 1)==0);
	/*now the high came means sensor responding (80us high)*/
	while(((GPIOA->IDR>>DHT11) & 1)==1);
	/*from now on sensor will be sending data with MSB first*/

	/*Receiving 5 bytes*/
	for(a=0;a<=4;a++)
	{

		dht11_data[a] = receive_bytes();
	}


}

unsigned char receive_bytes(void)
{
	unsigned char a = 0;
	unsigned char out;
	for(a=0; a<8; a++)
	{
		/*from here on the data started coming*/
		/*waiting for the 54us low part of sensor clear out*/
		while(((GPIOA->IDR>>DHT11) & 1)==0);

		/*Giving a delay of 35us for the high part which may be 24us or 70us*/
		delay_for_usec(27); //value given based on logic analyzer

		/*Checking if still its high after 35us then it will be given a value of 1
		 * and assign it to out variable*/
		if(((GPIOA->IDR>>DHT11) & 1)==1)
		{
			out |= (1U<<0);
			out = out<<1;
		}
		else
		{
			out &= ~(1U<<0);
			out = out<<1;
		}
		/*Waiting to be cleared out any remaining high part*/
		while(((GPIOA->IDR>>DHT11) & 1)==0);

	}
	return out;
}

void gpio_output_mode(void)
{
	/*Configuring mode to output*/
	GPIOA->MODER |=  (1U<<24);
	GPIOA->MODER &= ~(1U<<25);
}

void gpio_input_mode(void)
{
	/*Configuring mode to output*/
	GPIOA->MODER &= ~(1U<<24);
	GPIOA->MODER &= ~(1U<<25);
}

void delay_for_msec(int unit)
{
	for(int i=0; i<unit; i++)
	{
		for(int j=0; j<1334; j++);
	}
}

void delay_for_usec(int unit)
{
	int change = unit/3;
	for(int i=0; i<change; i++)
	{
		for(int j=0; j<4; j++);
	}
}
