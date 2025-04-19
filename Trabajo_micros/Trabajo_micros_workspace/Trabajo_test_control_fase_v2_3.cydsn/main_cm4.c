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

volatile long unsigned int cuentas = 0; //Debug
volatile long unsigned int cuentas1 = 0; //Debug


//Variables para funcionamiento de RX UART
uint32_t rx_data;
uint32_t data_received;
uint8_t error_uart_rx;

#define BUFFER_RX_UART 8
char buffer_rx [BUFFER_RX_UART];
uint16_t buffer_index = 0;



volatile _Bool flag_subir = false; //Bajando


/******** DECLARACIÓN FUNCIONES DE USUARIO ******************/

void Motor_subir();
void Motor_bajar();
void Motor_parado();
void Motor_setPeriodo(uint periodo);
void Motor_setVelocidad(uint velocidad);

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
                if (flag_subir)
                    Motor_bajar();
                else
                    Motor_subir();
            }
            else if (strncmp(buffer_rx, "s", 1) == 0){
                Motor_subir();
            }
            else if (strncmp(buffer_rx, "b", 1) == 0){
                Motor_bajar();
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


/******* FUNCIONES DE USUARIO **********/
void Motor_subir(){
    Cy_GPIO_Write(Rele_1_PORT, Rele_1_NUM, 1);
    Cy_SysLib_Delay(500); //Es bloqueante intencionadamente - sistema parado por seguridad
    Cy_GPIO_Write(Rele_2_PORT, Rele_2_NUM, 1);
    flag_subir = true;
}

void Motor_bajar(){
    Cy_GPIO_Write(Rele_1_PORT, Rele_1_NUM, 0);
    Cy_SysLib_Delay(500); //Es bloqueante intencionadamente - sistema parado por seguridad
    Cy_GPIO_Write(Rele_2_PORT, Rele_2_NUM, 0);
    flag_subir = false;
}

void Motor_parado(){
    if (Cy_GPIO_ReadOut(Rele_1_PORT, Rele_1_NUM) == Cy_GPIO_ReadOut(Rele_2_PORT, Rele_2_NUM)){
        Cy_GPIO_Write(Rele_1_PORT, Rele_1_NUM, 0);
        Cy_SysLib_Delay(200); //Es bloqueante intencionadamente - sistema parado por seguridad
        Cy_GPIO_Write(Rele_2_PORT, Rele_2_NUM, 1);
    }
}

void Motor_setPeriodo(uint periodo){
    if (periodo < 500){
        Counter_1_SetPeriod(500);
        Counter_2_SetPeriod(501);
    }
    else if(periodo > 900){
        Counter_1_SetPeriod(899);
        Counter_2_SetPeriod(900);
    }
    else{
        Counter_1_SetPeriod(periodo);
        Counter_2_SetPeriod(periodo+1);
    }
}

void Motor_setVelocidad(uint velocidad){
    //Valores de velocidad de 0 a 400
    Motor_setPeriodo(900 - velocidad);
}

/* [] END OF FILE */
