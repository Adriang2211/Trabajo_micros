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
void SysInt_1_IRQHandler(void){
    //Borrar interrupción
    Counter_1_ClearInterrupt(Counter_1_config.interruptSources);
    
    Cy_GPIO_Inv(LED_rojo_PORT, LED_rojo_NUM);
    Cy_GPIO_Inv(LED_verde_PORT, LED_verde_NUM);
}


int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //Inicialización de las comunicaciones UART
    UART_1_Start();
    
    // Configurar las interrupciones del temporizador
    Cy_SysInt_Init(&SysInt_1_cfg, SysInt_1_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(SysInt_1_cfg.intrSrc);
    // Habilitar las interrupciones del contador
    NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    
    Counter_1_Start(); //Iniciar el contador

    for(;;)
    {
        /* Place your application code here. */
        
        // No hay código, todo el programa funciona por interrupciones
        Cy_SysLib_Delay(10000); //Demuestra que no interfiere con la interrupción
    }
}

/* [] END OF FILE */
