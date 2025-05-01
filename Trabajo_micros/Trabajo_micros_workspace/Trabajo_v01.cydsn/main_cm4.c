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
#define TOLERANCIA_LLEGADA 5 //en mm

// Ajustes de velocidades (en % sobre velocidad max, que son 40rpm)
#define VELOCIDAD_CONSIGNA 50
#define VELOCIDAD_CONSIGNA_AJUSTE 25
// A que distancia del piso comienza a frenar
#define COMIENZO_FRENADA 50

//Ajustes de temporización de puertas
#define ABRIR_PUERTA 1000
#define PUERTA_ABIERTA 2000
#define CERRAR_PUERTA 1000


// Deficiones pantalla LCD
#define LCD_I2C_ADDR 0x27  // Dirección I2C del módulo
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_CMD 0
#define LCD_DATA 1


//Variables encoder
volatile long signed int cuentas = 0;
volatile long signed int cuentas1 = 0;
volatile double velocidad = 0;
volatile double posicion_abs = 0;
uint8_t planta = 0;

volatile _Bool flag_sentido = false; //Subir

//Aux calculo velocidad
volatile uint last_tick = 0;
volatile uint32_t temporizador2 = 0;

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
#define DEBOUNCER_TIME 200


// Maquina de estados
uint8_t estado = 0;

_Bool flanco_0 = false; //Para primer ciclo de ejecución tras cambio de estado
_Bool flanco_1 = false;
_Bool flanco_2 = false;
_Bool flanco_3 = false;
_Bool flanco_4 = false;
_Bool flanco_5 = false;
_Bool flanco_6 = false;
_Bool flanco_7 = false;
_Bool flanco_8 = false;
_Bool flanco_9 = false;
_Bool flanco_10 = false;
_Bool flanco_11 = false;
_Bool flanco_12 = false;


//Cota de los pisos (en mm)
double piso0 = 0;
double piso1 = 115;
double piso2 = 230;
double piso3 = 445;
double piso4 = 560;


volatile _Bool flag_periodic_main = false;
volatile uint32_t temporizador = 0;


//Regulador PI
double const kp = 0.1;
double const ki = 0.0005;
//volatile double error = 0;
volatile double error_acumulado = 0;
double const salida_max = 100;
double const salida_min = 0;

/********** DECLARACIONES DE FUNCIONES DE USUARIO *************/

//Pantalla LCD
void LCD_SendNibble(uint8 nibble, uint8 mode);
void LCD_SendByte(uint8 byte, uint8 mode);
void LCD_SendCommand(uint8 cmd);
void LCD_SendChar(char data);
void LCD_Init();
void LCD_SetCursor(uint8 row, uint8 col);
void LCD_Print(char *str);
void LCD_PulseEnable(uint8 data);
void LCD_WriteI2C(uint8 data);
//I2C
void Scan_I2C_Bus(void);

/**********TRATAMIENTOS DE INTERRUPCIÓN *************/

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
    
    Cy_GPIO_ClearInterrupt(Encoder_ChA_PORT, Encoder_ChA_NUM);
    
    cuentas++;
    
    //Aquí contar el tiempo transcurrido para obtener la velocidad
    //Convertir el tiempo en velocidad sabiendo que entre dos pulsos se da una décima parte de vuelta
    //Se sabe que la velocidad definida como 100% aproximadamente son 40rpm
    //Cy_SCB_UART_PutString(UART_1_HW, "**Debug ENC**\n");
    
    double rpm = (60.0 / ((double)Counter_5_GetCounter()/1000.0)) * (10.0 / 150.0);
    velocidad = 2.5*rpm; //Pasar a porcentaje siendo 100% 40rpm
    if (velocidad < 0)
        velocidad = 0;
    else if (velocidad > 150)
        velocidad = 150;
    
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%2.2f\n", velocidad);
    Cy_SCB_UART_PutString(UART_1_HW, buffer);
    Counter_5_SetCounter(0);
    Counter_5_Start();
    
    
    if (!flag_sentido){
    //if (Cy_GPIO_Read(Encoder_ChB_PORT, Encoder_ChB_NUM)){
        posicion_abs = posicion_abs + 0.0942;
    }
    else{
        posicion_abs = posicion_abs - 0.0942;
    }
 
}

