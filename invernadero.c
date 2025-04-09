#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#endif

#include "invernadero.h"

#ifdef _WIN32
static HANDLE hSerial = INVALID_HANDLE_VALUE;
#else
static int serialPort = -1;
#endif

int abrirPuertoSerie(const char* nombrePuerto) {
#ifdef _WIN32
    hSerial = CreateFileA(nombrePuerto, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hSerial == INVALID_HANDLE_VALUE) return -1;
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);
    return 1;
#else
    serialPort = open(nombrePuerto, O_RDWR | O_NOCTTY);
    if (serialPort < 0) return -1;
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    tcgetattr(serialPort, &tty);
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tcsetattr(serialPort, TCSANOW, &tty);
    return 1;
#endif
}

void cerrarPuertoSerie() {
#ifdef _WIN32
    if (hSerial != INVALID_HANDLE_VALUE) CloseHandle(hSerial);
#else
    if (serialPort >= 0) close(serialPort);
#endif
}

void enviarComando(const char* comando) {
#ifdef _WIN32
    DWORD bytesWritten;
    WriteFile(hSerial, comando, strlen(comando), &bytesWritten, NULL);
#else
    write(serialPort, comando, strlen(comando));
#endif
}

void leerRespuesta(char* buffer, int len) {
#ifdef _WIN32
    DWORD bytesRead;
    ReadFile(hSerial, buffer, len - 1, &bytesRead, NULL);
    buffer[bytesRead] = '\0';
#else
    int n = read(serialPort, buffer, len - 1);
    buffer[n] = '\0';
#endif
}

void encenderBomba() { enviarComando("ENCENDER_BOMBA\n"); }
void apagarBomba()   { enviarComando("APAGAR_BOMBA\n"); }
void encenderVentilador() { enviarComando("ENCENDER_VENTILADOR\n"); }
void apagarVentilador()   { enviarComando("APAGAR_VENTILADOR\n"); }
void encenderLed(const char* color) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ENCENDER_LED %s\n", color);
    enviarComando(cmd);
}
void apagarLed() { enviarComando("APAGAR_LED\n"); }

float leerTemperatura() {
    enviarComando("LEER_TEMPERATURA\n");
    char buffer[64];
    leerRespuesta(buffer, sizeof(buffer));
    return atof(buffer);
}

float leerHumedad() {
    enviarComando("LEER_HUMEDAD\n");
    char buffer[64];
    leerRespuesta(buffer, sizeof(buffer));
    return atof(buffer);
}

void escribirTemperatura(float valor) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ESCRIBIR_TEMP %.2f\n", valor);
    enviarComando(cmd);
}

void resetTemperatura() {
    enviarComando("RESET_TEMP\n");
}

void escribirHumedad(float valor) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ESCRIBIR_HUM %.2f\n", valor);
    enviarComando(cmd);
}

void resetHumedad() {
    enviarComando("RESET_HUM\n");
}

void escribirLCD(int fila, int columna, const char* texto) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ESCRIBIR_LCD %d %d %s\n", fila, columna, texto);
    enviarComando(cmd);
}

void leerEstado(char* buffer, int len) {
    enviarComando("LEER_ESTADO\n");
    leerRespuesta(buffer, len);
}

void estadoSistema(char* buffer, int len) {
    enviarComando("ESTADO_SISTEMA\n");
    leerRespuesta(buffer, len);
}

void resetSistema() {
    enviarComando("RESET_SISTEMA\n");
}

void testComponentes() {
    enviarComando("TEST_COMPONENTES\n");
}