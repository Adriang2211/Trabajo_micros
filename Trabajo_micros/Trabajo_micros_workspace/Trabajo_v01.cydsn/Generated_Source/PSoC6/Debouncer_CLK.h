/*******************************************************************************
* File Name: Debouncer_CLK.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the Debouncer_CLK
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(Debouncer_CLK_CY_TCPWM_COUNTER_PDL_H)
#define Debouncer_CLK_CY_TCPWM_COUNTER_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_counter.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  Debouncer_CLK_initVar;
extern cy_stc_tcpwm_counter_config_t const Debouncer_CLK_config;
/** @} group_globals */


/***************************************
*  Conditional Compilation Parameters
***************************************/

#define Debouncer_CLK_INIT_COMPARE_OR_CAPTURE    (2UL)


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void Debouncer_CLK_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t Debouncer_CLK_Init(cy_stc_tcpwm_counter_config_t const *config);
__STATIC_INLINE void Debouncer_CLK_DeInit(void);
__STATIC_INLINE void Debouncer_CLK_Enable(void);
__STATIC_INLINE void Debouncer_CLK_Disable(void);
__STATIC_INLINE uint32_t Debouncer_CLK_GetStatus(void);

#if(CY_TCPWM_COUNTER_MODE_CAPTURE == Debouncer_CLK_INIT_COMPARE_OR_CAPTURE)
    __STATIC_INLINE uint32_t Debouncer_CLK_GetCapture(void);
    __STATIC_INLINE uint32_t Debouncer_CLK_GetCaptureBuf(void);
#else
    __STATIC_INLINE void Debouncer_CLK_SetCompare0(uint32_t compare0);
    __STATIC_INLINE uint32_t Debouncer_CLK_GetCompare0(void);
    __STATIC_INLINE void Debouncer_CLK_SetCompare1(uint32_t compare1);
    __STATIC_INLINE uint32_t Debouncer_CLK_GetCompare1(void);
    __STATIC_INLINE void Debouncer_CLK_EnableCompareSwap(bool enable);
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == Debouncer_CLK_INIT_COMPARE_OR_CAPTURE) */

__STATIC_INLINE void Debouncer_CLK_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t Debouncer_CLK_GetCounter(void);
__STATIC_INLINE void Debouncer_CLK_SetPeriod(uint32_t period);
__STATIC_INLINE uint32_t Debouncer_CLK_GetPeriod(void);
__STATIC_INLINE void Debouncer_CLK_TriggerStart(void);
__STATIC_INLINE void Debouncer_CLK_TriggerReload(void);
__STATIC_INLINE void Debouncer_CLK_TriggerStop(void);
__STATIC_INLINE void Debouncer_CLK_TriggerCapture(void);
__STATIC_INLINE uint32_t Debouncer_CLK_GetInterruptStatus(void);
__STATIC_INLINE void Debouncer_CLK_ClearInterrupt(uint32_t source);
__STATIC_INLINE void Debouncer_CLK_SetInterrupt(uint32_t source);
__STATIC_INLINE void Debouncer_CLK_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t Debouncer_CLK_GetInterruptMask(void);
__STATIC_INLINE uint32_t Debouncer_CLK_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define Debouncer_CLK_HW                 (Debouncer_CLK_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define Debouncer_CLK_CNT_HW             (Debouncer_CLK_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define Debouncer_CLK_CNT_NUM            (Debouncer_CLK_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define Debouncer_CLK_CNT_MASK           (1UL << Debouncer_CLK_CNT_NUM)
/** @} group_macros */

#define Debouncer_CLK_INPUT_MODE_MASK    (0x3U)
#define Debouncer_CLK_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: Debouncer_CLK_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t Debouncer_CLK_Init(cy_stc_tcpwm_counter_config_t const *config)
{
    return(Cy_TCPWM_Counter_Init(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: Debouncer_CLK_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_DeInit(void)                   
{
    Cy_TCPWM_Counter_DeInit(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, &Debouncer_CLK_config);
}

/*******************************************************************************
* Function Name: Debouncer_CLK_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(Debouncer_CLK_HW, Debouncer_CLK_CNT_MASK);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(Debouncer_CLK_HW, Debouncer_CLK_CNT_MASK);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Debouncer_CLK_GetStatus(void)                
{
    return(Cy_TCPWM_Counter_GetStatus(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
}


#if(CY_TCPWM_COUNTER_MODE_CAPTURE == Debouncer_CLK_INIT_COMPARE_OR_CAPTURE)
    /*******************************************************************************
    * Function Name: Debouncer_CLK_GetCapture
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCapture() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Debouncer_CLK_GetCapture(void)               
    {
        return(Cy_TCPWM_Counter_GetCapture(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Debouncer_CLK_GetCaptureBuf
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCaptureBuf() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Debouncer_CLK_GetCaptureBuf(void)            
    {
        return(Cy_TCPWM_Counter_GetCaptureBuf(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
    }
#else
    /*******************************************************************************
    * Function Name: Debouncer_CLK_SetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Debouncer_CLK_SetCompare0(uint32_t compare0)      
    {
        Cy_TCPWM_Counter_SetCompare0(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, compare0);
    }


    /*******************************************************************************
    * Function Name: Debouncer_CLK_GetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Debouncer_CLK_GetCompare0(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare0(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Debouncer_CLK_SetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Debouncer_CLK_SetCompare1(uint32_t compare1)      
    {
        Cy_TCPWM_Counter_SetCompare1(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, compare1);
    }


    /*******************************************************************************
    * Function Name: Debouncer_CLK_GetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Debouncer_CLK_GetCompare1(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare1(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Debouncer_CLK_EnableCompareSwap
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_EnableCompareSwap() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Debouncer_CLK_EnableCompareSwap(bool enable)  
    {
        Cy_TCPWM_Counter_EnableCompareSwap(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, enable);
    }
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == Debouncer_CLK_INIT_COMPARE_OR_CAPTURE) */


/*******************************************************************************
* Function Name: Debouncer_CLK_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_SetCounter(uint32_t count)          
{
    Cy_TCPWM_Counter_SetCounter(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Debouncer_CLK_GetCounter(void)               
{
    return(Cy_TCPWM_Counter_GetCounter(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
}


/*******************************************************************************
* Function Name: Debouncer_CLK_SetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_SetPeriod(uint32_t period)          
{
    Cy_TCPWM_Counter_SetPeriod(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, period);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_GetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Debouncer_CLK_GetPeriod(void)                
{
    return(Cy_TCPWM_Counter_GetPeriod(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
}


/*******************************************************************************
* Function Name: Debouncer_CLK_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(Debouncer_CLK_HW, Debouncer_CLK_CNT_MASK);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(Debouncer_CLK_HW, Debouncer_CLK_CNT_MASK);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_TriggerStop
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_TriggerStop(void)
{
    Cy_TCPWM_TriggerStopOrKill(Debouncer_CLK_HW, Debouncer_CLK_CNT_MASK);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_TriggerCapture
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_TriggerCapture(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(Debouncer_CLK_HW, Debouncer_CLK_CNT_MASK);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Debouncer_CLK_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
}


/*******************************************************************************
* Function Name: Debouncer_CLK_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Debouncer_CLK_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: Debouncer_CLK_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Debouncer_CLK_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
}


/*******************************************************************************
* Function Name: Debouncer_CLK_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Debouncer_CLK_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(Debouncer_CLK_HW, Debouncer_CLK_CNT_NUM));
}

#endif /* Debouncer_CLK_CY_TCPWM_COUNTER_PDL_H */


/* [] END OF FILE */
