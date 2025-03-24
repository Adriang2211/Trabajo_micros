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

volatile float tiempo;
volatile int numero_ciclos;

int numero = 0;
// Rutina de atención a la interrupción del temporizador (ISR Counter 1)
void Opto_detec_IRQHandler(void){
    
    Cy_GPIO_Write(LED_rojo_PORT, LED_rojo_NUM, 1);
    Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 1);
    Cy_GPIO_Write(OPTO_trig_PORT, OPTO_trig_NUM, 0);
    Counter_1_Enable();
    Counter_1_SetCounter(0);
    Counter_1_Start();
    Counter_2_Enable();
    Counter_2_SetCounter(0);
    Counter_2_Start();
    tiempo = Counter_3_GetCounter();
    Counter_3_SetCounter(0);
    Counter_3_Start();
    numero_ciclos++;
    if (numero_ciclos == 100){
        Cy_SCB_UART_PutString(UART_1_HW, "Hola\n");  
        numero_ciclos = 0;
    }
   
    
    
    
}

void Timer_int_IRQHandler(void){
    Cy_GPIO_Write(OPTO_trig_PORT, OPTO_trig_NUM, 1);
    Counter_1_Disable();
    
}

void Timer2_int_IRQHandler(void){
    
    Cy_GPIO_Write(LED_rojo_PORT, LED_rojo_NUM, 0);
    Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 0);
    Cy_GPIO_Write(OPTO_trig_PORT, OPTO_trig_NUM, 0);
    Counter_2_Disable();
    
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //Inicialización de las comunicaciones UART
    UART_1_Start();
    Cy_SCB_UART_PutString(UART_1_HW, "Programa iniciado\n");
    
    Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 0);
    Cy_GPIO_Write(LED_rojo_PORT, LED_rojo_NUM, 0);
    Cy_GPIO_Write(OPTO_trig_PORT, OPTO_trig_NUM, 0);
    
    Counter_3_SetCounter(0);
    Counter_3_Start();
    
    // Configurar las interrupciones del optoacoplador de detección
    Cy_SysInt_Init(&Opto_detec_cfg, Opto_detec_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Opto_detec_cfg.intrSrc);
    // Habilitar las interrupciones del optoacoplador de detección
    NVIC_EnableIRQ(Opto_detec_cfg.intrSrc);
    
    
    // Configurar las interrupciones del temporizador
    Cy_SysInt_Init(&Timer_int_cfg, Timer_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Timer_int_cfg.intrSrc);
    // Habilitar las interrupciones del boton
    NVIC_EnableIRQ(Timer_int_cfg.intrSrc);
    
    // Configurar las interrupciones del temporizador 2
    Cy_SysInt_Init(&Timer2_int_cfg, Timer2_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Timer2_int_cfg.intrSrc);
    // Habilitar las interrupciones del boton
    NVIC_EnableIRQ(Timer2_int_cfg.intrSrc);
    

    for(;;)
    {
        /* Place your application code here. */
        
        // No hay código, todo el programa funciona por interrupciones
        Cy_SysLib_Delay(10000); //Demuestra que no interfiere con la interrupción
    }
}

/* [] END OF FILE */
