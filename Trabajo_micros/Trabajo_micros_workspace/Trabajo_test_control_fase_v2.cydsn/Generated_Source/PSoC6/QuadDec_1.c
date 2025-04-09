/*******************************************************************************
* File Name: QuadDec_1.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the QuadDec_1
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "QuadDec_1.h"


/** Indicates whether or not the QuadDec_1 has been initialized. 
*  The variable is initialized to 0 and set to 1 the first time 
*  QuadDec_1_Start() is called. This allows the Component to 
*  restart without reinitialization after the first call to 
*  the QuadDec_1_Start() routine.
*/
uint8_t QuadDec_1_initVar = 0U;

/** The instance-specific configuration structure. This should be used in the 
*  associated QuadDec_1_Init() function.
*/ 
cy_stc_tcpwm_quaddec_config_t const QuadDec_1_config =
{
	.resolution = 0UL,
	.interruptSources = 0UL,
	.indexInputMode = 3UL,
	.indexInput = CY_TCPWM_INPUT_CREATOR,
	.stopInputMode = 3UL,
	.stopInput = CY_TCPWM_INPUT_CREATOR,
    .phiAInput = CY_TCPWM_INPUT_CREATOR,
    .phiBInput = CY_TCPWM_INPUT_CREATOR,
};


/*******************************************************************************
* Function Name: QuadDec_1_Start
****************************************************************************//**
*
*  Calls the QuadDec_1_Init() when called the first time and enables 
*  the QuadDec_1. For subsequent calls the configuration is left 
*  unchanged and the component is just enabled.
*
* \globalvars
*  \ref QuadDec_1_initVar
*
*******************************************************************************/
void QuadDec_1_Start(void)
{
    if (0U == QuadDec_1_initVar)
    {
        (void)Cy_TCPWM_QuadDec_Init(QuadDec_1_HW, QuadDec_1_CNT_NUM, &QuadDec_1_config);

        QuadDec_1_initVar = 1U;
    }

    Cy_TCPWM_Enable_Multiple(QuadDec_1_HW, QuadDec_1_CNT_MASK);
    
    Cy_TCPWM_TriggerReloadOrIndex(QuadDec_1_HW, QuadDec_1_CNT_MASK);
}


/* [] END OF FILE */
