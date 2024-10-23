#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include <stdio.h>
//Sin punteros
/***************************
 * Definiciones
 **************************/
#define BOARD_GPIO    BOARD_INITPINS_PINES_GPIO // GPIOE


/* Led */
#define BOARD_GPIO_PIN_9 BOARD_INITPINS_PINES_9  // E2

// COLUMNAS(Entradas)

#define BOARD_GPIO_PIN_10  BOARD_INITPINS_PINES_10

#define BOARD_GPIO_PIN_11  BOARD_INITPINS_PINES_11

#define BOARD_GPIO_PIN_12  BOARD_INITPINS_PINES_12

#define BOARD_GPIO_PIN_14  BOARD_INITPINS_PINES_14

//Filas-(Salidas)

#define BOARD_GPIO_PIN_15  BOARD_INITPINS_PINES_15

#define BOARD_GPIO_PIN_16  BOARD_INITPINS_PINES_16

#define BOARD_GPIO_PIN_2   BOARD_INITPINS_PINES_2

#define BOARD_GPIO_PIN_6   BOARD_INITPINS_PINES_6

//Declaracion de variables
int col = 0, fil = 0;
int cont = 1;
int DutyCycle, Dmax, Ton, Toff;
int NUM, DEC;
int check_col1, check_col2, check_col3, check_col4;

//DECLARACION DE ESTADOS

enum Estados {INICIO, PWM, AJUSTEDC};


volatile uint32_t g_systickCounter;

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

//Esta es la funcion del PWM
void ActivarPWM(void)
{
    GPIO_PortClear(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9);
    SysTick_DelayTicks((Dmax * DutyCycle) / 100); // Tiempo encendido
    GPIO_PortToggle(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9);
    SysTick_DelayTicks(Dmax - (Dmax * DutyCycle) / 100); // Tiempo apagado
}

//Esta es la funcion del teclado
void Leerteclado(void)
{
    for (int fila = 1; fila <= 4; fila++)
    {
        // Apagar todas las filas
        GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_15);
        GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_16);
        GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_2);
        GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_6);

        // Activar la fila actual
        if (fila == 1) GPIO_PortClear(BOARD_GPIO, 1u << BOARD_GPIO_PIN_15);
        if (fila == 2) GPIO_PortClear(BOARD_GPIO, 1u << BOARD_GPIO_PIN_16);
        if (fila == 3) GPIO_PortClear(BOARD_GPIO, 1u << BOARD_GPIO_PIN_2);
        if (fila == 4) GPIO_PortClear(BOARD_GPIO, 1u << BOARD_GPIO_PIN_6);

        // Leer el estado de todas las columnas
        check_col1 = GPIO_PinRead(BOARD_GPIO, BOARD_GPIO_PIN_10);
        check_col2 = GPIO_PinRead(BOARD_GPIO, BOARD_GPIO_PIN_11);
        check_col3 = GPIO_PinRead(BOARD_GPIO, BOARD_GPIO_PIN_12);
        check_col4 = GPIO_PinRead(BOARD_GPIO, BOARD_GPIO_PIN_14);

        // Si alguna columna tiene nivel bajo (botón presionado en esta fila)
        if (check_col1 == 0 || check_col2 == 0 || check_col3 == 0 || check_col4 == 0)
        {
        	//Chequea la fila dependiendo de la entrada de la columna
            if (check_col1 == 0) { col = 1; fil = fila; }
            else if (check_col2 == 0) { col = 2; fil = fila; }
            else if (check_col3 == 0) { col = 3; fil = fila; }
            else if (check_col4 == 0) { col = 4; fil = fila; }
        }
    }
}

