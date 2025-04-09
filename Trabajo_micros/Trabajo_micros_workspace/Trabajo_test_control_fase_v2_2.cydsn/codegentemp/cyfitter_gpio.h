/*******************************************************************************
* File Name: cyfitter_gpio.h
* 
* PSoC Creator  4.4
*
* Description:
* 
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2020 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef INCLUDED_CYFITTER_GPIO_H
#define INCLUDED_CYFITTER_GPIO_H
#include "cy_device_headers.h"

/* LED_rojo */
#define LED_rojo_0_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW_IN_OFF
#define LED_rojo_0_INBUF_ENABLED 0u
#define LED_rojo_0_INIT_DRIVESTATE 0u
#define LED_rojo_0_INIT_MUXSEL 0u
#define LED_rojo_0_INPUT_SYNC 2u
#define LED_rojo_0_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define LED_rojo_0_NUM 3u
#define LED_rojo_0_PORT GPIO_PRT6
#define LED_rojo_0_SLEWRATE CY_GPIO_SLEW_SLOW
#define LED_rojo_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define LED_rojo_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW_IN_OFF
#define LED_rojo_INBUF_ENABLED 0u
#define LED_rojo_INIT_DRIVESTATE 0u
#define LED_rojo_INIT_MUXSEL 0u
#define LED_rojo_INPUT_SYNC 2u
#define LED_rojo_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define LED_rojo_NUM 3u
#define LED_rojo_PORT GPIO_PRT6
#define LED_rojo_SLEWRATE CY_GPIO_SLEW_SLOW
#define LED_rojo_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS

/* LED_verde */
#define LED_verde_0_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW_IN_OFF
#define LED_verde_0_INBUF_ENABLED 0u
#define LED_verde_0_INIT_DRIVESTATE 0u
#define LED_verde_0_INIT_MUXSEL 0u
#define LED_verde_0_INPUT_SYNC 2u
#define LED_verde_0_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define LED_verde_0_NUM 1u
#define LED_verde_0_PORT GPIO_PRT7
#define LED_verde_0_SLEWRATE CY_GPIO_SLEW_SLOW
#define LED_verde_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define LED_verde_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW_IN_OFF
#define LED_verde_INBUF_ENABLED 0u
#define LED_verde_INIT_DRIVESTATE 0u
#define LED_verde_INIT_MUXSEL 0u
#define LED_verde_INPUT_SYNC 2u
#define LED_verde_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define LED_verde_NUM 1u
#define LED_verde_PORT GPIO_PRT7
#define LED_verde_SLEWRATE CY_GPIO_SLEW_SLOW
#define LED_verde_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS

/* OPTO_trig */
#define OPTO_trig_0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define OPTO_trig_0_INBUF_ENABLED 0u
#define OPTO_trig_0_INIT_DRIVESTATE 0u
#define OPTO_trig_0_INIT_MUXSEL 0u
#define OPTO_trig_0_INPUT_SYNC 2u
#define OPTO_trig_0_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define OPTO_trig_0_NUM 3u
#define OPTO_trig_0_PORT GPIO_PRT5
#define OPTO_trig_0_SLEWRATE CY_GPIO_SLEW_FAST
#define OPTO_trig_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define OPTO_trig_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define OPTO_trig_INBUF_ENABLED 0u
#define OPTO_trig_INIT_DRIVESTATE 0u
#define OPTO_trig_INIT_MUXSEL 0u
#define OPTO_trig_INPUT_SYNC 2u
#define OPTO_trig_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define OPTO_trig_NUM 3u
#define OPTO_trig_PORT GPIO_PRT5
#define OPTO_trig_SLEWRATE CY_GPIO_SLEW_FAST
#define OPTO_trig_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS

/* UART_1_rx */
#define UART_1_rx_0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define UART_1_rx_0_INBUF_ENABLED 1u
#define UART_1_rx_0_INIT_DRIVESTATE 1u
#define UART_1_rx_0_INIT_MUXSEL 18u
#define UART_1_rx_0_INPUT_SYNC 2u
#define UART_1_rx_0_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define UART_1_rx_0_NUM 0u
#define UART_1_rx_0_PORT GPIO_PRT5
#define UART_1_rx_0_SLEWRATE CY_GPIO_SLEW_FAST
#define UART_1_rx_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define UART_1_rx_DRIVEMODE CY_GPIO_DM_HIGHZ
#define UART_1_rx_INBUF_ENABLED 1u
#define UART_1_rx_INIT_DRIVESTATE 1u
#define UART_1_rx_INIT_MUXSEL 18u
#define UART_1_rx_INPUT_SYNC 2u
#define UART_1_rx_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define UART_1_rx_NUM 0u
#define UART_1_rx_PORT GPIO_PRT5
#define UART_1_rx_SLEWRATE CY_GPIO_SLEW_FAST
#define UART_1_rx_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS

