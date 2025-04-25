#ifndef INVERNADERO_H
#define INVERNADERO_H

/**
 * @file invernadero.h
 * @brief Librería para controlar un invernadero automatizado desde C.
 *
 * Esta librería proporciona funciones para interactuar con un sistema automatizado de invernadero
 * que puede estar conectado a un Arduino físico o simulado mediante una implementación alternativa.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Abre el puerto de comunicación serial (versión real) o inicializa simulador.
 * @param nombrePuerto Nombre del puerto (ej. "COM4", "/dev/ttyUSB0").
 * @return 1 si se abre correctamente, -1 si falla (real); 1 en simulación.
 */
int abrirPuertoSerie(const char* nombrePuerto);

/**
 * @brief Cierra el puerto de comunicación (real) o limpia el entorno simulado.
 */
void cerrarPuertoSerie();

/**
 * @brief Enciende la bomba de agua.
 */
void encenderBomba();

/**
 * @brief Apaga la bomba de agua.
 */
void apagarBomba();

/**
 * @brief Enciende el ventilador.
 */
void encenderVentilador();

/**
 * @brief Apaga el ventilador.
 */
void apagarVentilador();

/**
 * @brief Lee la temperatura actual del sistema.
 * @return Temperatura como número flotante (dos decimales).
 */
float leerTemperatura();

/**
 * @brief Lee la humedad actual del sistema.
 * @return Humedad como número flotante (dos decimales).
 */
float leerHumedad();

/**
 * @brief Enciende los LEDs en el color indicado.
 * @param color Nombre del color (ej. "ROJO", "VERDE", "AZUL").
 */
void encenderLed(const char* color);

/**
 * @brief Apaga todos los LEDs.
 */
void apagarLed();

/**
 * @brief Establece una temperatura simulada.
 * @param valor Temperatura a simular.
 */
void escribirTemperatura(float valor);

/**
 * @brief Restablece la lectura real de temperatura.
 */
void resetTemperatura();

/**
 * @brief Establece una humedad simulada.
 * @param valor Humedad a simular.
 */
void escribirHumedad(float valor);

/**
 * @brief Restablece la lectura real de humedad.
 */
void resetHumedad();

/**
 * @brief Escribe texto en la pantalla LCD.
 * @param fila Fila (0 o 1)
 * @param columna Columna (0 a 15)
 * @param texto Texto a mostrar
 */
void escribirLCD(int fila, int columna, const char* texto);

/**
 * @brief Lee el estado de bomba y ventilador.
 * @param buffer Cadena de texto con el resultado
 * @param len Longitud máxima del buffer
 */
void leerEstado(char* buffer, int len);

/**
 * @brief Lee el estado general del sistema.
 * @param buffer Cadena de texto con el resultado
 * @param len Longitud máxima del buffer
 */
void estadoSistema(char* buffer, int len);

/**
 * @brief Reinicia el sistema: apaga todo y limpia el LCD.
 */
void resetSistema();

/**
 * @brief Ejecuta una prueba automática de todos los componentes.
 */
void testComponentes();

/**
 * @brief Revisa si se ha presionado una tecla.
 *
 * Funciona tanto en Windows como en Linux/MacOS.
 *
 * @return 1 si hay tecla presionada, 0 si no.
 */
int kbhit(void);

/**
 * @brief Espera un segundo de manera portable.
 *
 * Usa Sleep() en Windows y usleep() en Linux/Mac.
 */
void esperarUnSegundo(void);

#ifdef __cplusplus
}
#endif

#endif