void Temporizacion_int_IRQHandler(void){
    
    Counter_4_ClearInterrupt(Counter_4_config.interruptSources);
    temporizador++;
    
    if (temporizador > 10000)
        temporizador = 0;
}

void Temporizacion2_int_IRQHandler(void){
    
    Counter_5_ClearInterrupt(Counter_5_config.interruptSources);
    temporizador2++;
    
    if (temporizador2 > 10000)
        temporizador2 = 0;
}

void SW0_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW0_PORT, SW0_NUM);
    if (Debouncer_CLK_GetCounter() - last_sw0 > DEBOUNCER_TIME){
        last_sw0 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        //Cy_SCB_UART_PutString(UART_1_HW, "Debug - P0\n");
        flag_sw0 = true;
    }
    
}

void SW1_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW1_PORT, SW1_NUM);
    if (Debouncer_CLK_GetCounter()  - last_sw1 > DEBOUNCER_TIME){
        last_sw1 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        //Cy_SCB_UART_PutString(UART_1_HW, "Debug - P1\n");
        flag_sw1 = true;
    }
    
}

void SW2_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW2_PORT, SW2_NUM);
    if (Debouncer_CLK_GetCounter()  - last_sw2 > DEBOUNCER_TIME){
        last_sw2 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        //Cy_SCB_UART_PutString(UART_1_HW, "Debug - P2\n");
        flag_sw2 = true;
    }
    
}

void SW3_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW3_PORT, SW3_NUM);
    if (Debouncer_CLK_GetCounter()  - last_sw3 > DEBOUNCER_TIME){
        last_sw3 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        //Cy_SCB_UART_PutString(UART_1_HW, "Debug - P3\n");
        flag_sw3 = true;
    }
    
}

void SW4_IRQHandler(void){
    Cy_GPIO_ClearInterrupt(SW4_PORT, SW4_NUM);
    if (Debouncer_CLK_GetCounter()  - last_sw4 > DEBOUNCER_TIME){
        last_sw4 = Debouncer_CLK_GetCounter();
        //Código a ejecutar cuando se pulsa el boton
        //Cy_SCB_UART_PutString(UART_1_HW, "Debug - P4\n");
        flag_sw4 = true;
    }
    
}

