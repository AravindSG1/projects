

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <string.h>

void uart_rx_tx_init(void);
void uart_write(unsigned char ch);
char uart_read(void);
void uart_send_string(unsigned char a[]);

#endif /* UART_H_ */
