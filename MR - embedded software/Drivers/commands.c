/*
 * commands.c
 *
 *  Created on: Jun 6, 2019
 *      Author: matt
 */
#include "commands.h"



void Bwrite(char *args){

    	int address=0;
    	int data=0;
    	sscanf(args, "%x%*c%x", &address, &data);
    	write((uint32_t)address,(uint8_t)data);

}

void Bread(char *args){

	    	int address=0;
	     	uint8_t data;
	     	char str[100];
	     	sscanf(args, "%x", &address);
	     	data = read((uint32_t)address);
	     	sprintf(str, "0x%02x\r\n", data);
	     	USART_WriteString(str);
}

void Write(char *args){

    	int address=0;
    	int len=0;
    	static uint8_t data[256];
    	sscanf(args, "%x%*c%x", &address, &len);

    	for(uint8_t i=0; i<len; i++){
    		sscanf(13+args+i*5, "%x", (uint8_t*)data+i);
    	}

    	bulk_write((uint32_t)address,(uint8_t*)data,(uint8_t)len);

}

void Read(char *args){

	    	int address=0;
	     	uint8_t* data;
	     	int len=0;
	     	char str[100];
	     	sscanf(args, "%x%*c%x", &address, &len);
	     	data = bulk_read((uint32_t)address,(uint8_t)len);

	     	uint8_t i=0;
	     	for(i=0; i<len-1; i++){
	     		sprintf(str, "0x%02x ", *(data+i));
		     	USART_WriteString(str);
	     	}
     		sprintf(str, "0x%02x", *(data+i));
	     	USART_WriteString(str);
	     	sprintf(str, "\r\n");
	     	USART_WriteString(str);
}


void init_commands(void)
{
//***********************************************************************
	static CLI_CommandItem item_write = {.callback = Write,
											.commandName = "write",
											.description = NULL};
	if(CLI_AddCommand(&item_write) == false){
		USART_WriteString("ERROR in adding new item.\n\r");
	}
//***********************************************************************
	static CLI_CommandItem item_read = {.callback = Read,
											.commandName = "read",
											.description = NULL};
	if(CLI_AddCommand(&item_read) == false){
		USART_WriteString("ERROR in adding new item.\n\r");
	}
//***********************************************************************
	static CLI_CommandItem item_bwrite = {.callback = Bwrite,
											.commandName = "bwrite",
											.description = NULL};
	if(CLI_AddCommand(&item_bwrite) == false){
		USART_WriteString("ERROR in adding new item.\n\r");
	}
//***********************************************************************
	static CLI_CommandItem item_bread = {.callback = Bread,
											.commandName = "bread",
											.description = NULL};
	if(CLI_AddCommand(&item_bread) == false){
		USART_WriteString("ERROR in adding new item.\n\r");
	}
//***********************************************************************


}