void Debouncer_ovrflw_int_IRQHandler(void){
    /*
    Para cuando el contador se de la vuelta, que no deje de funcionar
    el debouncer. Se mantienen aquellas diferencias que fuesen menores
    al tiempo fijado para el debouncer.
    Esta interrupción se ejecuta solo cada varias semanas, por lo que no
    es un gran problema que sea un poco lenta.
    */
    Debouncer_CLK_ClearInterrupt(Debouncer_CLK_config.interruptSources);
    //Cy_SCB_UART_PutString(UART_1_HW, "Deboug ovrflw\n");
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

void Periodic_main_IRQHandler(void){
    Counter_3_ClearInterrupt(Counter_3_config.interruptSources);
    flag_periodic_main = true;
}

void Vel_int_IRQHandler(void){
    Counter_5_ClearInterrupt(Counter_5_config.interruptSources);
    velocidad = 0;
}



/******* FUNCIONES DE USUARIO **********/
void Motor_subir(){
    Cy_GPIO_Write(Rele_1_PORT, Rele_1_NUM, 1);
    Cy_SysLib_Delay(500); //Es bloqueante intencionadamente - sistema parado por seguridad
    Cy_GPIO_Write(Rele_2_PORT, Rele_2_NUM, 1);
    flag_sentido = false;
}

void Motor_bajar(){
    Cy_GPIO_Write(Rele_1_PORT, Rele_1_NUM, 0);
    Cy_SysLib_Delay(500); //Es bloqueante intencionadamente - sistema parado por seguridad
    Cy_GPIO_Write(Rele_2_PORT, Rele_2_NUM, 0);
    flag_sentido = true;
}

void Motor_parado(){
    if (Cy_GPIO_ReadOut(Rele_1_PORT, Rele_1_NUM) == Cy_GPIO_ReadOut(Rele_2_PORT, Rele_2_NUM)){
        Cy_GPIO_Write(Rele_1_PORT, Rele_1_NUM, 0);
        Cy_SysLib_Delay(200); //Es bloqueante intencionadamente - sistema parado por seguridad
        Cy_GPIO_Write(Rele_2_PORT, Rele_2_NUM, 1);
    }
    // Añadir algo de código para garantizar parada. Forzar el estado que lea en los pines.
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
    //Valores de velocidad de 0 a 100
    //Originalmente de 0 a 400 pero se ha convertido a %
    Motor_setPeriodo(900 - velocidad*4);
}


//Borra todas las peticiones que se hayan realizado con los pulsadores
void clearAllPetitions(void){
    flag_sw0 = false;
    flag_sw1 = false;
    flag_sw2 = false;
    flag_sw3 = false;
    flag_sw4 = false;
}


int pos2planta(uint posicion){
    if (posicion > piso4 - 20)
        return 4;
    else if(posicion > piso3 - 20)
        return 3;
    else if(posicion > piso2 - 20)
        return 2;
    else if(posicion > piso1 - 20)
        return 1;
    else
        return 0;
}

uint calcular_controlador_PI(uint velocidad_deseada, uint velocidad_actual) {
    // Calcular el error
    
    static int cuenta = 0;
    
    int error = 0;
    double salida_controlador = 0;
    
    error = velocidad_deseada - velocidad_actual;

    // Acumular el error para la parte integral
    error_acumulado += error;

    // Salida del controlador PI
    salida_controlador = (kp * error) + (ki * error_acumulado);
    
    
    char buffer [50];
    cuenta ++;
    if (cuenta > 100){
        snprintf(buffer, sizeof(buffer), "** %i - %i\n", (int)error, (int)salida_controlador);
        Cy_SCB_UART_PutString(UART_1_HW, buffer);
        cuenta = 0;
    }

    // Saturar la salida para que esté dentro de los límites
    if (salida_controlador > salida_max) {
        salida_controlador = salida_max;
    } else if (salida_controlador < salida_min) {
        salida_controlador = salida_min;
    }
    
    return (uint)salida_controlador;
}

/********* DEPURACION PUERTO SERIE ********/

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
                //Cy_SCB_UART_PutString(UART_1_HW, "Aumentado periodo\n");
            }
            else if (strncmp(buffer_rx, "-", 1) == 0)
            {
                Cy_TCPWM_Counter_SetPeriod(Counter_1_HW, Counter_1_CNT_NUM, Cy_TCPWM_Counter_GetPeriod(Counter_1_HW, Counter_1_CNT_NUM)-20);
                Cy_TCPWM_Counter_SetPeriod(Counter_2_HW, Counter_2_CNT_NUM, Cy_TCPWM_Counter_GetPeriod(Counter_1_HW, Counter_1_CNT_NUM)+1);
                //Cy_SCB_UART_PutString(UART_1_HW, "Recudido periodo\n");
            }
            else if (strncmp(buffer_rx, "c", 1) == 0){
                if (flag_sentido)
                    Motor_subir();
                else
                    Motor_bajar();
            }
            else if (strncmp(buffer_rx, "s", 1) == 0){
                Motor_subir();
            }
            else if (strncmp(buffer_rx, "b", 1) == 0){
                Motor_bajar();
            }
            else if (strncmp(buffer_rx, "0", 1) == 0)
                flag_sw0 = true;
            else if (strncmp(buffer_rx, "1", 1) == 0)
                flag_sw1 = true;
            else if (strncmp(buffer_rx, "2", 1) == 0)
                flag_sw2 = true;
            else if (strncmp(buffer_rx, "3", 1) == 0)
                flag_sw3 = true;
            else if (strncmp(buffer_rx, "4", 1) == 0)
                flag_sw4 = true;
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


/*************** MAIN **************************/


