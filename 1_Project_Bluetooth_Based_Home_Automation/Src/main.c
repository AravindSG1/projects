/*Bluetooth based Home automation demonstration by turning on a lamp and fan
 * using a Smartphone*/

#include "stm32f4xx.h"

#include "uart.h"
#include<string.h>

void func_for_string(char a[]);

#define GPIOCEN     (1U<<2)

#define PIN_PC0     (1U<<0)
#define LAMP        PIN_PC0

#define PIN_PC1     (1U<<1)
#define FAN         PIN_PC1

int main(void)
{
	/*configuring GPIOC pins PC0 and PC1 as output*/
	/*Enabling clock for GPIOC*/
	RCC->AHB1ENR |= GPIOCEN;

	/*setting mode as output*/
	/*for pin PC0*/
	GPIOC->MODER |=  (1U<<0);
	GPIOC->MODER &= ~(1U<<1);

	/*for pin PC1*/
	GPIOC->MODER |=  (1U<<2);
	GPIOC->MODER &= ~(1U<<3);

	uart_rx_tx_init();
	//uart_read();
	//uart_write('s');
	while(1)
	{
		/* temp variable is needed otherwise when other than 1 is pressed and
		 * program goes to next if else condition there won't be a value for comparison
		 * its already gone from the data register*/
		char temp = uart_read();

		if(temp == '1')
		{
			GPIOC->ODR |= LAMP;
			func_for_string("LAMP ON\r\n");
		}
		else if(temp == '2')
		{
			GPIOC->ODR &= ~LAMP;
			func_for_string("LAMP OFF\r\n");
		}
		else if(temp == '3')
		{
			GPIOC->ODR |=  FAN;
			func_for_string("FAN ON\r\n");
		}
		else if(temp == '4')
		{
			GPIOC->ODR &= ~FAN;
			func_for_string("FAN OFF\r\n");
		}




	}
}

void func_for_string(char a[])
{
	for(int i = 0; i<strlen(a);i++)
	{
		uart_write(a[i]);

	}
}
