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

#define ENC_TIMES_NUM 10
#define ENC_CLK 1000000 //Velocidad de reloj del contador del encoder en Hz
#define PULSES 500 //Pulsos por revolución de motor


// Ajustes del encoder
#define MY_TCPWM_CNT_NUM   (0UL)
#define MY_TCPWM_CNT_MASK  (1UL << MY_TCPWM_CNT_NUM)

volatile int numero_ciclos;
volatile int encoder_times [ENC_TIMES_NUM];
volatile int enc_index = 0;
volatile int speed = 0;
volatile long unsigned int cuentas = 0; //Debug

// Rutina de atención a la interrupción del temporizador (ISR Counter 1)
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





int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //Inicialización de las comunicaciones UART
    UART_1_Start();
    Cy_SCB_UART_PutString(UART_1_HW, "Programa iniciado ahora\n");
    
    //Inicialización del encoder
    Cy_TCPWM_QuadDec_Init(Encoder_HW, MY_TCPWM_CNT_NUM, &Encoder_config);
    Cy_TCPWM_QuadDec_Enable(Encoder_HW, MY_TCPWM_CNT_NUM);
    Cy_TCPWM_Enable_Multiple(Encoder_HW ,MY_TCPWM_CNT_MASK);
    Encoder_Start();
    Cy_TCPWM_QuadDec_SetCounter(Encoder_HW, MY_TCPWM_CNT_NUM, 0);
    Cy_TCPWM_TriggerStart(TCPWM0, MY_TCPWM_CNT_MASK);
    
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
    
    
    // Crear un buffer limpio para COM UART
    char buffer [20];
    
    for (int i = 0; i < 20; i++)
        buffer[i] = ' ';
    
    for(;;)
    {
        /* Place your application code here. */
        
        Cy_SysLib_Delay(1000); //Demuestra que no interfiere con la interrupción
        Cy_GPIO_Inv(LED_verde_PORT, LED_verde_NUM);
        
        Cy_TCPWM_TriggerStopOrKill(TCPWM0, MY_TCPWM_CNT_MASK);
        Cy_TCPWM_QuadDec_SetCounter(TCPWM0, MY_TCPWM_CNT_NUM, 0);
        Cy_TCPWM_TriggerStart(TCPWM0, MY_TCPWM_CNT_MASK);
        Cy_TCPWM_TriggerReloadOrIndex(Encoder_HW, MY_TCPWM_CNT_MASK);
        int32_t speed2 = Cy_TCPWM_QuadDec_GetCounter(Encoder_HW, MY_TCPWM_CNT_NUM);
        Cy_SysLib_Delay(500);
        Cy_TCPWM_TriggerReloadOrIndex(Encoder_HW, MY_TCPWM_CNT_MASK);
        int32_t speed3 = Cy_TCPWM_QuadDec_GetCounter(Encoder_HW, MY_TCPWM_CNT_NUM);
       
        
        
        //uint32_t status = Cy_SysLib_EnterCriticalSection();
        snprintf(buffer, sizeof(buffer), "%ld rpm\n", (long int)(speed3 - speed2));
        Cy_SCB_UART_PutString(UART_1_HW, buffer);
        
        if (0UL != (CY_TCPWM_QUADDEC_STATUS_COUNTER_RUNNING & 
                Cy_TCPWM_QuadDec_GetStatus(TCPWM0, MY_TCPWM_CNT_NUM)))
        {
            /* The Counter is running */
            Cy_SCB_UART_PutString(UART_1_HW, "OK\n");
            
        }
        //Cy_SysLib_ExitCriticalSection(status);
    }
}

/* [] END OF FILE */
