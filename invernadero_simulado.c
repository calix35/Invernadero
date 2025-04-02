#include "invernadero.h"
#include <string.h>
#include <stdio.h>

static float tempSimulada = 25.0;
static float humSimulada = 50.0;
static int usarTempSimulada = 0;
static int usarHumSimulada = 0;

int abrirPuertoSerie(const char* nombrePuerto) {
    return 1;
}

void cerrarPuertoSerie() {
}

void encenderBomba() {
    printf("Simulación: Bomba encendida\n");
}

void apagarBomba() {
    printf("Simulación: Bomba apagada\n");
}

void encenderVentilador() {
    printf("Simulación: Ventilador encendido\n");
}

void apagarVentilador() {
    printf("Simulación: Ventilador apagado\n");
}

float leerTemperatura() {
    float valor = usarTempSimulada ? tempSimulada : 26.5;
    printf("Simulación: Temperatura leída: %.2f°C\n", valor);
    return valor;
}

float leerHumedad() {
    float valor = usarHumSimulada ? humSimulada : 55.5;
    printf("Simulación: Humedad leída: %.2f%%\n", valor);
    return valor;
}

void encenderLed(const char* color) {
    printf("Simulación: LED encendido en color %s\n", color);
}

void apagarLed() {
    printf("Simulación: LED apagado\n");
}

void escribirTemperatura(float valor) {
    tempSimulada = valor;
    usarTempSimulada = 1;
    printf("Simulación: Temperatura simulada establecida a %.2f°C\n", valor);
}

void resetTemperatura() {
    usarTempSimulada = 0;
    printf("Simulación: Restablecida la lectura real de temperatura\n");
}

void escribirHumedad(float valor) {
    humSimulada = valor;
    usarHumSimulada = 1;
    printf("Simulación: Humedad simulada establecida a %.2f%%\n", valor);
}

void resetHumedad() {
    usarHumSimulada = 0;
    printf("Simulación: Restablecida la lectura real de humedad\n");
}

void escribirLCD(int fila, int columna, const char* texto) {
    printf("Simulación: LCD[%d][%d]: %s\n", fila, columna, texto);
}

void leerEstado(char* buffer, int len) {
    snprintf(buffer, len, "Simulación: Bomba=ON, Ventilador=OFF");
}

void estadoSistema(char* buffer, int len) {
    snprintf(buffer, len, "Simulación completa: Temp=%.2f°C, Hum=%.2f%%", leerTemperatura(), leerHumedad());
}

void resetSistema() {
    usarTempSimulada = 0;
    usarHumSimulada = 0;
    printf("Simulación: Sistema reiniciado\n");
}

void testComponentes() {
    printf("Simulación: Ejecutando prueba de componentes...\n");
    encenderBomba();
    encenderVentilador();
    encenderLed("ROJO");
    escribirLCD(0, 0, "Test OK");
}
