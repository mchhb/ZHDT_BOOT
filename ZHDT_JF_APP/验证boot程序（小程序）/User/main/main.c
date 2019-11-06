/* 
  ******************************************************************************
  * @file    MOTO control
  * @author  June_hhb
  * @version V0.1
  * @date    23-June-2017
  * @brief   
  *          
  *          
  *********************************************       *********************************
*/


//const int chksum __attribute__((at(0x08004188)))={0x55aa11ee};

#include "LOOP.h"
int main(void)
{
   Initialization_configuration();
	
	for(;;) 
	{
          
			application();
	}
}




