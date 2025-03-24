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

// Rutina de atención a la interrupción del temporizador (ISR Counter 1)
void Boton_int_IRQHandler(void){
    
    Cy_GPIO_Write(LED_rojo_PORT, LED_rojo_NUM, 0);
    Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 1);
    Counter_1_Enable();
    Counter_1_SetCounter(0);
    Counter_1_Start();
    
    
}

void Timer_int_IRQHandler(void){
    
    Cy_GPIO_Write(LED_rojo_PORT, LED_rojo_NUM, 1);
    Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 0);
    Counter_1_Disable();
    
}


int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //Inicialización de las comunicaciones UART
    UART_1_Start();
    
    
    // Configurar las interrupciones del boton
    Cy_SysInt_Init(&Boton_int_cfg, Boton_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Boton_int_cfg.intrSrc);
    // Habilitar las interrupciones del boton
    NVIC_EnableIRQ(Boton_int_cfg.intrSrc);
    
    
    // Configurar las interrupciones del temporizador
    Cy_SysInt_Init(&Timer_int_cfg, Timer_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Timer_int_cfg.intrSrc);
    // Habilitar las interrupciones del boton
    NVIC_EnableIRQ(Timer_int_cfg.intrSrc);
    

    for(;;)
    {
        /* Place your application code here. */
        
        // No hay código, todo el programa funciona por interrupciones
        Cy_SysLib_Delay(10000); //Demuestra que no interfiere con la interrupción
    }
}

/* [] END OF FILE */
