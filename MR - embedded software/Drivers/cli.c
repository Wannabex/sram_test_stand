#include "cli.h"

#include <string.h>
#include <ctype.h>

// head of CLI command list
static CLI_CommandItem *head = NULL;

// char buffer where command will be stored
static char commandBuffer[1500];

/**
 * This function searches the CLI command list and tries to find a descriptor for the provided command.
 * The command format is case-insensitive.
 * Returns pointer to @ref CLI_MenuItem structure if given command was found in the CLI command list.
 *
 * @param command pointer to command (string)
 *
 * @retval pointer to @ref CLI_MenuItem structure desrcibing the command, if command was found
 * @retval NULL if the given command does not match any command regitstered in the CLI command list 
 */
static CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command);

/**
 * @bref This function is responsible for collecting the command reading in from USART.
 *
 * This function should check wether the USART interface has some new data. If it does
 * this function reads new characters and stores them in a command buffer. This function
 * is also responsible for providing echo of the input characters back to the buffer.
 *
 * The function exits when:
 * - no more characters are available to read from USART - the function returns false
 * - new line was detected - this function returns true
 *
 * @retval true if command was collected
 * @retval false if command is yet incomplete
 */
static bool CLI_StoreCommand(void);

/**
 * @brief This function converts string to a lowercase
 *
 * @param dst pointer where converted null terminated string will be stored
 * @param src pointer to string which will be converted
 */
static void CLI_StringToLower(char *dst, const char *src);
	
	
	
void CLI_Proc(void){
	uint8_t count=0;
	
	if(CLI_StoreCommand()){
		CLI_StringToLower(commandBuffer,commandBuffer);
		
		while(commandBuffer[count]!=' '&&commandBuffer[count]!=0){
			count++;
		}
		commandBuffer[count]=0;
		
		if(CLI_GetMenuItemByCommandName(commandBuffer)==NULL){
			USART_WriteString("No command\r\n");
		}else{
			CLI_GetMenuItemByCommandName(commandBuffer)->callback(commandBuffer+count+1);
		}
	}
	
}

bool CLI_AddCommand(CLI_CommandItem *item){
	
	if(!(item->callback==NULL /*&& *item->commandName==0*/)){
		
	item->next=head;
	head=item;
		
	}else{
		return false;
	}
	return true;
}

void CLI_PrintAllCommands(void){
	
	
CLI_CommandItem *tmp = head;
	
	while(tmp){
		USART_WriteString(tmp->commandName);
		tmp=tmp->next;
	}
}

CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command){
	
	CLI_CommandItem *tmp = head;
	
	while(tmp){
		if(!(strcmp(tmp->commandName,command))){
			return tmp;
		}
		tmp=tmp->next;
	}
	
	return NULL;
}

void CLI_StringToLower(char *dst, const char *src){
	
	uint8_t i = 0;
	 while( src[i] ) {
      dst[i]=(tolower(src[i]));
      i++;
   }
	
}

bool CLI_StoreCommand(){
	char c;
	static uint8_t count=0;

	if(USART_GetChar(&c)){
		if(!(c=='\n')){
			if(c==0x7F){
				if(count>0){
				//USART_WriteData(&c,1);
				count--;
				}
			}else{
				//USART_WriteData(&c,1);
				commandBuffer[count]=c;
				count++;
			}
		}else{
			//USART_WriteData("\r\n",2);
			commandBuffer[count]=0;
			count=0;
			return true;
		}
	}
	
	return false;
}