//Empieza el programa principal
int main(void)
{

    BOARD_InitPins();
    BOARD_InitBootClocks();

    // Condiciones Iniciales PWM
    DutyCycle = 50; // De 0 a 99
    Dmax = 100U;    // Periodo máximo con D = 100%
    Ton = (Dmax * DutyCycle) / 100;
    Toff = Dmax - Ton;

    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 100000U))
    {
        while (1) {}
    }

    // GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); //Apaga led
    enum Estados Estado_actual = INICIO; //Declara que siempre empieza en inicio

    while (1)
    {
        switch (Estado_actual)
        {
            case INICIO:
                Leerteclado(); //Llama la funcion teclado para ver que se esta oprimiendo en la matriz

                if (col == 4 && fil == 4) // A
                {
                    Estado_actual = PWM; // Si se oprime la A pasa a estado PWM
                    GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); //Apaga led por seguridad
                }
                else
                {
                    Estado_actual = INICIO; //Si no se oprime nada sigue en inicio
                    GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); //Apaga led por seguridad
                }
                col = 0, fil = 0; // Por seguridad se reicia el teclado poniendo las filas y las columnas en 0
                break;

            case PWM:
                Leerteclado(); //Llama la funcion teclado para ver que se esta oprimiendo en la matriz

                if (col == 4 && fil == 3) // B
                {
                    Estado_actual = INICIO; // Este estado hace que en cualquier parte que este del programa se vaya para estado incio
                    GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); //Apaga led por seguridad
                }
                else if (col < 4 && col != 0) //Anulamos letras , ya que se espera una entrada numerica
                {
                    if ((col == 1 || col == 3) && fil == 1) // * o #
                    {
                        Estado_actual = PWM; // Si se oprime alguanas de las las letras, * y # se reinicia el estado
                        ActivarPWM();
                    }
                    else if (col == 2 && fil == 1) // si es 0 el primer numero
                    {
                        DEC = 0;
                        col = 0, fil = 0;
                        while (col == 0 && fil == 0)
                        {
                        	ActivarPWM();
                            Leerteclado(); // Espera el segundo numero y ni no se ingresa nada vuelve al principio del estado actual
                        }
                        if (col == 2 && fil == 1) // si vuelve a ser 0
                        {
                            NUM = 0; // Literal apaga el led por que el porcentaje es 0% :v
                            ActivarPWM();
                            Estado_actual = AJUSTEDC; // Te manda para el siguiente estado ya que es una entrada valida.
                        }
                        else
                        {
                            NUM = col - 3 * (fil - 4); // Algoritmo por si solo ingresa un solo numero
                            ActivarPWM();
                            Estado_actual = AJUSTEDC; //Te manda para el siguiente estado ya que es una entrada valida.
                        }
                    }
                    else // si no es cero el primer numero
                    {
                        DEC = col - 3 * (fil - 4);
                        col = 0, fil = 0;
                        while (col == 0 && fil == 0)
                        {
                            Leerteclado();
                        }
                        if (col == 2 && fil == 1) // si es cero
                        {
                            NUM = 0; // De igual manera en 0% entonces apaga
                            ActivarPWM();
                            Estado_actual = AJUSTEDC;
                        }
                        else
                        {
                            NUM = col - 3 * (fil - 4);
                            ActivarPWM();
                            Estado_actual = AJUSTEDC; // Ya cuando es una variable normal
                        }
                    }
                }
                else // A, C, D o nada
                {
                    Estado_actual = PWM; // Vuelve y se repite ya que no son validas esas entradas
                    ActivarPWM();
                }
                col = 0, fil = 0;
                break;

            case AJUSTEDC:
                Leerteclado();
                if (col == 4 && fil == 3) // B
                {
                    Estado_actual = INICIO; // Si de presiona B se devuelve al estado inicial
                    GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); //Apaga led por seguridad
                }
                else if (col == 4 && fil == 2) // C
                {
                	ActivarPWM();
                    Estado_actual = PWM; // Si se presiona C se cancela el proceso de cambio y se regresa al estado anterior.
                }
                else if (col == 4 && fil == 1) // D
                {
                	// Se ingresa el valor al que se desea la intensidad del Led
                    DutyCycle = (DEC * 10) + NUM; // De 0 a 99
                    Ton = (Dmax * DutyCycle) / 100;
                    Toff = Dmax - Ton;
                    Estado_actual = PWM;
                }
                else
                {
                	ActivarPWM();
                    Estado_actual = AJUSTEDC; // Se queda a la espera de un nuevo valor despues de haber ingresado ya uno
                }
                col = 0, fil = 0; // Si no se ingresa nada se reinicia la matriz
                break;
        }
    }
    return 0;
}
