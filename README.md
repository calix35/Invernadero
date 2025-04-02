# Universidad Autónoma de Tamaulipas  
## Facultad de Ingeniería y Ciencias  
### Materia: Programación Estructurada  
#### Docente: Dr. Alan Díaz Manríquez

 Control de Invernadero Automatizado

Este proyecto consiste en controlar un invernadero automatizado desde un programa en C, utilizando un Arduino como sistema embebido conectado por puerto serial. Los alumnos deben implementar un sistema con distintos modos de operación y demostrar el uso de estructuras fundamentales de programación: condicionales, ciclos, funciones, arreglos y matrices.

---

## Menú de Contenidos

- [Descripción General](#descripción-general)
- [Estructura del Proyecto](#estructura-del-proyecto)
  - [Modo Automático](#1-modo-automático)
  - [Modo Manual](#2-modo-manual)
  - [Modo Prueba](#3-modo-prueba)
  - [Modo Macro](#4-modo-macro)
  - [Historial de Comandos Ejecutados](#5-historial-de-comandos-ejecutados)
- [Librería `invernadero`](#librería-invernadero)
- [Compilación](#compilación)
- [Comunicación Serial](#comunicación-serial)
- [Evaluación](#evaluación)
- [Archivos Entregables](#archivos-entregables)
- [Créditos](#créditos)

---

## Descripción General

El Arduino está previamente programado con un firmware que responde a comandos enviados por el programa en C. El sistema puede controlar:

- Una bomba de agua
- Un ventilador
- Una tira de LEDs WS2812
- Una pantalla LCD 1602 con I2C
- Lectura de sensores DHT22 (temperatura y humedad)

---

## Estructura del Proyecto

El programa en C debe implementar un **menú principal** con los siguientes modos:

### 1. Modo Automático

- El sistema lee continuamente temperatura y humedad reales desde el sensor.
- Según la lógica que el alumno defina, decide automáticamente cuándo activar la bomba o el ventilador.
- El usuario puede ver en pantalla las condiciones actuales y los actuadores activos.

**Submenú sugerido:**

```
--- MODO AUTOMÁTICO ---
1. Mostrar condiciones actuales (temp/hum)
2. Ver estado de actuadores (ventilador/bomba)
3. Salir de modo automático
```

---

### 2. Modo Manual

- Permite al usuario controlar manualmente cada componente del sistema.
- Cada acción se traduce en un comando al Arduino.

**Submenú sugerido:**

```
--- MODO MANUAL ---
1. Encender bomba
2. Apagar bomba
3. Encender ventilador
4. Apagar ventilador
5. Encender LED (color)
6. Apagar LED
7. Escribir en LCD
8. Leer temperatura
9. Leer humedad
0. Salir del modo manual
```

---

### 3. Modo Prueba

- Permite simular temperatura y humedad con valores definidos por el usuario.
- El Arduino utilizará esos valores en lugar del sensor real hasta que se reinicie o se restablezca.

**Submenú sugerido:**

```
--- MODO PRUEBA ---
1. Escribir temperatura simulada
2. Restablecer temperatura real
3. Escribir humedad simulada
4. Restablecer humedad real
0. Salir del modo prueba
```

---

### 4. Modo Macro

- El usuario puede grabar una secuencia de comandos (máximo 10).
- Puede ver la lista de comandos guardados.
- Puede ejecutar toda la macro automáticamente.

**Submenú sugerido:**

```
--- MODO MACRO ---
1. Grabar nuevo comando
2. Ver comandos grabados
3. Ejecutar macro
0. Salir del modo macro
```

---

### 5. Historial de Comandos Ejecutados

- Guarda los últimos 10 comandos enviados al Arduino.
- Funciona como un buffer circular: al llegar al límite, sobrescribe los más antiguos.
- Puede ser consultado desde el menú principal.

---

## Librería `invernadero`

Se proporciona una librería en ANSI C (`invernadero.h` y `invernadero.c`) que encapsula todas las funciones necesarias para comunicarse con el Arduino por puerto serial.

### Funcionalidades disponibles:
- Encendido/apagado de bomba y ventilador
- Lectura de temperatura y humedad
- Encendido/apagado de LEDs RGB
- Escritura de texto en LCD (posición y mensaje)
- Simulación de temperatura/humedad
- Reinicio del sistema
- Ejecución de pruebas automáticas
- Envío de comandos directos

### Ejemplo básico de uso:

```c
#include "invernadero.h"
#include <stdio.h>

int main() {
    if (abrirPuertoSerie("/dev/ttyUSB0") < 0) {
        printf("No se pudo abrir el puerto.\n");
        return 1;
    }

    encenderBomba();
    escribirTemperatura(30.5);
    float t = leerTemperatura();
    printf("Temperatura simulada: %.2f\n", t);

    cerrarPuertoSerie();
    return 0;
}
```

---

## Compilación

### En Linux/macOS:

```bash
gcc main.c invernadero.c -o invernadero
```

### En Windows (con MinGW):

```bash
gcc main.c invernadero.c -o invernadero.exe
```

---

## Comunicación Serial

La comunicación con el Arduino se realiza automáticamente a través del puerto serial utilizando la librería `invernadero`. Los alumnos **no necesitan preocuparse por cómo se envían o reciben los comandos**, solo deben usar las funciones proporcionadas como `encenderBomba()`, `leerTemperatura()`, etc.

---

## Evaluación

El programa será evaluado con base en:

- Implementación correcta de los modos
- Uso de funciones y estructuras de control
- Uso obligatorio de:
  - Arreglos en modo macro
  - Arreglo circular en historial de comandos
- Interacción clara por consola
- Creatividad y estructura

---

---

---


---


---


---


---

## 🗓️ Cronograma de Exposición de Proyectos

Las exposiciones se llevarán a cabo de manera **presencial** con la computadora de cada equipo conectada al **invernadero físico**.

> **Lugar:** Por definir


### Fechas y horarios de presentación:

| Fecha       | Horario         | Integrantes del equipo                |
|-------------|-----------------|----------------------------------------|
| DD/MM/AAAA  | HH:MM - HH:MM   | (Nombre 1, Nombre 2, ...)              |
| DD/MM/AAAA  | HH:MM - HH:MM   | (Nombre 1, Nombre 2, ...)              |
| DD/MM/AAAA  | HH:MM - HH:MM   | (Nombre 1, Nombre 2, ...)              |


(La tabla será actualizada posteriormente con los nombres de los equipos y fechas exactas)


## 🧪 Presentación del Proyecto

Cada equipo deberá exponer su proyecto de forma **presencial**, utilizando su propia computadora conectada al **invernadero físico**.  
Durante la presentación se deberá demostrar el funcionamiento completo del sistema, incluyendo todos los modos implementados.


## ✅ Evaluación del Proyecto

> **Nota importante:** Aunque el proyecto se trabaja en equipo, la **evaluación será individual**.  
> Cada alumno deberá demostrar de manera clara su comprensión del funcionamiento del proyecto  
> durante la revisión personal.

### Rúbrica de Evaluación Individual (100 puntos)

| Criterio                                      | Puntos |
|-----------------------------------------------|--------|
| Explica claramente el funcionamiento del proyecto | 25     |
| Muestra correctamente el modo automático         | 20     |
| Muestra correctamente el modo manual             | 15     |
| Explica y demuestra el modo prueba               | 10     |
| Explica y demuestra el modo macro                | 10     |
| Explica cómo funciona el historial de comandos   | 10     |
| Presenta el código `main.c` correctamente        | 10     |

> **El reporte en PDF es requisito obligatorio para calificar**, pero no tiene ponderación directa.  
> Si no se entrega, no se podrá evaluar el proyecto.


## 📄 Archivos Entregables

Este proyecto se realizará en **equipo**. Cada equipo deberá entregar los siguientes archivos:

1. `main.c` – Programa principal en C que implementa todos los modos y funcionalidades requeridas.
2. `reporte.pdf` – Documento obligatorio para calificar. Debe incluir:

### Estructura del reporte:

- **Portada**:
  - Nombres completos de todos los integrantes del equipo
  - Matrículas y grupo
  - Nombre del proyecto: “Control de Invernadero Automatizado”

- **1. Descripción general del proyecto**
  - ¿Qué hace el sistema? ¿Qué dispositivos controla?
  - ¿Cómo se organiza el programa (menús, funciones, estructura)?

- **2. Modos implementados**
  - Explicar brevemente qué hace cada modo:
    - Modo automático: cómo decide actuar según temperatura y humedad
    - Modo manual: qué se puede controlar
    - Modo prueba: cómo se simulan los valores
    - Modo macro: cómo se graban y ejecutan los comandos
    - Historial: cómo funciona el almacenamiento circular

- **3. Uso de estructuras de programación**
  - ¿Dónde y cómo se usaron: funciones, condicionales, ciclos, arreglos y matrices?

- **4. Dificultades enfrentadas y aprendizajes**
  - ¿Qué fue lo más complicado?
  - ¿Qué aprendieron durante el desarrollo?

- **5. (Opcional) Evidencia visual**
  - Capturas de pantalla o fotografías del programa funcionando



## 📚 Documentación de la Librería

La documentación técnica de la librería `invernadero` está disponible en el siguiente enlace:

**[https://tucuenta.github.io/tu-repo/](https://tucuenta.github.io/tu-repo/)**

(Modifica este enlace según tu repositorio real)


## Créditos

Este proyecto fue diseñado como una práctica final integradora para la materia de **Programación Estructurada**, enfocada en el uso de estructuras de control, funciones y manejo de periféricos mediante programación serial.
