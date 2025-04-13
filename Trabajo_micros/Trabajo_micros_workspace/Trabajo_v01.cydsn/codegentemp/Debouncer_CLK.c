/*******************************************************************************
* File Name: Debouncer_CLK.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the Debouncer_CLK
*  component
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Debouncer_CLK.h"

/** Indicates whether or not the Debouncer_CLK has been initialized. 
*  The variable is initialized to 0 and set to 1 the first time 
*  Debouncer_CLK_Start() is called. This allows the Component to 
*  restart without reinitialization after the first call to 
*  the Debouncer_CLK_Start() routine.
*/
uint8_t Debouncer_CLK_initVar = 0U;

/** The instance-specific configuration structure. This should be used in the 
*  associated Debouncer_CLK_Init() function.
*/ 
cy_stc_tcpwm_counter_config_t const Debouncer_CLK_config =
{
        .period = 2147483647UL,
        .clockPrescaler = 0UL,
        .runMode = 1UL,
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
* Function Name: Debouncer_CLK_Start
****************************************************************************//**
*
*  Calls the Debouncer_CLK_Init() when called the first time and enables 
*  the Debouncer_CLK. For subsequent calls the configuration is left 
*  unchanged and the component is just enabled.
*
* \globalvars
*  \ref Debouncer_CLK_initVar
*
*******************************************************************************/
void Debouncer_CLK_Start(void)
{
    if (0U == Debouncer_CLK_initVar)
    {
        (void)Cy_TCPWM_Counter_Init(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, &Debouncer_CLK_config); 

        Debouncer_CLK_initVar = 1U;
    }

    Cy_TCPWM_Enable_Multiple(Debouncer_CLK_HW, Debouncer_CLK_CNT_MASK);
    
    #if (Debouncer_CLK_INPUT_DISABLED == 7UL)
        Cy_TCPWM_TriggerStart(Debouncer_CLK_HW, Debouncer_CLK_CNT_MASK);
    #endif /* (Debouncer_CLK_INPUT_DISABLED == 7UL) */    
}


/* [] END OF FILE */
