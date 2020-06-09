/* Includes ------------------------------------------------------------------*/

#include "usart.h"

static volatile char rcv_char;
static volatile uint8_t flag=0;

// UART transmit buffer descriptor
static RingBuffer USART_RingBuffer_Tx;
// UART transmit buffer memory pool
static char RingBufferData_Tx[1024];

// UART receive buffer descriptor
static RingBuffer USART_RingBuffer_Rx;
// UART receive buffer memory pool
static char RingBufferData_Rx[1024];

int _write(int file, char *ptr, int len) {
	USART_WriteData(ptr,len);
	return len;
}

size_t USART_WriteData(const void *data, size_t dataSize){
	size_t i = 0;
	
	static char c;


	for(;i<dataSize;i++)
	{
		
		CORE_EnterCriticalSection();
		if(!RingBuffer_PutChar(&USART_RingBuffer_Tx, *((char*)data+i)) ) {
			CORE_ExitCriticalSection();
			return false;
		}
		CORE_ExitCriticalSection();
	}
	CORE_EnterCriticalSection();
	if(!flag){
		RingBuffer_GetChar(&USART_RingBuffer_Tx, &c );
		HAL_UART_Transmit_IT(&HUARTx, &c,1);
		flag=1;
	}
	CORE_ExitCriticalSection();
	return i;
}


size_t USART_WriteString(const char *string){
	return USART_WriteData(string, strlen(string));
}


bool USART_GetChar(char *c){
	
	CORE_EnterCriticalSection();
	if(!RingBuffer_GetChar(&USART_RingBuffer_Rx, c )) {
		CORE_ExitCriticalSection();
		return false;
	}
	CORE_ExitCriticalSection();
	
	return true;
}


size_t USART_ReadData(char *data, size_t maxSize){
	size_t i = 0;
	
	for(;i<maxSize;i++)
	{	
		CORE_EnterCriticalSection();
		if(!RingBuffer_GetChar(&USART_RingBuffer_Rx, &data[i] )) {
			CORE_ExitCriticalSection();	
			break;
		}
		CORE_ExitCriticalSection();	
	}
	return i;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if(huart->Instance==HUARTx.Instance){

		RingBuffer_PutChar(&USART_RingBuffer_Rx, rcv_char);
		HAL_UART_Receive_IT(&HUARTx,&rcv_char,1);

	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){

	if(huart->Instance==HUARTx.Instance){
		static char c;

		if(RingBuffer_GetChar(&USART_RingBuffer_Tx, &c) == true)
		{
			//c=*"a";
			HAL_UART_Transmit_IT(&HUARTx, &c,1);
			flag=1;
		}else flag=0;

	}

}





/**
  * This function initialize ring buffers, USART device and enable receive interrupt.  
  */
void USART_Init(void){
	// initialize ring buffers
	RingBuffer_Init(&USART_RingBuffer_Tx, RingBufferData_Tx, sizeof(RingBufferData_Tx));
	RingBuffer_Init(&USART_RingBuffer_Rx, RingBufferData_Rx, sizeof(RingBufferData_Rx));
	
	HAL_UART_Receive_IT(&HUARTx,&rcv_char,1);


}


