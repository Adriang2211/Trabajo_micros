/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>  // Para usar sprintf

// Pulsos por rotación que proporciona el encoder
#define PULSES_PER_ROTATION 10
// Relación de reducción de la reductora del motor
#define REL_REDUCTION 150

volatile long unsigned int cuentas = 0; //Debug
volatile long unsigned int cuentas1 = 0; //Debug

void Opto_detec_IRQHandler(void){
    
    
    Cy_GPIO_Write(OPTO_trig_PORT, OPTO_trig_NUM, 0);
    Counter_1_Enable();
    Counter_1_SetCounter(0);
    Counter_1_Start();
    Counter_2_Enable();
    Counter_2_SetCounter(0);
    Counter_2_Start();
}

void Timer_int_IRQHandler(void){
    
    Counter_1_ClearInterrupt(Counter_1_config.interruptSources);
    Cy_GPIO_Write(OPTO_trig_PORT, OPTO_trig_NUM, 1);
    Counter_1_Disable();
    
}

void Timer2_int_IRQHandler(void){
    
    Counter_2_ClearInterrupt(Counter_2_config.interruptSources);
    Cy_GPIO_Write(OPTO_trig_PORT, OPTO_trig_NUM, 0);
    Counter_2_Disable();
    
}

void Encoder_int_IRQHandler(void){
    
    if (Cy_GPIO_Read(Encoder_ChB_PORT, Encoder_ChB_NUM))
        cuentas++;
    else
        cuentas--;
 
}




int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //Inicialización de las comunicaciones UART
    UART_1_Start();
    Cy_SCB_UART_PutString(UART_1_HW, "Programa iniciado ahora\n");
    
    Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 0);
    Cy_GPIO_Write(OPTO_trig_PORT, OPTO_trig_NUM, 0);
    
    
    // Configurar las interrupciones del optoacoplador de detección
    Cy_SysInt_Init(&Opto_detec_int_cfg, Opto_detec_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Opto_detec_int_cfg.intrSrc);
    // Habilitar las interrupciones del optoacoplador de detección
    NVIC_EnableIRQ(Opto_detec_int_cfg.intrSrc);
    
    
    // Configurar las interrupciones del temporizador
    Cy_SysInt_Init(&Trig_int_cfg, Timer_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Trig_int_cfg.intrSrc);
    // Habilitar las interrupciones del boton
    NVIC_EnableIRQ(Trig_int_cfg.intrSrc);
    
    
    // Configurar las interrupciones del temporizador 2
    Cy_SysInt_Init(&Trig_end_int_cfg, Timer2_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Trig_end_int_cfg.intrSrc);
    // Habilitar las interrupciones del boton
    NVIC_EnableIRQ(Trig_end_int_cfg.intrSrc);
    
    
    // Configurar las interrupciones del encoder
    Cy_SysInt_Init(&Encoder_int_cfg, Encoder_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Encoder_int_cfg.intrSrc);
    // Habilitar las interrupciones del optoacoplador de detección
    NVIC_EnableIRQ(Encoder_int_cfg.intrSrc);
    
    
    // Crear un buffer limpio para COM UART
    char buffer [20];
    
    for (int i = 0; i < 20; i++)
        buffer[i] = ' ';
    
    for(;;)
    {
        /* Place your application code here. */
        
        Cy_SysLib_Delay(1000); //Demuestra que no interfiere con la interrupción
        Cy_GPIO_Inv(LED_verde_PORT, LED_verde_NUM);
        snprintf(buffer, sizeof(buffer), "%ld rpm\n", (long int)(cuentas - cuentas1)*60/PULSES_PER_ROTATION/REL_REDUCTION);
        cuentas1 = cuentas;
        Cy_SCB_UART_PutString(UART_1_HW, buffer);
        
    }
}

/* [] END OF FILE */
