[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/tn5SB-Yw)
# Unidad 3
## Documentación del Proyecto
 
Nombre del estudiante: Jose Carlos Herrera Castro
ID: 000269913

# Ejercicio 1:
1. 
- C
- C++
- Assembly
- Python
- Rust
- Ada
- Java
- Lua
- Embedded JavaScript (Espruino)
- Verilog/VHDL (para sistemas embebidos en FPGA)

2. 
- **C++**: Ofrece más características que C, pero con un posible costo de rendimiento.
- **Assembly**: Aún más cercano al hardware que C, pero con una curva de aprendizaje mucho mayor.
- **Python**: Comparado con C, Python es mucho más lento y menos eficiente en sistemas con recursos limitados. No es una opción común en sistemas embebidos de bajo nivel.
- **Rust**: Más seguro que C en cuanto al manejo de memoria, pero su ecosistema en sistemas embebidos aún está en crecimiento.
- **Ada**: Aunque es más seguro que C para aplicaciones críticas, tiene menos uso y soporte en la industria general de sistemas embebidos.
- **Java**: Java es mucho más pesado y consume más recursos que C, lo que lo hace ineficiente para sistemas embebidos con limitaciones estrictas.
- **Lua**: Comparado con C, Lua es más adecuado para sistemas donde se necesite scripting ligero, pero no para control directo del hardware.
- **Verilog/VHDL**: Estos lenguajes son específicos para diseñar circuitos electrónicos, no para controlar software embebido como C.

3. Sí, existen algunos rankings y análisis de popularidad de lenguajes en el ámbito de sistemas embebidos. Uno de los más reconocidos es el "2023 Embedded Markets Study", que se publica anualmente y clasifica lenguajes basados en su uso y adopción en la industria de sistemas embebidos.
En estos estudios, C sigue siendo el líder indiscutible, seguido de C++. Assembly también mantiene una posición importante debido a su cercanía con el hardware. Python y Rust están emergiendo, especialmente Python para prototipos rápidos y Rust por su enfoque en la seguridad de la memoria. Ada sigue siendo relevante en aplicaciones críticas, como la industria aeroespacial, pero tiene una adopción limitada en otros campos.
- **----> Link**: https://www.geeksforgeeks.org/embedded-systems-programming-languages/

# Ejercicio 2:
~~~
1. 

2. 
#define SET_BIT(reg, bit) ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))
#define TOGGLE_BIT(reg,bit) ((reg)) ^=  (1 << 31)
3.  
#define CHECK_PER(reg) (reg >> 31)
~~~

# Ejercicio 3:
~~~
//Correcion del codigo
#include <stdio.h>
#include <string.h>

int main() {
    // Datos enteros
    float entero = 10.5;  
    printf("El valor del entero es: %d\n", entero);  

    // Datos float
    float decimal = 3.0 / 2.0;  // O tambien float decimal = (float)3 / 2;
    printf("El valor del decimal es: %f\n", decimal);  
    float decimal_1 = 12.5 * 5;  
    printf("El valor del decimal es: %f\n", decimal_1);  

    // Caracteres
    char letra = `A`;  
    printf("El valor del caracter es: %c\n", letra);

    // Cadenas de caracteres
    char nombre[6];  
    strcpy(nombre, "Henry");  
    printf("El nombre es: %s\n", nombre);

    return 0;
}
~~~

# Ejercicio 4:
~~~
#include <stdio.h>

int main() {
    int i;
    int num = 10;
    int array[5] = {1, 2, 3, 4, 5};
    int contador = 0;

    for (i = 1; i < 10; i++) {
        printf("Valor de i: %d\n", i);
    }

    for (i = 0; i <= 4; i++) { // O tambien for (i=0; i<5; i++)
        printf("Elemento del array: %d\n", array[i]);
    }

    while (num != 0) {
        printf("Valor de num: %d\n", num);
        num = num - 1;  
    }

    while (contador < 5) { // El contador nunca se actualiza entonces no va a cambiar
        printf("Valor de contador: %d\n", contador);
    }

    return 0;
}
~~~
## Reto:
Configuré un nuevo pin para un nuevo led en el micro.
- **Aqui declare el gpio y el pin**

![alt text](image.png)

- **Aqui configure el delay del led**

![alt text](image-1.png)

- **Aqui el numero del pin 10 , el regisgtro y el gpio los cuales van ser del registro E**

![alt text](image-2.png)

- **Por ultimo aqui inicialice todo**

![alt text](image-3.png)

![alt text](image-4.png)

## Evaluacion:
# Maquina de estados

![alt text](image-6.png)

# Implentacion del ejempo de SDK de PWM:

- **Led con alta intencidad**
![alt text](image-7.png)

- **Led con baja intencidad**
![alt text](image-8.png)