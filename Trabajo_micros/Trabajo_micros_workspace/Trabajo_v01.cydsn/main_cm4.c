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
#include <string.h>
#include <stdlib.h>

// Pulsos por rotación que proporciona el encoder
#define PULSES_PER_ROTATION 10
// Relación de reducción de la reductora del motor
#define REL_REDUCTION 150

volatile long signed int cuentas = 0;
volatile long signed int cuentas1 = 0;
volatile double posicion_abs = 0;

volatile _Bool flag_sentido = false; //Subir


//Variables para funcionamiento de RX UART
uint32_t rx_data;
uint32_t data_received;
uint8_t error_uart_rx;

#define BUFFER_RX_UART 8
char buffer_rx [BUFFER_RX_UART];
uint16_t buffer_index = 0;


//Variables de los pulsadores
volatile _Bool flag_sw0 = false;
volatile _Bool flag_sw1 = false;
volatile _Bool flag_sw2 = false;
volatile _Bool flag_sw3 = false;
volatile _Bool flag_sw4 = false;
volatile uint32_t last_sw0 = 0;
volatile uint32_t last_sw1 = 0;
volatile uint32_t last_sw2 = 0;
volatile uint32_t last_sw3 = 0;
volatile uint32_t last_sw4 = 0;
#define DEBOUNCER_TIME 2000


void Opto_detec_IRQHandler(void){
    
    Cy_GPIO_ClearInterrupt(OPTO_PC817_PORT, OPTO_PC817_NUM);
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
    
    cuentas++;
    
    if (flag_sentido){
        posicion_abs = posicion_abs + 0.01;
    }
    else{
        posicion_abs = posicion_abs - 0.01;
    }
 
}

void ISR_UART(void)
{
    // Verificar si se ha recibido un dato en el buffer RX
    if ((UART_1_HW->INTR_RX_MASKED & SCB_INTR_RX_MASKED_NOT_EMPTY_Msk) != 0)
    {
        // Limpiar la interrupción
        Cy_SCB_ClearRxInterrupt(UART_1_HW, CY_SCB_RX_INTR_NOT_EMPTY);

        // Leer el carácter recibido
        char received_char = Cy_SCB_UART_Get(UART_1_HW);

        // Si es un Enter (fin de línea), procesamos el comando
        if (received_char == '\n' || received_char == '\r')
        {
            // Null-terminar la cadena para procesarla
            buffer_rx[buffer_index] = '\0';  // Finaliza el comando
            Cy_SCB_UART_PutString(UART_1_HW, buffer_rx);
            
            // Comprobar si el comando es "ON" o "OFF"
            if (strncmp(buffer_rx, "+", 1) == 0)
            {
                // Counter period ++
                Cy_TCPWM_Counter_SetPeriod(Counter_1_HW, Counter_1_CNT_NUM, Cy_TCPWM_Counter_GetPeriod(Counter_1_HW, Counter_1_CNT_NUM)+20);
                Cy_TCPWM_Counter_SetPeriod(Counter_2_HW, Counter_2_CNT_NUM, Cy_TCPWM_Counter_GetPeriod(Counter_1_HW, Counter_1_CNT_NUM)+1);
                Cy_SCB_UART_PutString(UART_1_HW, "Aumentado periodo\n");
            }
            else if (strncmp(buffer_rx, "-", 1) == 0)
            {
                Cy_TCPWM_Counter_SetPeriod(Counter_1_HW, Counter_1_CNT_NUM, Cy_TCPWM_Counter_GetPeriod(Counter_1_HW, Counter_1_CNT_NUM)-20);
                Cy_TCPWM_Counter_SetPeriod(Counter_2_HW, Counter_2_CNT_NUM, Cy_TCPWM_Counter_GetPeriod(Counter_1_HW, Counter_1_CNT_NUM)+1);
                Cy_SCB_UART_PutString(UART_1_HW, "Recudido periodo\n");
            }
            else if (strncmp(buffer_rx, "c", 1) == 0){
                Cy_GPIO_Inv(Rele_1_PORT, Rele_1_NUM);
                Cy_SysLib_Delay(500);
                Cy_GPIO_Inv(Rele_2_PORT, Rele_2_NUM);
                flag_sentido = !flag_sentido;
            }
            
            else
            {
                Cy_SCB_UART_PutString(UART_1_HW, "\r\nComando no valido\r\n");
            }

            // Resetear el índice del buffer y limpiarlo
            data_received = 1;
            buffer_index = 0;
            memset(buffer_rx, 0, BUFFER_RX_UART);
        }
        else if (buffer_index < BUFFER_RX_UART - 1)
        {
            // Si no es fin de línea, almacenar el carácter en el buffer
            buffer_rx[buffer_index++] = received_char;
        }
    }
    else
    {
        //Tratamiento de errores
    }
}