/* UART_1_tx */
#define UART_1_tx_0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define UART_1_tx_0_INBUF_ENABLED 0u
#define UART_1_tx_0_INIT_DRIVESTATE 1u
#define UART_1_tx_0_INIT_MUXSEL 18u
#define UART_1_tx_0_INPUT_SYNC 2u
#define UART_1_tx_0_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define UART_1_tx_0_NUM 1u
#define UART_1_tx_0_PORT GPIO_PRT5
#define UART_1_tx_0_SLEWRATE CY_GPIO_SLEW_FAST
#define UART_1_tx_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define UART_1_tx_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define UART_1_tx_INBUF_ENABLED 0u
#define UART_1_tx_INIT_DRIVESTATE 1u
#define UART_1_tx_INIT_MUXSEL 18u
#define UART_1_tx_INPUT_SYNC 2u
#define UART_1_tx_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define UART_1_tx_NUM 1u
#define UART_1_tx_PORT GPIO_PRT5
#define UART_1_tx_SLEWRATE CY_GPIO_SLEW_FAST
#define UART_1_tx_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS

/* OPTO_PC817 */
#define OPTO_PC817_0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define OPTO_PC817_0_INBUF_ENABLED 1u
#define OPTO_PC817_0_INIT_DRIVESTATE 0u
#define OPTO_PC817_0_INIT_MUXSEL 0u
#define OPTO_PC817_0_INPUT_SYNC 2u
#define OPTO_PC817_0_INTERRUPT_MODE CY_GPIO_INTR_RISING
#define OPTO_PC817_0_NUM 2u
#define OPTO_PC817_0_PORT GPIO_PRT5
#define OPTO_PC817_0_SLEWRATE CY_GPIO_SLEW_FAST
#define OPTO_PC817_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define OPTO_PC817_DRIVEMODE CY_GPIO_DM_HIGHZ
#define OPTO_PC817_INBUF_ENABLED 1u
#define OPTO_PC817_INIT_DRIVESTATE 0u
#define OPTO_PC817_INIT_MUXSEL 0u
#define OPTO_PC817_INPUT_SYNC 2u
#define OPTO_PC817_INTERRUPT_MODE CY_GPIO_INTR_RISING
#define OPTO_PC817_NUM 2u
#define OPTO_PC817_PORT GPIO_PRT5
#define OPTO_PC817_SLEWRATE CY_GPIO_SLEW_FAST
#define OPTO_PC817_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS

/* Encoder_ChA */
#define Encoder_ChA_0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define Encoder_ChA_0_INBUF_ENABLED 1u
#define Encoder_ChA_0_INIT_DRIVESTATE 0u
#define Encoder_ChA_0_INIT_MUXSEL 0u
#define Encoder_ChA_0_INPUT_SYNC 2u
#define Encoder_ChA_0_INTERRUPT_MODE CY_GPIO_INTR_RISING
#define Encoder_ChA_0_NUM 4u
#define Encoder_ChA_0_PORT GPIO_PRT10
#define Encoder_ChA_0_SLEWRATE CY_GPIO_SLEW_FAST
#define Encoder_ChA_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define Encoder_ChA_DRIVEMODE CY_GPIO_DM_HIGHZ
#define Encoder_ChA_INBUF_ENABLED 1u
#define Encoder_ChA_INIT_DRIVESTATE 0u
#define Encoder_ChA_INIT_MUXSEL 0u
#define Encoder_ChA_INPUT_SYNC 2u
#define Encoder_ChA_INTERRUPT_MODE CY_GPIO_INTR_RISING
#define Encoder_ChA_NUM 4u
#define Encoder_ChA_PORT GPIO_PRT10
#define Encoder_ChA_SLEWRATE CY_GPIO_SLEW_FAST
#define Encoder_ChA_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS

/* Encoder_ChB */
#define Encoder_ChB_0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define Encoder_ChB_0_INBUF_ENABLED 1u
#define Encoder_ChB_0_INIT_DRIVESTATE 0u
#define Encoder_ChB_0_INIT_MUXSEL 0u
#define Encoder_ChB_0_INPUT_SYNC 2u
#define Encoder_ChB_0_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define Encoder_ChB_0_NUM 5u
#define Encoder_ChB_0_PORT GPIO_PRT10
#define Encoder_ChB_0_SLEWRATE CY_GPIO_SLEW_FAST
#define Encoder_ChB_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define Encoder_ChB_DRIVEMODE CY_GPIO_DM_HIGHZ
#define Encoder_ChB_INBUF_ENABLED 1u
#define Encoder_ChB_INIT_DRIVESTATE 0u
#define Encoder_ChB_INIT_MUXSEL 0u
#define Encoder_ChB_INPUT_SYNC 2u
#define Encoder_ChB_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define Encoder_ChB_NUM 5u
#define Encoder_ChB_PORT GPIO_PRT10
#define Encoder_ChB_SLEWRATE CY_GPIO_SLEW_FAST
#define Encoder_ChB_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS

#endif /* INCLUDED_CYFITTER_GPIO_H */
