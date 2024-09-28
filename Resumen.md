#   Resumen 
## Introducción
Las interrupciones son eventos que ocurren durante la ejecución de un programa, interrumpiendo su flujo normal para atender una tarea prioritaria. Estas interrupciones pueden ser generadas por hardware (eventos externos como cambios en las entradas) o por software (instrucciones del programa). Las interrupciones se clasifican en enmascarables (controlables mediante habilitación/deshabilitación) y no enmascarables (NMI), las cuales siempre deben ser atendidas.

## Modos de operación del procesador
El procesador ARM Cortex-M4 opera en dos modos:

1. **Thread Mode**: Ejecuta el código principal. Mientras lo hace, puede ser interrumpido para atender eventos externos.
2. **Handler Mode**: Ejecuta el código en respuesta a una interrupción. Durante este modo, las interrupciones normales están deshabilitadas para evitar que el manejo de la interrupción actual sea interrumpido.

## Estados de excepción
Los microcontroladores manejan interrupciones y excepciones en diferentes estados:

1. **Inactivo**: No hay excepciones.
2. **Pendiente**: Se ha detectado una excepción, pero aún no ha sido atendida.
3. **Activo**: El procesador está manejando una excepción.
4. **Activo y Pendiente**: Múltiples excepciones del mismo tipo están ocurriendo simultáneamente.

## Tratamiento de una interrupción
Cuando se atiende una interrupción, el procesador realiza varios pasos:

1. Guarda el estado actual del programa (registros y estado de la pila).
2. Cambia al Handler Mode para ejecutar la rutina de interrupción (ISR).
3. Al terminar, restaura el estado anterior y vuelve al User Mode.

## Módulo NVIC
El Nested Vectored Interrupt Controller (NVIC) es el encargado de gestionar las interrupciones en los microcontroladores ARM Cortex-M4. Configura prioridades, habilita/deshabilita interrupciones, y maneja el vector de interrupción para ejecutar la rutina correspondiente.

Mapa de interrupciones
El mapa de interrupciones es una tabla que contiene los apuntadores a los controladores de interrupciones en la memoria. Cada interrupción tiene un vector que la identifica, como:

- **Non-maskable Interrupt (NMI)**: Interrupción crítica que no puede ser deshabilitada.
- **Hard Fault**: Ocurre cuando hay fallos graves como errores de memoria.
- **Bus Fault y Usage Fault**: Relacionados con errores de acceso a memoria o instrucciones inválidas.

## Número de IRQ y registros NVIC
El número de IRQ es un identificador único para cada interrupción. Para gestionarlas, el NVIC utiliza registros como:

- **ISER (Set Enable)**: Para habilitar una interrupción.
- **ICER (Clear Enable)**: Para deshabilitarla.
- **ISPR (Set Pending)**: Para marcarla como pendiente.
- **ICPR (Clear Pending)**: Para borrar el estado pendiente.
- **IABR (Active Bit)**: Indica si una interrupción está activa.
- **IPR (Priority)**: Establece la prioridad de la interrupción.

## Niveles de prioridad
En los ARM Cortex-M4, las interrupciones tienen niveles de prioridad, donde 0 es el nivel más alto y 15 el más bajo. El NVIC permite gestionar la ejecución de interrupciones en función de su prioridad.

**Ejemplos de código**
Para habilitar la interrupción del temporizador SysTick, se establece su prioridad y se configura en el NVIC. Las funciones básicas para manipular interrupciones incluyen:

- **Habilitar**: NVIC_EnableIRQ(IRQn);
- **Deshabilitar**: NVIC_DisableIRQ(IRQn);
- **Marcar como pendiente**: NVIC_SetPendingIRQ(IRQn);
- **Limpiar el estado pendiente**: NVIC_ClearPendingIRQ(IRQn);
- **Establecer prioridad**: NVIC_SetPriority(IRQn, prioridad);

## Rutinas de servicio de interrupción (ISR)

**Ejemplo de ISRs:**
~~~
__attribute__((interrupt)) void SysTick_Handler(void) {
	contador_systick++;
}

__attribute__((interrupt)) void PORTC_IRQHandler(void) {
	if ((PORTC->ISFR & MASK_PULSADOR) != 0) {
		PORTC->ISFR = (MASK_PULSADOR);  // limpiar la bandera
	}
}
~~~

**Ejemplo práctico**
Para atender un pulsador, se configura el pin GPIO como entrada y se habilita una interrupción en el flanco de subida o bajada. Cuando el pulsador cambia de estado, se genera una interrupción, y el microcontrolador ejecuta la rutina asociada para manejar la acción.

Este manejo eficiente de interrupciones permite al microcontrolador responder a eventos externos sin afectar el rendimiento del programa principal.
