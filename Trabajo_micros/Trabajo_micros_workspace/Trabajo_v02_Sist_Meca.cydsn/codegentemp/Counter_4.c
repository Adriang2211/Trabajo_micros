/*******************************************************************************
* File Name: Counter_4.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the Counter_4
*  component
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Counter_4.h"

/** Indicates whether or not the Counter_4 has been initialized. 
*  The variable is initialized to 0 and set to 1 the first time 
*  Counter_4_Start() is called. This allows the Component to 
*  restart without reinitialization after the first call to 
*  the Counter_4_Start() routine.
*/
uint8_t Counter_4_initVar = 0U;

/** The instance-specific configuration structure. This should be used in the 
*  associated Counter_4_Init() function.
*/ 
cy_stc_tcpwm_counter_config_t const Counter_4_config =
{
        .period = 100UL,
        .clockPrescaler = 0UL,
        .runMode = 0UL,
        .countDirection = 0UL,
        .compareOrCapture = 2UL,
        .compare0 = 16384UL,
        .compare1 = 16384UL,
        .enableCompareSwap = false,
        .interruptSources = 1UL,
        .captureInputMode = 3UL,
        .captureInput = CY_TCPWM_INPUT_CREATOR,
        .reloadInputMode = 3UL,
        .reloadInput = CY_TCPWM_INPUT_CREATOR,
        .startInputMode = 3UL,
        .startInput = CY_TCPWM_INPUT_CREATOR,
        .stopInputMode = 3UL,
        .stopInput = CY_TCPWM_INPUT_CREATOR,
        .countInputMode = 3UL,
        .countInput = CY_TCPWM_INPUT_CREATOR,
};


/*******************************************************************************
* Function Name: Counter_4_Start
****************************************************************************//**
*
*  Calls the Counter_4_Init() when called the first time and enables 
*  the Counter_4. For subsequent calls the configuration is left 
*  unchanged and the component is just enabled.
*
* \globalvars
*  \ref Counter_4_initVar
*
*******************************************************************************/
void Counter_4_Start(void)
{
    if (0U == Counter_4_initVar)
    {
        (void)Cy_TCPWM_Counter_Init(Counter_4_HW, Counter_4_CNT_NUM, &Counter_4_config); 

        Counter_4_initVar = 1U;
    }

    Cy_TCPWM_Enable_Multiple(Counter_4_HW, Counter_4_CNT_MASK);
    
    #if (Counter_4_INPUT_DISABLED == 7UL)
        Cy_TCPWM_TriggerStart(Counter_4_HW, Counter_4_CNT_MASK);
    #endif /* (Counter_4_INPUT_DISABLED == 7UL) */    
}


/* [] END OF FILE */