int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    //Inicialización de las comunicaciones UART
    
    UART_1_Start();
    //Cy_SCB_UART_PutString(UART_1_HW, "Programa iniciado ahora\n");
    
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
    
    
    // Configurar las interrupciones del temporizador 4 (temporizador)
    Cy_SysInt_Init(&Temporizacion_int_cfg, Temporizacion_int_IRQHandler);
    NVIC_ClearPendingIRQ(Temporizacion_int_cfg.intrSrc);
    NVIC_EnableIRQ(Temporizacion_int_cfg.intrSrc);
    
    
    // Configurar las interrupciones del temporizador 6 (periodic main)
    // (Es para tareas que se ejecutan solo cada x tiempo en main)
    Cy_SysInt_Init(&Periodic_main_int_cfg, Periodic_main_IRQHandler);
    NVIC_ClearPendingIRQ(Periodic_main_int_cfg.intrSrc);
    NVIC_EnableIRQ(Periodic_main_int_cfg.intrSrc);
    
    
    // Configurar las interrupciones de overflow del contador para medida de velocidad
    Cy_SysInt_Init(&Vel_int_cfg, Vel_int_IRQHandler);
    NVIC_ClearPendingIRQ(Vel_int_cfg.intrSrc);
    NVIC_EnableIRQ(Vel_int_cfg.intrSrc);
    
    //Inicializar contadores
    Counter_1_Start();
    Counter_2_Start();
    Counter_1_Disable(); //Solo necesario en ciertos casos
    Counter_2_Disable(); //Solo necesario en ciertos casos
    Debouncer_CLK_Start();
    Counter_3_Start();
    Counter_4_Start();
    Counter_4_Disable();
    Counter_5_Start();
    
    
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
    
    //Arranque de la pantalla y de las comunicaciones I2C
    I2C_1_Start();
    //Cy_SCB_UART_PutString(UART_1_HW, "Escaner completado\n");
    LCD_Init();

    LCD_SetCursor(0, 0);
    LCD_Print("#TEST#");
    
    // Crear un buffer limpio para COM UART
    char buffer [50];
    
    for (int i = 0; i < 20; i++)
        buffer[i] = ' ';
    
    //Variables locales
    uint8_t siguiente_estado = 0;
    double destino = 0;
    uint16_t velocidad_consigna_rampa = 0;
    uint16_t contador_rampa = 0;
        
    //Inicialización máquina de estados
    estado = 0;
    flanco_0 = true;    
        
    for(;;)
    {
        /* Place your application code here. */
        
        if (flag_periodic_main){
            flag_periodic_main = false;
            planta = pos2planta(posicion_abs);
            LCD_SetCursor(1, 0);
            snprintf(buffer, sizeof(buffer), "%i;%i\n", (int)estado, (int)planta);
            //Cy_SCB_UART_PutString(UART_1_HW, buffer);
            snprintf(buffer, sizeof(buffer), "Planta %i", (int)planta);
            LCD_Print(buffer);
        }
        // TRANSICIÓN DE ESTADOS
        switch (estado){
            case 0: //Reposo total
                //Transición a estado 1
                if (flag_sw1){
                    siguiente_estado = 1;
                    flanco_1 = true;
                    flag_sw1 = false;
                }
                //Transición a estado 0
                else if(flag_sw0){
                    siguiente_estado = 2;
                    flanco_2 = true;
                    flag_sw0 = false;
                    posicion_abs = 0; //Accion excepcional ("maquina de Mealey")
                }
                else
                    siguiente_estado = 0;
            break;
            case 1: //Bajar hasta "home"
                //Transición a estado 0
                if (flag_sw1){
                    siguiente_estado = 0;
                    flanco_0 = true;
                    flag_sw1 = false;
                }
                //Transición a estado 2
                else if(flag_sw0){
                    siguiente_estado = 2;
                    flanco_2 = true;
                    flag_sw0 = false;
                    posicion_abs = 0; //Accion excepcional ("maquina de Mealey")
                }
                else
                    siguiente_estado = 1;
            
            break;
            case 2: //Reposo en operación
                //Transición a estado 3
                if (flag_sw0 || flag_sw1 || flag_sw2 || flag_sw3 || flag_sw4){
                    siguiente_estado = 3;
                    flanco_3 = true;
                    if(flag_sw0)
                        destino = piso0;
                    else if(flag_sw1)
                        destino = piso1;
                    else if (flag_sw2)
                        destino = piso2;
                    else if (flag_sw3)
                        destino = piso3;
                    else
                        destino = piso4;
                    flag_sw0 = false;
                    flag_sw1 = false;
                    flag_sw2 = false;
                    flag_sw3 = false;
                    flag_sw4 = false;
                    //snprintf(buffer, sizeof(buffer), "Destino: %2.2f\n", destino);
                    //Cy_SCB_UART_PutString(UART_1_HW, buffer);
                }
                else
                    siguiente_estado = 2;
            
            break;
            case 3: //Solicitado
                //Transición a estado 4 (rampa aceleración subir)
                if (pos2planta(destino) > planta){
                    siguiente_estado = 4;
                    flanco_4 = true;
                }
                //Transición a estado 5 (rampa aceleración bajar)
                else if (pos2planta(destino) < planta){
                    siguiente_estado = 7;
                    flanco_7 = true;
                }
                //Transición a estado 2 (se ha solicitado desde el mismo piso, volver a reposo)
                else{
                    siguiente_estado = 2;
                    flanco_2 = true;
                }
            
            break;
            case 4: //Subir - rampa de aceleración
                if (velocidad_consigna_rampa == VELOCIDAD_CONSIGNA){
                    siguiente_estado = 5;
                    flanco_5 = true;
                }
            
            break;
            case 5: //Subir
                if (destino - posicion_abs < COMIENZO_FRENADA){
                    siguiente_estado = 6;
                    flanco_6 = true;                    
                }
            break;
            case 6: //Subir - rampa de frenado
                if (destino - posicion_abs < TOLERANCIA_LLEGADA || posicion_abs > destino){
                    siguiente_estado = 10;
                    flanco_10 = true;
                }
            break;
            case 7: //Bajar - rampa de aceleración
                if (velocidad_consigna_rampa == VELOCIDAD_CONSIGNA){
                    siguiente_estado = 8;
                    flanco_8 = true;
                }
            break;
            case 8: //Bajar
                if (posicion_abs - destino < COMIENZO_FRENADA){
                    siguiente_estado = 9;
                    flanco_9 = true;
                }
            
            break;
            case 9: //Bajar - rampa de frenado
                if (posicion_abs - destino < TOLERANCIA_LLEGADA || posicion_abs < destino){
                    siguiente_estado = 10;
                    flanco_10 = true;
                }
            break;
            case 10: //Abrir puerta
                if (temporizador >= ABRIR_PUERTA){
                    siguiente_estado = 11;
                    flanco_11 = true;
                }
                
            break;
            case 11: //Puerta abierta (esperando)
                if (temporizador >= PUERTA_ABIERTA){
                    siguiente_estado = 12;
                    flanco_12 = true;
                }
            break;
            case 12: //Cerrando puerta
                if (temporizador >= CERRAR_PUERTA){
                    siguiente_estado = 2;
                    flanco_2 = true;
                }
            break;
        }
        
        estado = siguiente_estado;
        
        // ACCIONES ASOCIADAS A LOS ESTADOS
        switch (estado){
            case 0:
                if (flanco_0){
                    flanco_0 = false;
                    //Acciones primer ciclo de ejecución   
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 0\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("0-Reposo        ");
                    Motor_parado();
                    Motor_setVelocidad(0); //Por seguridad aunque es redundante
                }
            break;
            case 1:
                if (flanco_1){
                    flanco_1 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 1\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("1-Ajuste de 0   ");
                    Motor_setVelocidad(0);
                    Motor_bajar();
                    Motor_setVelocidad(VELOCIDAD_CONSIGNA);
                }
            
            break;
            case 2:
                if (flanco_2){
                    flanco_2 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 2\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("2-Reposo op.    ");
                    Motor_parado();
                    Motor_setVelocidad(0);
                    clearAllPetitions();
                    Counter_4_Disable();
                }
            
            break;
            case 3: //Solicitado
                if (flanco_3){
                    flanco_3 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 3\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("3-Solicitado    ");
                    Motor_parado();
                    Motor_setVelocidad(0);
                    clearAllPetitions();
                }
            
            break;
            case 4: //Rampa de aceleración subida
                if (flanco_4){
                    flanco_4 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 4\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("4-Subir ramp up ");
                    Motor_setVelocidad(0);
                    Motor_subir();
                    contador_rampa = 0;
                    velocidad_consigna_rampa = VELOCIDAD_CONSIGNA_AJUSTE;
                }
                contador_rampa++;
                if (contador_rampa == 2000){
                    contador_rampa = 0;
                    velocidad_consigna_rampa++;
                }
                Motor_setVelocidad(velocidad_consigna_rampa);
            
            break;
            case 5: //Subir
                if (flanco_5){
                    flanco_5 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 5\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("5-Subiendo      ");
                    Motor_setVelocidad(VELOCIDAD_CONSIGNA);
                    error_acumulado = 0; //Reset del controlador PI
                }
                
                //CONTROL DE VELOCIDAD CON REGULADOR PI
                //snprintf(buffer, sizeof(buffer), "%i\n", calcular_controlador_PI(VELOCIDAD_CONSIGNA, (uint)velocidad));
                //Cy_SCB_UART_PutString(UART_1_HW, buffer);
                Motor_setVelocidad(calcular_controlador_PI(VELOCIDAD_CONSIGNA, (uint)velocidad));
                Cy_SysLib_Delay(1);
            
            break;
            case 6: //Rampa de frenada en subida y ajuste de llegada
                if (flanco_6){
                    flanco_6 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 6\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("6-Subir ramp dow");
                    contador_rampa = 0;
                    velocidad_consigna_rampa = VELOCIDAD_CONSIGNA;
                }
                contador_rampa++;
                if (contador_rampa == 1000 && velocidad_consigna_rampa > VELOCIDAD_CONSIGNA_AJUSTE){
                    velocidad_consigna_rampa--;
                    contador_rampa = 0;
                }
                Motor_setVelocidad(velocidad_consigna_rampa);
            
            break;
            case 7: //Rampa aceleración bajada
                if (flanco_7){
                    flanco_7 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 7\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("7-Bajar ramp up ");
                    Motor_setVelocidad(0);
                    Motor_bajar();
                    contador_rampa = 0;
                    velocidad_consigna_rampa = VELOCIDAD_CONSIGNA_AJUSTE;
                }
                contador_rampa++;
                if (contador_rampa == 2000){
                    contador_rampa = 0;
                    velocidad_consigna_rampa++;
                }
                Motor_setVelocidad(velocidad_consigna_rampa);
            
            break;
            case 8: //Bajar
                if (flanco_8){
                    flanco_8 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 8\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("8-Bajando       ");
                    Motor_setVelocidad(VELOCIDAD_CONSIGNA);
                    
                }
                Motor_setVelocidad(calcular_controlador_PI(VELOCIDAD_CONSIGNA, (uint)velocidad));
                Cy_SysLib_Delay(1);
            
            break;
            case 9: //Rampa frenada bajada
                if (flanco_9){
                    flanco_9 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 9\n");
                    LCD_SetCursor(0, 0);
                    LCD_Print("9-Bajar ramp dow");
                    contador_rampa = 0;
                    velocidad_consigna_rampa = VELOCIDAD_CONSIGNA;
                }
                contador_rampa++;
                if (contador_rampa == 1000 && velocidad_consigna_rampa > VELOCIDAD_CONSIGNA_AJUSTE){
                    Motor_setVelocidad(velocidad_consigna_rampa--);
                    contador_rampa = 0;
                }
                Motor_setVelocidad(velocidad_consigna_rampa);
                
            
            break;
            case 10:
                if (flanco_10){
                    flanco_10 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 10\n");
                    Motor_parado();
                    Motor_setVelocidad(0);
                    Counter_4_Enable();
                    Counter_4_SetCounter(0);
                    Counter_4_Start();
                    temporizador = 0;
                    LCD_SetCursor(0, 0);
                    LCD_Print("10-Abriendo...  ");
                    
                }
            
            break;
            case 11:
                if (flanco_11){
                    flanco_11 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 11\n");
                    temporizador = 0;
                    LCD_SetCursor(0, 0);
                    LCD_Print("11-Puerta open  ");
                    
                }
            
            break;
            case 12:
                if (flanco_12){
                    flanco_12 = false;
                    //Acciones primer ciclo de ejecución
                    //Cy_SCB_UART_PutString(UART_1_HW, "Debug - Estado 12\n");
                    temporizador = 0;
                    LCD_SetCursor(0, 0);
                    LCD_Print("12-Cerrando...  ");
                    
                }
            
            break;
        }
        
    }
}

