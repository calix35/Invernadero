#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#include "invernadero.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#endif

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
    memset(buffer, 0, len);

    int index = 0;
#ifdef _WIN32
    DWORD bytesRead;
    char c;
    while (index < len - 1) {
        ReadFile(hSerial, &c, 1, &bytesRead, NULL);
        if (bytesRead > 0) {
            buffer[index++] = c;
            if (c == '\n') break; // Terminó la línea
        } else {
            Sleep(10); // Esperar si no hay datos
        }
    }
#else
    char c;
    int n;
    while (index < len - 1) {
        n = read(serialPort, &c, 1);
        if (n > 0) {
            buffer[index++] = c;
            if (c == '\n') break; // Terminó la línea
        } else {
            usleep(10000); // Esperar 10 ms
        }
    }
#endif
    buffer[index] = '\0'; // Finalizar la cadena
}

void encenderBomba() { enviarComando("BOMBA ON\n"); }
void apagarBomba()   { enviarComando("BOMBA OFF\n"); }
void encenderVentilador() { enviarComando("FAN ON\n"); }
void apagarVentilador()   { enviarComando("FAN OFF\n"); }
void encenderLed(const char* color) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "LED %s\n", color);
    enviarComando(cmd);
}
void apagarLed() { enviarComando("LED OFF\n"); }

float leerTemperatura() {
    enviarComando("GET TEMP\n");
    char buffer[64];
    leerRespuesta(buffer, sizeof(buffer));
    return strtod(buffer, NULL);
}

float leerHumedad() {
    enviarComando("GET HUM\n");
    char buffer[64];
    leerRespuesta(buffer, sizeof(buffer));
    return strtod(buffer, NULL);
}

void escribirTemperatura(float valor) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "SET TEMP %.2f\n", valor);
    enviarComando(cmd);
}

void resetTemperatura() {
    enviarComando("RESET TEMP\n");
}

void escribirHumedad(float valor) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "SET HUM %.2f\n", valor);
    enviarComando(cmd);
}

void resetHumedad() {
    enviarComando("RESET HUM\n");
}

void escribirLCD(int fila, int columna, const char* texto) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "LCD %d %d %s\n", fila, columna, texto);
    enviarComando(cmd);
}

void leerEstado(char* buffer, int len) {
    enviarComando("GET STATE\n");
    leerRespuesta(buffer, len);
}

void estadoSistema(char* buffer, int len) {
    enviarComando("GET FULL\n");
    leerRespuesta(buffer, len);
}

void resetSistema() {
    enviarComando("RESET ALL\n");
}

void testComponentes() {
    enviarComando("TEST\n");
}

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

void esperarUnSegundo(void) {
    #ifdef _WIN32
        Sleep(1000); // 1000 ms = 1 segundo
    #else
        usleep(1000000); // 1,000,000 microsegundos = 1 segundo
    #endif
}