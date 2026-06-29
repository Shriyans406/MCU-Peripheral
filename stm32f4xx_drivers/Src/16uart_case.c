/*
 * 16uart_case.c
 *
 *  Created on: Jun 29, 2026
 *      Author: HP
 */


#include<stdio.h>
#include<string.h>
#include "stm32f407xx.h"


//we have 3 different messages that we transmit to arduino
//you can by all means add more messages
char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};

//reply from arduino will be stored here
char rx_buf[1024] ;

USART_Handle_t usart2_handle;


//This flag indicates reception completion
uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

extern void initialise_monitor_handles();



int main(void)
{
}