void SW0_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW0_PORT, SW0_NUM);
    if (Debouncer_CLK_GetCounter() - last_sw0 > DEBOUNCER_TIME){
        last_sw0 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        Cy_SCB_UART_PutString(UART_1_HW, "P0\n");
    }
    
}

void SW1_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW1_PORT, SW1_NUM);
    if (Debouncer_CLK_GetCounter()  - last_sw1 > DEBOUNCER_TIME){
        last_sw1 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        Cy_SCB_UART_PutString(UART_1_HW, "P1\n");
    }
    
}

void SW2_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW2_PORT, SW2_NUM);
    if (Debouncer_CLK_GetCounter()  - last_sw2 > DEBOUNCER_TIME){
        last_sw2 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        Cy_SCB_UART_PutString(UART_1_HW, "P2\n");
    }
    
}

void SW3_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW3_PORT, SW3_NUM);
    if (Debouncer_CLK_GetCounter()  - last_sw3 > DEBOUNCER_TIME){
        last_sw3 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        Cy_SCB_UART_PutString(UART_1_HW, "P3\n");
    }
    
}

void SW4_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW4_PORT, SW4_NUM);
    if (Debouncer_CLK_GetCounter()  - last_sw4 > DEBOUNCER_TIME){
        last_sw4 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        Cy_SCB_UART_PutString(UART_1_HW, "P4\n");
    }
    
}