/******* IMPLEMENTAACIÓN FUNCIONES DE USUARIO *************/


// Funciones de la pantalla LCD
void LCD_WriteI2C(uint8 data) {
    Cy_SCB_I2C_MasterSendStart(I2C_1_HW, LCD_I2C_ADDR, CY_SCB_I2C_WRITE_XFER, 1000, &I2C_1_context);
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW, data | LCD_BACKLIGHT, 1000, &I2C_1_context);
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW, 1000, &I2C_1_context);
    //CyDelayUs(50);
}

void LCD_PulseEnable(uint8 data) {
    LCD_WriteI2C(data | LCD_ENABLE);
    //CyDelayUs(1);
    LCD_WriteI2C(data & ~LCD_ENABLE);
    //CyDelayUs(50);
}

void LCD_SendNibble(uint8 nibble, uint8 mode) {
    uint8 data = (nibble & 0xF0) | mode | LCD_BACKLIGHT;
    LCD_WriteI2C(data);
    LCD_PulseEnable(data);
}

void LCD_SendByte(uint8 byte, uint8 mode) {
    LCD_SendNibble(byte & 0xF0, mode);
    LCD_SendNibble((byte << 4) & 0xF0, mode);
}

void LCD_SendCommand(uint8 cmd) {
    LCD_SendByte(cmd, LCD_CMD);
}

