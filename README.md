# Universidad Autónoma de Tamaulipas  
## Facultad de Ingeniería y Ciencias  
### Materia: Programación Estructurada  
#### Docente: Dr. Alan Díaz Manríquez

# 🌿 Control de Invernadero Automatizado

En la Facultad de Ingeniería y Ciencias (FIC), surge la necesidad de contar con un sistema que permita monitorear y automatizar las condiciones de cultivo dentro de un invernadero a pequeña escala. La automatización en este tipo de entornos permite mantener condiciones óptimas de temperatura, humedad, y luz, lo cual es esencial para el crecimiento saludable de las plantas.

Con este contexto, se solicitó a los alumnos de la materia de **Programación Estructurada** el desarrollo de un **prototipo de invernadero automatizado**, que pudiera ser controlado desde un programa en lenguaje C, utilizando un sistema embebido basado en Arduino. 

Este proyecto tiene como propósito integrar y aplicar los conocimientos clave adquiridos durante el curso, tales como:

- Uso de estructuras condicionales y de repetición
- Modularidad mediante funciones
- Manejo de arreglos y matrices
- Comunicación con dispositivos externos

Cada equipo es responsable de desarrollar el software de control que se conectará al invernadero físico, permitiendo la gestión automática o manual de las condiciones internas del sistema.

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
- [Archivos Entregables](#-archivos-entregables)
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

En este modo, el sistema lee continuamente la temperatura y la humedad desde el sensor, y toma decisiones automáticas según la siguiente lógica:

#### 🌡️ Lógica de temperatura
- Si la **temperatura ≥ 30°C** → **Encender ventilador**
- Si la **temperatura < 28°C** → **Apagar ventilador**

#### 💧 Lógica de humedad
- Si la **humedad ≤ 40%** → **Encender bomba de agua**
- Si la **humedad ≥ 60%** → **Apagar bomba de agua**

#### 🔄 Condición combinada
- Si la **temperatura ≥ 30°C** **y** la **humedad ≤ 40%**:
  - Encender **ventilador y bomba**

#### 💡 LED como semáforo ambiental
- Si **temperatura < 30°C** y **humedad > 40%** → **LED verde** (ambiente ideal)
- Si **solo una** condición está fuera de rango → **LED amarillo**
- Si **ambas** condiciones están fuera de rango → **LED rojo** (ambiente crítico)

#### 📌 Nota importante: histeresis
Para evitar que los actuadores cambien constantemente con cada pequeña variación en la lectura del sensor, se utiliza una lógica de **histeresis**:

- La bomba **solo se enciende** si la humedad baja a **40% o menos**
- La bomba **solo se apaga** si la humedad sube a **60% o más**
- En el rango entre 41% y 59%, **la bomba mantiene su estado anterior**

Esto significa que, por ejemplo, si la humedad está en 50%, **la bomba no cambia su estado actual**: se mantiene encendida o apagada según cómo estaba antes.

---

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
    if (abrirPuertoSerie("COM3") < 0) {
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

### Uso con Versión Real o Simulada

Esta librería (`invernadero.h`) puede ser utilizada con dos implementaciones distintas:

- `invernadero.c` – Versión real que se comunica con el Arduino por puerto serial.
- `invernadero_simulado.c` – Versión simulada que permite probar el programa sin hardware.

El archivo `.h` es común para ambas versiones, por lo que los alumnos **no deben modificarlo** ni cambiar el `#include`.

### Compilación:

Para compilar con la versión real en Linux/macOS:

```bash
gcc main.c invernadero.c -o invernadero
```

Para compilar con la versión simulada en Linux/macOS:

```bash
gcc main.c invernadero_simulado.c -o invernadero_simulado
```

Para compilar con la versión real en Windows (con MinGW):

```bash
gcc main.c invernadero.c -o invernadero.exe
```

Para compilar con la versión simulada en Windows (con MinGW):

```bash
gcc main.c invernadero_simulado.c -o invernadero_simulado.exe
```

Esto permite desarrollar y depurar sin necesidad del invernadero físico, y simplemente cambiar el `.c` al compilar para pruebas o entrega final.

---

### 📚 Documentación de la Librería

La documentación técnica de la librería `invernadero` está disponible en el siguiente enlace:

**[https://calix35.github.io/Invernadero/](https://calix35.github.io/Invernadero/)**


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

### 🗓️ Cronograma de Exposición de Proyectos

Las exposiciones se llevarán a cabo de manera **presencial** con la computadora de cada equipo conectada al **invernadero físico**.

> **Lugar:** Por definir

> **Fecha y hora de entrega:** 10/05/2025 20:00 horas

> El archivo `main.c` y el reporte (PDF) deberá ser entregado via Teams (cada alumno deberá subir su propio proyecto, sin importar que su compañero de equipo ya lo haya entregado)

> 📌 **Es requisito para entrega que su código compile**


### Fechas y horarios de presentación:

| Fecha       | Horario         | Integrantes del equipo                |
|-------------|-----------------|----------------------------------------|
| DD/MM/AAAA  | HH:MM - HH:MM   | (Nombre 1, Nombre 2, ...)              |
| DD/MM/AAAA  | HH:MM - HH:MM   | (Nombre 1, Nombre 2, ...)              |
| DD/MM/AAAA  | HH:MM - HH:MM   | (Nombre 1, Nombre 2, ...)              |


(La tabla será actualizada el día 11/05/2025 con los nombres de los alumnos y fechas exactas de presentación, **es responsabilidad del alumno estar pendiente de la
fecha y hora que le corresponde**)


### 🧪 Presentación del Proyecto

Cada equipo deberá exponer su proyecto de forma **presencial**, utilizando su propia computadora conectada al **invernadero físico**.  
Durante la presentación se deberá demostrar el funcionamiento completo del sistema, incluyendo todos los modos implementados.


### ✅ Evaluación del Proyecto

> **Nota importante:** Aunque el proyecto se trabaja en equipo, la **evaluación será individual**.  
> Cada alumno deberá demostrar de manera clara su comprensión del funcionamiento del proyecto durante la revisión personal.

### Rúbrica de Evaluación Individual (100 puntos)

| Criterio                                      | Puntos |
|-----------------------------------------------|--------|
| Explica claramente y muestra correctamente el modo automático         | 25     |
| Explica claramente y muestra correctamente el modo manual             | 5     |
| Explica y demuestra el modo prueba               | 10     |
| Explica y demuestra el modo macro                | 25     |
| Explica cómo funciona el historial de comandos   | 25     |
| Presenta el código `main.c` correctamente        | 10     |
| Total        | 100     |

> **El reporte en PDF es requisito obligatorio para calificar**, pero no tiene ponderación directa.  
> Si no se entrega, no se podrá evaluar el proyecto.

> 📌 **Nota importante:** El invernadero para pruebas físicas se encontrará en el Laboratorio de Inteligencia Artificial, en dicho laboratorio habrá una lista de asistencia en la cual deberán anotarse las personas que asistan a realizar pruebas físicas, esto servirá para valorar su esfuerzo en el proyecto.

> **EN CASO DE QUE EL ALUMNO NO ALCANCE A PROMEDIAR UNA CALIFICACIÓN APROBATORIA EN EL CURSO, PODRÁ EVALUAR NUEVAMENTE EL PROYECTO EN EXTRAORDINARIO, LA FECHA DE ENTREGA Y PRESENTACIÓN SERÁN PUBLICADOS VIA TEAMS**


## 📄 Archivos Entregables

Este proyecto se realizará en **equipo** (máximo 2 personas). Cada equipo deberá entregar los siguientes archivos:

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



## Créditos

Este proyecto fue diseñado como una práctica final integradora para la materia de **Programación Estructurada**, enfocada en el uso de estructuras de control, funciones y manejo de periféricos mediante programación serial.