void Debouncer_ovrflw_int_IRQHandler(void){
    /*
    Para cuando el contador se de la vuelta, que no deje de funcionar
    el debouncer. Se mantienen aquellas diferencias que fuesen menores
    al tiempo fijado para el debouncer
    */
    Debouncer_CLK_ClearInterrupt(Debouncer_CLK_config.interruptSources);
    Cy_SCB_UART_PutString(UART_1_HW, "Deboug ovrflw\n");
    uint32_t dif_sw0 = Debouncer_CLK_GetPeriod() - last_sw0; //Lo que llevaba contado
    uint32_t dif_sw1 = Debouncer_CLK_GetPeriod() - last_sw1;
    uint32_t dif_sw2 = Debouncer_CLK_GetPeriod() - last_sw2;
    uint32_t dif_sw3 = Debouncer_CLK_GetPeriod() - last_sw3;
    uint32_t dif_sw4 = Debouncer_CLK_GetPeriod() - last_sw4;
    Debouncer_CLK_SetCounter(DEBOUNCER_TIME);
    if (dif_sw0 < DEBOUNCER_TIME)
        last_sw0 = dif_sw0;
    else
        last_sw0 = 0;
    if (dif_sw1 < DEBOUNCER_TIME)
        last_sw1 = dif_sw0;
    else
        last_sw1 = 0;
    if (dif_sw2 < DEBOUNCER_TIME)
        last_sw2 = dif_sw0;
    else
        last_sw2 = 0;
    if (dif_sw3 < DEBOUNCER_TIME)
        last_sw3 = dif_sw0;
    else
        last_sw3 = 0;
    if (dif_sw4 < DEBOUNCER_TIME)
        last_sw4 = dif_sw0;
    else
        last_sw4 = 0;
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //Inicialización de las comunicaciones UART
    
    UART_1_Start();
    Cy_SCB_UART_PutString(UART_1_HW, "Programa iniciado ahora\n");
    
    // Configurar las interrupciones del UART (RX)
    Cy_SCB_UART_Init(UART_1_HW, &UART_1_config, &UART_1_context);
    Cy_SCB_UART_Enable(UART_1_HW);
    /* Unmasking only the RX fifo not empty interrupt bit */
    UART_1_HW->INTR_RX_MASK = SCB_INTR_RX_MASK_NOT_EMPTY_Msk;   
    Cy_SysInt_Init(&UART_1_SCB_IRQ_cfg, ISR_UART);
    NVIC_EnableIRQ(UART_1_SCB_IRQ_cfg.intrSrc);
    
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
    
    
    // Configurar las interrupciones del temporizador 3 (debouncer_clk)
    Cy_SysInt_Init(&Debouncer_ovrflw_int_cfg, Debouncer_ovrflw_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Debouncer_ovrflw_int_cfg.intrSrc);
    // Habilitar las interrupciones del boton
    NVIC_EnableIRQ(Debouncer_ovrflw_int_cfg.intrSrc);
    
    
    //Inicializar contadores
    Counter_1_Start();
    Counter_2_Start();
    Counter_1_Disable();
    Counter_2_Disable();
    Debouncer_CLK_Start();
    
    
    // Configurar las interrupciones del encoder
    Cy_SysInt_Init(&Encoder_int_cfg, Encoder_int_IRQHandler);
    // Limpiar interrupciones espúreas
    NVIC_ClearPendingIRQ(Encoder_int_cfg.intrSrc);
    // Habilitar las interrupciones del optoacoplador de detección
    NVIC_EnableIRQ(Encoder_int_cfg.intrSrc);
    
    
    //Inicializar el sentido de giro del sistema
    Cy_GPIO_Write(Rele_1_PORT, Rele_1_NUM, 1);
    Cy_GPIO_Write(Rele_2_PORT, Rele_2_NUM, 1);


    // Configurar las interrupciones del pulsador SW0
    Cy_SysInt_Init(&SW0_int_cfg, SW0_IRQHandler);
    NVIC_ClearPendingIRQ(SW0_int_cfg.intrSrc);
    NVIC_EnableIRQ(SW0_int_cfg.intrSrc);
    
    // Configurar las interrupciones del pulsador SW1
    Cy_SysInt_Init(&SW1_int_cfg, SW1_IRQHandler);
    NVIC_ClearPendingIRQ(SW1_int_cfg.intrSrc);
    NVIC_EnableIRQ(SW1_int_cfg.intrSrc);
    
    // Configurar las interrupciones del pulsador SW2
    Cy_SysInt_Init(&SW2_int_cfg, SW2_IRQHandler);
    NVIC_ClearPendingIRQ(SW2_int_cfg.intrSrc);
    NVIC_EnableIRQ(SW2_int_cfg.intrSrc);
    
    // Configurar las interrupciones del pulsador SW3
    Cy_SysInt_Init(&SW3_int_cfg, SW3_IRQHandler);
    NVIC_ClearPendingIRQ(SW3_int_cfg.intrSrc);
    NVIC_EnableIRQ(SW3_int_cfg.intrSrc);
    
    // Configurar las interrupciones del pulsador SW4
    Cy_SysInt_Init(&SW4_int_cfg, SW4_IRQHandler);
    NVIC_ClearPendingIRQ(SW4_int_cfg.intrSrc);
    NVIC_EnableIRQ(SW4_int_cfg.intrSrc);
    
    // Crear un buffer limpio para COM UART
    char buffer [50];
    
    for (int i = 0; i < 20; i++)
        buffer[i] = ' ';
    
    for(;;)
    {
        /* Place your application code here. */
        
        Cy_SysLib_Delay(1000); //Demuestra que no interfiere con la interrupción
        Cy_GPIO_Inv(LED_verde_PORT, LED_verde_NUM);
        int32_t velocidad = cuentas - cuentas1;
        if (!flag_sentido)
            velocidad = velocidad*(-1);
        snprintf(buffer, sizeof(buffer), "%ld rpm - Pos:%f \n", (long int)(velocidad)*60/PULSES_PER_ROTATION/REL_REDUCTION, posicion_abs);
        cuentas1 = cuentas;
        Cy_SCB_UART_PutString(UART_1_HW, buffer);
        
    }
}

/* [] END OF FILE */
