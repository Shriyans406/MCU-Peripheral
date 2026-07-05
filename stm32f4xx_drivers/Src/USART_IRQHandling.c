/*********************************************************************
 * @fn      		  - USART_IRQHandler
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs

 */
void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{

	uint32_t temp1 , temp2, temp3;

/*************************Check for TC flag ********************************************/

    //Implement the code to check the state of TC bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << TODO);

	 //Implement the code to check the state of TCEIE bit
	temp2 = pUSARTHandle->pUSARTx->TODO & ( 1 << TODO);

	if(temp1 && temp2 )
	{
		//this interrupt is because of TC

		//close transmission and call application callback if TxLen is zero
		if ( pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			//Check the TxLen . If it is zero then close the data transmission
			if(! pUSARTHandle->TODO )
			{
				//Implement the code to clear the TC flag
				pUSARTHandle->pUSARTx->TODO &= ~( 1 << TODO);

				//Implement the code to clear the TCIE control bit

				//Reset the application state
				pUSARTHandle->TxBusyState = USART_READY;

				//Reset Buffer address to NULL
				TODO

				//Reset the length to zero
				TODO

				//Call the applicaton call back with event USART_EVENT_TX_CMPLT
				USART_ApplicationEventCallback(pUSARTHandle,TODO);
			}
		}
	}

	/*************************Check for TXE flag ********************************************/

		//Implement the code to check the state of TXE bit in the SR
		temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << TODO);

		//Implement the code to check the state of TXEIE bit in CR1
		temp2 = TODO


		if(temp1 && temp2 )
		{
			//this interrupt is because of TXE

			if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
			{
				//Keep sending data until Txlen reaches to zero
				if(pUSARTHandle->TxLen > 0)
				{
					//Check the USART_WordLength item for 9BIT or 8BIT in a frame
					if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
					{
						//if 9BIT , load the DR with 2bytes masking the bits other than first 9 bits
						pdata = (uint16_t*) pTxBuffer;

						//loading only first 9 bits , so we have to mask with the value 0x01FF
						pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

						//check for USART_ParityControl
						if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
						{
							//No parity is used in this transfer , so, 9bits of user data will be sent
							//Implement the code to increment pTxBuffer twice
							pTxBuffer++;
							pTxBuffer++;

							//Implement the code to decrement the length
							TODO
						}
						else
						{
							//Parity bit is used in this transfer . so , 8bits of user data will be sent
							//The 9th bit will be replaced by parity bit by the hardware
							pTxBuffer++;

							//Implement the code to decrement the length
							TODO
						}
					}
					else
					{
						//This is 8bit data transfer
						pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);

						//Implement the code to increment the buffer address
						pTxBuffer++;

						//Implement the code to decrement the length
						TODO
					}

				}
				if (TxLen == 0 )
				{
					//TxLen is zero
					//Implement the code to clear the TXEIE bit (disable interrupt for TXE flag )
					TODO
				}
			}
		}

