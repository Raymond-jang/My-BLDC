/*
 ***********************************************************************************************************************
 *
 * Copyright (c) Infineon Technologies AG
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

/**
 * \file     main.c
 *
 * \brief    Template for main.c
 *
 * \version  V1.0.1
 * \date     28. Feb 2022
 *
 * \note
 */

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** DM           Daniel Mysliwitz                                              **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2015-08-07, DM:   Initial version                                  **
** V1.0.1: 2022-02-28, JO:   EP-936: Updated copyright and branding           **
*******************************************************************************/

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/

#include "tle_device.h"
#include "eval_board.h"

/*******************************************************************************
**                          Private Macro Definitions                         **
*******************************************************************************/

/*******************************************************************************
**                        Private Function Declarations                       **
*******************************************************************************/
void Task_1ms(void);
/*******************************************************************************
**                        Private Variable Definitions                        **
*******************************************************************************/
static uint16 ms;
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

  /*****************************************************************************
  ** Place your application code here                                         **
  *****************************************************************************/
	ms = 0;
  /*****************************************************************************
  ** Main endless loop                                                        **
  *****************************************************************************/
  for (;;)
  {
    /* Main watchdog1 (WDT1) service */
    (void)WDT1_Service();
    
    /***************************************************************************
    ** Place your application code here                                       **
    ***************************************************************************/
  }
}

void Task_1ms(void)
{
	ms++;
	
	if (ms > 1000)
	{
		PORT_P01_Output_Toggle_Set();
		ms = 0;
	}
}
