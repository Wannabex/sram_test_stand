/* Includes ------------------------------------------------------------------*/
#include "core.h"
#include "main.h"


void CORE_EnterCriticalSection(void){	
	__disable_irq();
}

void CORE_ExitCriticalSection(void) {
	__enable_irq();
}
