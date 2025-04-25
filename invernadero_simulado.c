#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "invernadero.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#endif

static float temperatura_actual = 25.0;
static float humedad_actual = 50.0;
static int simulacion_temp_activa = 0;
static int simulacion_hum_activa = 0;

int abrirPuertoSerie(const char* nombrePuerto) {
    srand(time(0));
    return 1; // no hace nada en la versión simulada
}

void cerrarPuertoSerie() {
    // No hace nada
}

void encenderBomba() {
    printf("[SIMULADO] Bomba ENCENDIDA\n");
}

void apagarBomba() {
    printf("[SIMULADO] Bomba APAGADA\n");
}

void encenderVentilador() {
    printf("[SIMULADO] Ventilador ENCENDIDO\n");
}

void apagarVentilador() {
    printf("[SIMULADO] Ventilador APAGADO\n");
}

void encenderLed(const char* color) {
    printf("[SIMULADO] LED encendido con color: %s\n", color);
}

void apagarLed() {
    printf("[SIMULADO] LED apagado\n");
}

float generarVariacion(float valor, float min, float max, float delta) {
    float cambio = ((rand() % ((int)(2 * delta * 10 + 1))) - delta * 10) / 10.0;
    float nuevo = valor + cambio;
    if (nuevo < min) nuevo = min;
    if (nuevo > max) nuevo = max;
    return nuevo;
}

float leerTemperatura() {
    if (!simulacion_temp_activa) {
        temperatura_actual = generarVariacion(temperatura_actual, 0, 50, 1.5);
    }
    return temperatura_actual;
}

float leerHumedad() {
    if (!simulacion_hum_activa) {
        humedad_actual = generarVariacion(humedad_actual, 10, 90, 2.0);
    }
    return humedad_actual;
}

void escribirTemperatura(float valor) {
    temperatura_actual = valor;
    simulacion_temp_activa = 1;
}

void resetTemperatura() {
    simulacion_temp_activa = 0;
}

void escribirHumedad(float valor) {
    humedad_actual = valor;
    simulacion_hum_activa = 1;
}

void resetHumedad() {
    simulacion_hum_activa = 0;
}

void escribirLCD(int fila, int columna, const char* texto) {
    printf("[LCD][%d,%d]: %s\n", fila, columna, texto);
}

void leerEstado(char* buffer, int len) {
    snprintf(buffer, len, "[SIMULADO] Estado: Bomba y ventilador normal");
}

void estadoSistema(char* buffer, int len) {
    snprintf(buffer, len, "[SIMULADO] Temp: %.2f, Hum: %.2f", temperatura_actual, humedad_actual);
}

void resetSistema() {
    printf("[SIMULADO] Sistema reiniciado\n");
    simulacion_temp_activa = 0;
    simulacion_hum_activa = 0;
}

void testComponentes() {
    printf("[SIMULADO] Prueba de componentes ejecutada\n");
    encenderBomba();
    encenderVentilador();
    encenderLed("ROJO");
    escribirLCD(0, 0, "Test OK");
}

/**
 * @brief Revisa si se ha presionado una tecla.
 *
 * Funciona tanto en Windows como en Linux/MacOS.
 *
 * @return 1 si hay tecla presionada, 0 si no.
 */
int kbhit(void) {
    #ifdef _WIN32
        return _kbhit();
    #else
        struct termios oldt, newt;
        int oldf;
        struct timeval tv = {0, 0};
    
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // sin buffer y sin eco
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &tv);
    
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    
        return rv > 0;
    #endif
}

/**
 * @brief Espera un segundo de manera portable.
 *
 * Usa Sleep() en Windows y usleep() en Linux/Mac.
 */
void esperarUnSegundo(void) {
    #ifdef _WIN32
        Sleep(1000); // 1000 ms = 1 segundo
    #else
        usleep(1000000); // 1,000,000 microsegundos = 1 segundo
    #endif
}