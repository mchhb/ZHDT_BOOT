/* 
  ******************************************************************************
  * @file    ZHDT
  * @author  June_hhb
  * @version 
  * @date    
  * @brief   
  *          
  *          
  *********************************************       *********************************
*/
#include "LOOP.h"
#include "delay.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h> 

int main(void)
{

	Systick_Init(72); //����ϵͳʱ��Ϊ72M   
	
  Initialization_configuration();
	
	for(;;) 
	{
		
		application();
		
	}
}



