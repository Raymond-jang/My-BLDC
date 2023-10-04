/*
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************************************************/

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** BG           Blandine Guillot                                              **
** JO           Julia Ott                                                     **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2020-04-15, BG:   Initial version of revision history              **
** V1.0.1: 2020-07-21, BG:   EP-439: Formatted .h/.c files                    **
** V1.1.0: 2022-04-01, JO:   EP-1113: Updated example for IAR to work with    **
**                           external terminal                                **
*******************************************************************************/

/*******************************************************************************
**                                  Abstract                                  **
********************************************************************************
** UART2: Send example                                                        **
********************************************************************************
** In a terminal window set up a serial connection @115.2kBaud through the    **
** debugger port.                                                             **
** Send "Hello World!" through the STDIO @ 115200 baud                        **
** STDIO uses UART2 P1.1/P1.2 through on-board debugger and VCOM              **
*******************************************************************************/

#include "tle_device.h"
#include "eval_board.h"
#include <stdio.h>

/*******************************************************************************
**                          Private Macro Definitions                         **
*******************************************************************************/

/*******************************************************************************
**                        Private Function Declarations                       **
*******************************************************************************/
void Task_1ms(void);
void Poti_Handler(void);

/*******************************************************************************
**                        Private Variable Definitions                        **
*******************************************************************************/
static uint16 ms;
static  uint16 mV;
	
/*******************************************************************************
**                        Private Constant Definitions                        **
*******************************************************************************/

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

int main(void)
{
  /* Initialization of the hardware modules based on Config Wizard config */
  TLE_Init();
	
  /* Start Timer13 */
  CCU6_StartTmr_T13();

  /*****************************************************************************
  ** Place your application code here                                         **
  *****************************************************************************/
	ms = 0;

	printf("\n\n Hello World!\n");

  /*****************************************************************************
  ** Main endless loop                                                        **
  *****************************************************************************/
  for (;;)
  {
    /* Main watchdog1 (WDT1) service */
    (void)WDT1_Service();
//  printf("Hello World!\n");
    
    /***************************************************************************
    ** Place your application code here                                       **
    ***************************************************************************/
  

		
		printf("%d \n", mV);
	}
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

void Task_1ms(void)
{
	ms++;
	
	if (ms > 1000)
	{
		PORT_P01_Output_Toggle_Set();
		ms = 0;
	}
}

void Poti_Handler(void)
{
#define Interval_mV 625u
//  uint16 mV;
  bool value_valid;
  value_valid = ADC1_GetEIMResult_mV(&mV);

  if (value_valid == true)
  {

  }
}



/*
void Poti_Handler(void)
{


// Read the value at Ch4 (Poti) in mV - values between 0 and 5000 are possible 
  if (ADC1_GetChResult_mV(&mV, ADC1_CH4) == true)
  {
//   Emo_SetRefSpeed(mV / 2);
		
    if (mV > 100)
    {
//      Main_lStartMotor();
      PORT_P01_Output_High_Set();
    }
    else
    {
//      Main_lStopMotor();
      PORT_P01_Output_Low_Set();
    }
		printf("%d \n", mV);
  }
}
*/

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000)
  #pragma clang diagnostic pop
#endif

