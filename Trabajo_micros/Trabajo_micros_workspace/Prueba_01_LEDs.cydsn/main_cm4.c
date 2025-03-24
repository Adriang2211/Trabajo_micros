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

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    UART_1_Start();

    for(;;)
    {
        /* Place your application code here. */
        Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 1);
        Cy_SysLib_Delay(200);
        Cy_GPIO_Write(LED_verde_PORT, LED_verde_NUM, 0);
        Cy_SysLib_Delay(200);
        
        Cy_SCB_UART_PutString(UART_1_HW, "Hola\n");
    }
}

/* [] END OF FILE */