void LCD_SendChar(char data) {
    LCD_SendByte(data, LCD_DATA);
}

void LCD_Init() {
    //CyDelay(50);
    LCD_SendNibble(0x30, LCD_CMD);
    //CyDelay(5);
    LCD_SendNibble(0x30, LCD_CMD);
    //CyDelayUs(100);
    LCD_SendNibble(0x30, LCD_CMD);
    //CyDelayUs(100);

    LCD_SendNibble(0x20, LCD_CMD);  // 4-bit mode
    LCD_SendCommand(0x28); // 2 lines, 5x8 dots
    LCD_SendCommand(0x0C); // Display on, cursor off
    LCD_SendCommand(0x06); // Entry mode
    LCD_SendCommand(0x01); // Clear display
    //CyDelay(2);
}

void LCD_SetCursor(uint8 row, uint8 col) {
    uint8 row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    LCD_SendCommand(0x80 | (col + row_offsets[row]));
}

void LCD_Print(char *str) {
    while (*str) {
        LCD_SendChar(*str++);
    }
}

//Escaneo I2C - Debug
void Scan_I2C_Bus(void)
{
    uint8_t address;
    cy_en_scb_i2c_status_t result;

    for (address = 0x03; address <= 0x77; address++)
    {
        // Inicia transmisión I2C con dirección
        result = Cy_SCB_I2C_MasterSendStart(I2C_1_HW, address, CY_SCB_I2C_WRITE_XFER, 1000, &I2C_1_context);
        char buffer2 [50];
        if (result == CY_SCB_I2C_SUCCESS)
        {
            
            // Dirección encontrada
            snprintf(buffer2, sizeof(buffer2), "Dispositivo encontrado en 0x%02X\n", address);
            Cy_SCB_UART_PutString(UART_1_HW, buffer2);
            // Finaliza la transmisión correctamente
            Cy_SCB_I2C_MasterSendStop(I2C_1_HW, 1000, &I2C_1_context);
        }
        else
        {
            // Stop por si hubo error
            Cy_SCB_I2C_MasterSendStop(I2C_1_HW, 1000, &I2C_1_context);
            snprintf(buffer2, sizeof(buffer2), "Dispositivo NO encontrado en 0x%02X\n", address);
            Cy_SCB_UART_PutString(UART_1_HW, buffer2);
        }
        CyDelay(10); // Pequeño retardo para estabilidad
    }
}

/* [] END OF FILE */
