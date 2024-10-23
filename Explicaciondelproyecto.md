# Explicacion del proyecto:

```markdown
# Explicación del código

Este código implementa la interacción con un teclado matricial para ajustar el ciclo de trabajo (duty cycle) de un PWM y controlar un LED. El sistema tiene tres estados: `INICIO`, `PWM`, y `AJUSTEDC`, gestionados mediante una máquina de estados.

---

## 1. Librerías y Definiciones
```c
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include <stdio.h>
```
- **Librerías incluidas**: Estas librerías son específicas para la inicialización del hardware, la configuración de pines y relojes, y para manejar la entrada/salida (I/O) estándar con `stdio.h`.

```c
#define BOARD_GPIO    BOARD_INITPINS_PINES_GPIO
```
- **Definición de pines GPIO**: Se define un alias para el puerto GPIO que controla los pines del teclado y del LED.

```c
/* Definiciones de pines */
#define BOARD_GPIO_PIN_9   BOARD_INITPINS_PINES_9  // LED (pin E2)
#define BOARD_GPIO_PIN_10  BOARD_INITPINS_PINES_10 // Columna 1
#define BOARD_GPIO_PIN_11  BOARD_INITPINS_PINES_11 // Columna 2
#define BOARD_GPIO_PIN_12  BOARD_INITPINS_PINES_12 // Columna 3
#define BOARD_GPIO_PIN_14  BOARD_INITPINS_PINES_14 // Columna 4
#define BOARD_GPIO_PIN_15  BOARD_INITPINS_PINES_15 // Fila 1
#define BOARD_GPIO_PIN_16  BOARD_INITPINS_PINES_16 // Fila 2
#define BOARD_GPIO_PIN_2   BOARD_INITPINS_PINES_2  // Fila 3
#define BOARD_GPIO_PIN_6   BOARD_INITPINS_PINES_6  // Fila 4
```
- **Pines del teclado y el LED**: Se definen los pines correspondientes a las filas y columnas del teclado y al LED.

---

## 2. Variables Globales
```c
int col = 0, fil = 0;
int cont = 1;
int DutyCycle, Dmax, Ton, Toff;
int NUM, DEC;
int check_col1, check_col2, check_col3, check_col4;
```
- **Variables del teclado**: `col` y `fil` guardan la columna y fila que han sido presionadas.
- **PWM**: `DutyCycle`, `Dmax`, `Ton`, y `Toff` controlan los tiempos del PWM (ciclo de trabajo, máximo valor, tiempo encendido y apagado).
- **Variables para entrada numérica**: `NUM` y `DEC` almacenan el número ingresado a través del teclado.

---

## 3. Enumeración de Estados
```c
enum Estados { INICIO, PWM, AJUSTEDC };
```
- **Estados del sistema**: Se definen tres estados (`INICIO`, `PWM`, y `AJUSTEDC`) para la máquina de estados.

---

## 4. Funciones

### 4.1 `SysTick_Handler()`
```c
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}
```
- **Interrupción de SysTick**: Esta función se invoca cada vez que el temporizador SysTick genera una interrupción, decrementando el contador `g_systickCounter` para manejar retardos.

### 4.2 `SysTick_DelayTicks(uint32_t n)`
```c
void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U) {}
}
```
- **Función de retardo**: Usa el contador `g_systickCounter` para crear un retardo controlado de `n` ticks, bloqueando la ejecución hasta que el contador llegue a cero.

### 4.3 `ActivarPWM()`
```c
void ActivarPWM(void)
{
    GPIO_PortClear(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); // Apaga LED
    SysTick_DelayTicks((Dmax * DutyCycle) / 100); // Tiempo encendido
    GPIO_PortToggle(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); // Enciende LED
    SysTick_DelayTicks(Dmax - (Dmax * DutyCycle) / 100); // Tiempo apagado
}
```
- **Generación de PWM**: Controla el LED usando un ciclo de trabajo (`DutyCycle`). Apaga el LED, espera el tiempo correspondiente al ciclo encendido, y luego lo alterna (enciende y apaga).

### 4.4 `Leerteclado()`
```c
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

        // Leer estado de las columnas
        check_col1 = GPIO_PinRead(BOARD_GPIO, BOARD_GPIO_PIN_10);
        check_col2 = GPIO_PinRead(BOARD_GPIO, BOARD_GPIO_PIN_11);
        check_col3 = GPIO_PinRead(BOARD_GPIO, BOARD_GPIO_PIN_12);
        check_col4 = GPIO_PinRead(BOARD_GPIO, BOARD_GPIO_PIN_14);

        // Si se detecta una tecla presionada
        if (check_col1 == 0 || check_col2 == 0 || check_col3 == 0 || check_col4 == 0)
        {
            if (check_col1 == 0) { col = 1; fil = fila; }
            else if (check_col2 == 0) { col = 2; fil = fila; }
            else if (check_col3 == 0) { col = 3; fil = fila; }
            else if (check_col4 == 0) { col = 4; fil = fila; }
        }
    }
}
```
- **Lectura del teclado matricial**: Activa una fila a la vez y luego revisa qué columnas tienen un valor bajo, lo que indica qué tecla ha sido presionada. Las teclas se identifican por su fila y columna.

---

## 5. Función principal `main()`
```c
int main(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();

    DutyCycle = 50; // Ciclo de trabajo inicial
    Dmax = 100U;    // Valor máximo del periodo

    if (SysTick_Config(SystemCoreClock / 100000U))
    {
        while (1) {} // En caso de error en configuración de SysTick
    }

    enum Estados Estado_actual = INICIO; // Estado inicial

    while (1)
    {
        switch (Estado_actual)
        {
            case INICIO:
                Leerteclado();
                if (col == 4 && fil == 4) // Si se presiona A
                {
                    Estado_actual = PWM;
                    GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); // Apaga el LED
                }
                else
                {
                    Estado_actual = INICIO; // Sigue en INICIO si no se presiona A
                    GPIO_PortSet(BOARD_GPIO, 1u << BOARD_GPIO_PIN_9); // Apaga el LED
                }
                col = 0; fil = 0; // Resetea filas y columnas
                break;
```
- **Estado `INICIO`**: Lee el teclado y verifica si se ha presionado la tecla A (columna 4, fila 4). Si es así, cambia al estado `PWM`; si no, sigue en el estado `INICIO`.

```c
            case PWM:
                // Similar al anterior, pero en este estado se controla el PWM
                ...
                break;
```
- **Estado `PWM`**: Aquí se ajusta el ciclo de trabajo del LED basado en la entrada del teclado.

```c
            case AJUSTEDC:
                // Ajusta el ciclo de trabajo basado en la entrada
                ...
                break;
```
- **Estado `AJUSTEDC`**: Este estado se encarga de ajustar el valor del ciclo de trabajo (`DutyCycle`), esperando la confirmación del usuario.

---


Este código en general implementa una máquina de estados que controla el teclado matricial y ajusta el ciclo de trabajo del PWM para un LED lo cual sabe cuanto puede ser su intencidad dependiendo de que valor en % ingresa el usuario.
