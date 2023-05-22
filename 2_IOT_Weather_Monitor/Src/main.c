
#include "dht11.h"
#include "uart.h"


#include <stdint.h>

static unsigned char command_basic_1[5]="AT\r\n"; // BASIC
static unsigned char command_basic_2[9]="AT+RST\r\n";//BASIC
static unsigned char command_ESP_1[14]="AT+CWMODE=1\r\n";//WIFI
static unsigned char command_ESP_2[14]="AT+CWQAP\r\n";//WIFI
static unsigned char command_ESP_4[25]="AT+CWDHCP=1,1\r\n";//THINGS SPEAK
//This command is to connect to an Access Point (like a router).
static unsigned char command_ESP_3[50]="AT+CWJAP=\"IN1b\",\"password\"\r\n";
static unsigned char command_ESP_5[50]="AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n";
static unsigned char command_ESP_6[20]="AT+CIPSEND=64\r\n";
static unsigned char command_ESP_7[64]="GET /update?api_key=PWGIM4ISOJ6AZN7F&field1=";
static unsigned char command_ESP_8[9]="&field2=";
static unsigned char command_ESP_9[9]="\r\n\r\n";
static unsigned char command_ESP_10[20]="AT+CIPCLOSE\r\n";

static unsigned char data_string_1[8],data_string_2[8],display[30]="HUMIDITY & TEMPERATURE  --  ";

extern unsigned char dht11_data[8];  //new change

void integer_to_ascii(unsigned char Val, unsigned char *str);
void Basic_Commands(void);
void Wifi_Conn_Commands(void);
void Thing_Speak_Command_Start(void);
void Init_ESP8266(void);
void Wifi_acc(void);
void Data_Commands(void);

int main(void)
{

	uart_rx_tx_init();
	DHT11_config();
		Init_ESP8266();
		Wifi_acc();
	    Thing_Speak_Command_Start();
		delay_for_msec(500);
		while(1)
		{
			DHT11_init();
			dht11_data[0] = dht11_data[0]/1.60;
			integer_to_ascii(dht11_data[0],data_string_1);
			dht11_data[2] = dht11_data[2]/2.05;
			integer_to_ascii(dht11_data[2],data_string_2);
			Data_Commands();
			uart_send_string(display);
			uart_send_string(data_string_1);
			uart_write('\t');
			uart_write('\t');
			uart_send_string(data_string_2);
			uart_write('\r');
			uart_write('\n');
			delay_for_msec(10000);
		}

}





void integer_to_ascii(unsigned char Val, unsigned char *str)
{
  str[0] = Val/1000 + 0x30;
   Val   = Val%1000;
	str[1] = Val/100 + 0x30;
	 Val   = Val%100;
	str[2] = Val/10 + 0x30;
	str[3] = Val%10 + 0x30;
	str[4] = '\0';
}

/*AT commands*/
void Basic_Commands(void)
{
	uart_send_string(command_basic_1);
	delay_for_msec(500);
	uart_send_string(command_basic_2);
	delay_for_msec(2000);
}

void Wifi_Conn_Commands(void)
{
	uart_send_string(command_ESP_1);
	delay_for_msec(500);
	uart_send_string(command_ESP_2);
	delay_for_msec(500);

}

void Thing_Speak_Command_Start(void)
{
	uart_send_string(command_ESP_4);
	delay_for_msec(5000);

}

void Init_ESP8266(void)
{
	delay_for_msec(5000);
	Basic_Commands();
	Wifi_Conn_Commands();
}
void Wifi_acc(void)
{
	uart_send_string(command_ESP_3);
  delay_for_msec(8000);
}

void Data_Commands(void)
{
	uart_send_string(command_ESP_5);
	delay_for_msec(5000);
	uart_send_string(command_ESP_6);
    delay_for_msec(5000);
	uart_send_string(command_ESP_7);
	uart_send_string(data_string_1);
	uart_send_string(command_ESP_8);
	uart_send_string(data_string_2);
	uart_send_string(command_ESP_9);
	delay_for_msec(1000);
	uart_send_string(command_ESP_10);
	delay_for_msec(5000);
}

