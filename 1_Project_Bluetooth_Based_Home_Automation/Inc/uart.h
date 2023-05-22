

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uart_rx_tx_init(void);
void uart_write(char ch);
char uart_read(void);

#endif /* UART_H_ */
