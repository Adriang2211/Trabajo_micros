/*******************************************************************************
* File Name: Encoder.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the Encoder
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(Encoder_CY_TCPWM_QUADDEC_PDL_H)
#define Encoder_CY_TCPWM_QUADDEC_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_quaddec.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  Encoder_initVar;
extern cy_stc_tcpwm_quaddec_config_t const Encoder_config;
/** @} group_globals */

    
/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void Encoder_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t Encoder_Init(cy_stc_tcpwm_quaddec_config_t const *config);
__STATIC_INLINE void Encoder_DeInit(void);
__STATIC_INLINE void Encoder_Enable(void);
__STATIC_INLINE void Encoder_Disable(void);
__STATIC_INLINE uint32_t Encoder_GetStatus(void);
__STATIC_INLINE uint32_t Encoder_GetCapture(void);
__STATIC_INLINE uint32_t Encoder_GetCaptureBuf(void);
__STATIC_INLINE void Encoder_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t Encoder_GetCounter(void);
__STATIC_INLINE void Encoder_TriggerIndex(void);
__STATIC_INLINE void Encoder_TriggerStop(void);
__STATIC_INLINE uint32_t Encoder_GetInterruptStatus(void);
__STATIC_INLINE void Encoder_ClearInterrupt(uint32_t source);
__STATIC_INLINE void Encoder_SetInterrupt(uint32_t source);
__STATIC_INLINE void Encoder_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t Encoder_GetInterruptMask(void);
__STATIC_INLINE uint32_t Encoder_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define Encoder_HW                 (Encoder_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define Encoder_CNT_HW             (Encoder_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define Encoder_CNT_NUM            (Encoder_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define Encoder_CNT_MASK           (1UL << Encoder_CNT_NUM)
/** @} group_macros */

#define Encoder_INPUT_MODE_MASK    (0x3U)


/*******************************************************************************
* Function Name: Encoder_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t Encoder_Init(cy_stc_tcpwm_quaddec_config_t const *config)
{
    return(Cy_TCPWM_QuadDec_Init(Encoder_HW, Encoder_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: Encoder_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_DeInit(void)                   
{
    Cy_TCPWM_QuadDec_DeInit(Encoder_HW, Encoder_CNT_NUM, &Encoder_config);
}


/*******************************************************************************
* Function Name: Encoder_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(Encoder_HW, Encoder_CNT_MASK);
}


/*******************************************************************************
* Function Name: Encoder_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(Encoder_HW, Encoder_CNT_MASK);
}


/*******************************************************************************
* Function Name: Encoder_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Encoder_GetStatus(void)                
{
    return(Cy_TCPWM_QuadDec_GetStatus(Encoder_HW, Encoder_CNT_NUM));
}


/*******************************************************************************
* Function Name: Encoder_GetCapture
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_GetCapture() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Encoder_GetCapture(void)               
{
    return(Cy_TCPWM_QuadDec_GetCapture(Encoder_HW, Encoder_CNT_NUM));
}


/*******************************************************************************
* Function Name: Encoder_GetCaptureBuf
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_GetCaptureBuf() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Encoder_GetCaptureBuf(void)            
{
    return(Cy_TCPWM_QuadDec_GetCaptureBuf(Encoder_HW, Encoder_CNT_NUM));
}


/*******************************************************************************
* Function Name: Encoder_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_SetCounter(uint32_t count)          
{
    Cy_TCPWM_QuadDec_SetCounter(Encoder_HW, Encoder_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: Encoder_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_QuadDec_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Encoder_GetCounter(void)               
{
    return(Cy_TCPWM_QuadDec_GetCounter(Encoder_HW, Encoder_CNT_NUM));
}


/*******************************************************************************
* Function Name: Encoder_TriggerIndex
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_TriggerIndex(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(Encoder_HW, Encoder_CNT_MASK);
}


/*******************************************************************************
* Function Name: Encoder_TriggerStop
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_TriggerStop(void)
{
    Cy_TCPWM_TriggerStopOrKill(Encoder_HW, Encoder_CNT_MASK);
}


/*******************************************************************************
* Function Name: Encoder_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Encoder_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(Encoder_HW, Encoder_CNT_NUM));
}


/*******************************************************************************
* Function Name: Encoder_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(Encoder_HW, Encoder_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Encoder_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(Encoder_HW, Encoder_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Encoder_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Encoder_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(Encoder_HW, Encoder_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: Encoder_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Encoder_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(Encoder_HW, Encoder_CNT_NUM));
}


/*******************************************************************************
* Function Name: Encoder_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Encoder_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(Encoder_HW, Encoder_CNT_NUM));
}

#endif /* Encoder_CY_TCPWM_QUADDEC_PDL_H */


/* [] END OF FILE */
