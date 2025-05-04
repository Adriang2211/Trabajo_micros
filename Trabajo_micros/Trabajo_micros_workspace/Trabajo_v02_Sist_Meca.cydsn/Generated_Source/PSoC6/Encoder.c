/*******************************************************************************
* File Name: Encoder.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the Encoder
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Encoder.h"


/** Indicates whether or not the Encoder has been initialized. 
*  The variable is initialized to 0 and set to 1 the first time 
*  Encoder_Start() is called. This allows the Component to 
*  restart without reinitialization after the first call to 
*  the Encoder_Start() routine.
*/
uint8_t Encoder_initVar = 0U;

/** The instance-specific configuration structure. This should be used in the 
*  associated Encoder_Init() function.
*/ 
cy_stc_tcpwm_quaddec_config_t const Encoder_config =
{
	.resolution = 1UL,
	.interruptSources = 0UL,
	.indexInputMode = 3UL,
	.indexInput = CY_TCPWM_INPUT_CREATOR,
	.stopInputMode = 3UL,
	.stopInput = CY_TCPWM_INPUT_CREATOR,
    .phiAInput = CY_TCPWM_INPUT_CREATOR,
    .phiBInput = CY_TCPWM_INPUT_CREATOR,
};


/*******************************************************************************
* Function Name: Encoder_Start
****************************************************************************//**
*
*  Calls the Encoder_Init() when called the first time and enables 
*  the Encoder. For subsequent calls the configuration is left 
*  unchanged and the component is just enabled.
*
* \globalvars
*  \ref Encoder_initVar
*
*******************************************************************************/
void Encoder_Start(void)
{
    if (0U == Encoder_initVar)
    {
        (void)Cy_TCPWM_QuadDec_Init(Encoder_HW, Encoder_CNT_NUM, &Encoder_config);

        Encoder_initVar = 1U;
    }

    Cy_TCPWM_Enable_Multiple(Encoder_HW, Encoder_CNT_MASK);
    
    Cy_TCPWM_TriggerReloadOrIndex(Encoder_HW, Encoder_CNT_MASK);
}


/* [] END OF FILE */
