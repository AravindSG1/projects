#include "stm32f4xx.h"

#include "uart.h"

#define GPIOAEN   (1U<<0)

#define UART1EN    (1<<4)
#define UART1RE    (1<<2)
#define UART1TE    (1<<3)
#define UART1UE     (1<<13)

#define SYSTEM_FREQ  16000000
#define APB1_CLK     SYSTEM_FREQ

#define UART1_SR_RXNE  (1<<5)
#define UART1_SR_TXE  (1<<7)


#define UART_BAUDRATE 9600

void uart_set_baudrate(USART_TypeDef* USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t BaudRate);

void uart_write(char ch);
void uart_rx_tx_init(void);

char uart_read(void);

/*Creating a uart trasmit driver on Uart2*/
void uart_rx_tx_init(void)
{
	/*Configuring the GPIOA PA9(TX) and PA10(RX)*/
	/*Enabling clock to GPIOA pins*/
	RCC->AHB1ENR |= GPIOAEN;

	/*selecting mode for GPIOA*/
	/*for pin PA9*/
	GPIOA->MODER &= ~(1U<<18);
	GPIOA->MODER |=  (1U<<19);

	/*for pin PA10*/
	GPIOA->MODER &= ~(1U<<20);
	GPIOA->MODER |=  (1U<<21);

	/*selecting the alternate function as UART*/
	/*for pin PA9*/
	GPIOA->AFR[1] |= (1U<<4);
	GPIOA->AFR[1] |= (1U<<5);
	GPIOA->AFR[1] |= (1U<<6);
	GPIOA->AFR[1] &=~(1U<<7);

	/*for pin PA10*/
	GPIOA->AFR[1] |= (1U<<8);
	GPIOA->AFR[1] |= (1U<<9);
	GPIOA->AFR[1] |= (1U<<10);
	GPIOA->AFR[1] &=~(1U<<11);
	/*********************************************/

	/*Configuring UART2 module*/
	/*Enabling clock of APB2 for uart1*/
	RCC->APB2ENR |= UART1EN;

	/*Setting baud rate*/
	uart_set_baudrate(USART1, APB1_CLK, UART_BAUDRATE);

	/*Setting transfer direction rx and tx*/
	USART1->CR1 |= UART1TE | UART1RE;

	/*Enable UART module*/
	USART1->CR1 |= UART1UE;

}

void uart_write(char ch)
{
	/*check whether transmit data register is empty*/
	while(!(USART1->SR & UART1_SR_TXE)){}

	/*writing to data register*/
	USART1->DR = (ch & 0xff);
}

char uart_read(void)
{

	/*check whether receive data register(both are same) is empty*/
	while(!(USART1->SR & UART1_SR_RXNE)){}

	/*return data in DR or read data*/
	 return USART1->DR;
}

void uart_set_baudrate(USART_TypeDef* USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_baudrate(PeriphClk, BaudRate);
}

static uint16_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
