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

cy_stc_scb_uart_context_t UART_1_context;

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //Inicialización de las comunicaciones UART
    UART_1_Start();
    
    // Iniciaización del temporizador
    Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_IMO, 3999999);
    Cy_SysTick_DisableInterrupt(); //Deshabilitar interrupciones del temporizador
    
    // Estado inicial de los LEDs
    Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 0);
    Cy_GPIO_Write(LED_rojo_PORT, LED_rojo_NUM, 0);

    for(;;)
    {
        /* Place your application code here. */
        
        //Modificar el estado de los LEDs cuando el temporizador llega a 0
        if (Cy_SysTick_GetCountFlag()){
            Cy_GPIO_Inv(LED_verde_PORT, LED_verde_NUM);   
        }
        
        switch(Cy_SCB_UART_Transmit(UART_1_HW, "Hola\n", 5, &UART_1_context)){
            case CY_SCB_UART_SUCCESS:
                break;
            case CY_SCB_UART_RECEIVE_BUSY:
            case CY_SCB_UART_TRANSMIT_BUSY:
                break;
            default:
                break;
        }
        
        //Cy_SCB_UART_PutString(UART_1_HW, "Hola\n");
    }
}

/* [] END OF FILE */
