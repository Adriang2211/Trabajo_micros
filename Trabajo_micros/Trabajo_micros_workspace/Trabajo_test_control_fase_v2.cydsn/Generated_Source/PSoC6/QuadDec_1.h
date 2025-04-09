/*******************************************************************************
* File Name: QuadDec_1.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the QuadDec_1
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(QuadDec_1_CY_TCPWM_QUADDEC_PDL_H)
#define QuadDec_1_CY_TCPWM_QUADDEC_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_quaddec.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  QuadDec_1_initVar;
extern cy_stc_tcpwm_quaddec_config_t const QuadDec_1_config;
/** @} group_globals */

    
/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void QuadDec_1_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t QuadDec_1_Init(cy_stc_tcpwm_quaddec_config_t const *config);
__STATIC_INLINE void QuadDec_1_DeInit(void);
__STATIC_INLINE void QuadDec_1_Enable(void);
__STATIC_INLINE void QuadDec_1_Disable(void);
__STATIC_INLINE uint32_t QuadDec_1_GetStatus(void);
__STATIC_INLINE uint32_t QuadDec_1_GetCapture(void);
__STATIC_INLINE uint32_t QuadDec_1_GetCaptureBuf(void);
__STATIC_INLINE void QuadDec_1_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t QuadDec_1_GetCounter(void);
__STATIC_INLINE void QuadDec_1_TriggerIndex(void);
__STATIC_INLINE void QuadDec_1_TriggerStop(void);
__STATIC_INLINE uint32_t QuadDec_1_GetInterruptStatus(void);
__STATIC_INLINE void QuadDec_1_ClearInterrupt(uint32_t source);
__STATIC_INLINE void QuadDec_1_SetInterrupt(uint32_t source);
__STATIC_INLINE void QuadDec_1_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t QuadDec_1_GetInterruptMask(void);
__STATIC_INLINE uint32_t QuadDec_1_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define QuadDec_1_HW                 (QuadDec_1_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define QuadDec_1_CNT_HW             (QuadDec_1_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define QuadDec_1_CNT_NUM            (QuadDec_1_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define QuadDec_1_CNT_MASK           (1UL << QuadDec_1_CNT_NUM)
/** @} group_macros */

#define QuadDec_1_INPUT_MODE_MASK    (0x3U)


/*******************************************************************************
* Function Name: QuadDec_1_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t QuadDec_1_Init(cy_stc_tcpwm_quaddec_config_t const *config)
{
    return(Cy_TCPWM_QuadDec_Init(QuadDec_1_HW, QuadDec_1_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: QuadDec_1_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_DeInit(void)                   
{
    Cy_TCPWM_QuadDec_DeInit(QuadDec_1_HW, QuadDec_1_CNT_NUM, &QuadDec_1_config);
}


/*******************************************************************************
* Function Name: QuadDec_1_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(QuadDec_1_HW, QuadDec_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: QuadDec_1_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(QuadDec_1_HW, QuadDec_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: QuadDec_1_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t QuadDec_1_GetStatus(void)                
{
    return(Cy_TCPWM_QuadDec_GetStatus(QuadDec_1_HW, QuadDec_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: QuadDec_1_GetCapture
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_GetCapture() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t QuadDec_1_GetCapture(void)               
{
    return(Cy_TCPWM_QuadDec_GetCapture(QuadDec_1_HW, QuadDec_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: QuadDec_1_GetCaptureBuf
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_GetCaptureBuf() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t QuadDec_1_GetCaptureBuf(void)            
{
    return(Cy_TCPWM_QuadDec_GetCaptureBuf(QuadDec_1_HW, QuadDec_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: QuadDec_1_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_SetCounter(uint32_t count)          
{
    Cy_TCPWM_QuadDec_SetCounter(QuadDec_1_HW, QuadDec_1_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: QuadDec_1_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t QuadDec_1_GetCounter(void)               
{
    return(Cy_TCPWM_QuadDec_GetCounter(QuadDec_1_HW, QuadDec_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: QuadDec_1_TriggerIndex
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_TriggerIndex(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(QuadDec_1_HW, QuadDec_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: QuadDec_1_TriggerStop
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_TriggerStop(void)
{
    Cy_TCPWM_TriggerStopOrKill(QuadDec_1_HW, QuadDec_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: QuadDec_1_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t QuadDec_1_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(QuadDec_1_HW, QuadDec_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: QuadDec_1_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(QuadDec_1_HW, QuadDec_1_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: QuadDec_1_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(QuadDec_1_HW, QuadDec_1_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: QuadDec_1_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void QuadDec_1_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(QuadDec_1_HW, QuadDec_1_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: QuadDec_1_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t QuadDec_1_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(QuadDec_1_HW, QuadDec_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: QuadDec_1_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t QuadDec_1_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(QuadDec_1_HW, QuadDec_1_CNT_NUM));
}

#endif /* QuadDec_1_CY_TCPWM_QUADDEC_PDL_H */


/* [] END OF FILE */
