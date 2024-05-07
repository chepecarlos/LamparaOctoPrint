// Creado ChepeCarlos
// Tutorial Completo en https://nocheprogramacion.com
// Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

#if defined(ESP32)

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>

WebServer server(80);

#elif defined(ESP8266)

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);

#endif


#include <WiFiClient.h>
#include <TelnetStream.h>
#include <Ticker.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include "LittleFS.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

#include "data.h"

// estado del sistema
#define noWifi 0
#define noServer 1
#define conectado 2
#define consultando 4
#define errorEstado 3

int estado = noWifi;
int estadoAnterior = -1;

#define PinNeopixel 13
#define CantidadNeopixel 24
Adafruit_NeoPixel tira = Adafruit_NeoPixel(CantidadNeopixel, PinNeopixel, NEO_GRB + NEO_KHZ800);

const uint32_t TiempoEsperaWifi = 5000;
boolean EstadoImprimiendo = false;
boolean EstadoTelegram = false;
boolean Arcoiris = false;
int Nivel = 50;
int Rojo = 255;
int Verde = 0;
int Azul = 0;

long HueActual = 0;
String Valor = "";

Ticker cambiarLed;
int LedEstado = 2;
boolean EstadoLed = false;

int BotonRojoTemperatura = 4;
int BotonAmarilloCambiarColor = 14;
int BotonAzulReImprimir = 5;

void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando Server Web");
  pinMode(LedEstado, OUTPUT);
  iniciarBotones();
  conectarWifi();
  configurarServer();
  configurarAro();
}

void loop() {

  actualizarBotones();
  actualizarWifi();
  server.handleClient();
  actualizarAro();
  actualizarEstado();
  LeerTelnet();
  LeerSerial();
  delay(10);
}

void LeerSerial() {
  if (Serial.available()) {
    char Letra = Serial.read();
    switch (Letra) {
      case 'e':
      case 'E':
        Serial.println("Estado: ");
        Serial << "Estado: " << "\n";
        //        TelnetStream  << " Imprimiendo: " << (EstadoImprimiendo ? "Activo" : "Apagado");
        //        TelnetStream  << " EstadoTelegram: " << (EstadoTelegram ? "Activo" : "Apagado");
        Serial  << " Nivel: " << Nivel;
        Serial  << " Rojo: " << Rojo;
        Serial  << " Verde: " << Verde;
        Serial  << " Azul: " << Azul;
        Serial  << "\n";
        break;
      case 'f':
      case 'F':
        if (!LittleFS.format()) {
          Serial.println("Error formatiando");
          return;
        } else {
          Serial.println("Se boro todo");
        }
        break;
    }
  }
}
