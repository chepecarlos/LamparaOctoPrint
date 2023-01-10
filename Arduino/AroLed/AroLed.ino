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

WebServer server(80);

#elif defined(ESP8266)

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

#endif

#include <Ticker.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#include "data.h"

IPAddress ip_local(192, 168, 50, 123);
IPAddress gateway(192, 168, 50, 1);
IPAddress subnet(255, 255, 255, 0);

#define Pin 4
#define CantidadLed 24
Adafruit_NeoPixel tira = Adafruit_NeoPixel(CantidadLed, Pin, NEO_GRB + NEO_KHZ800);

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

void FuncionLed() {
  EstadoLed = !EstadoLed;
  digitalWrite(LedEstado, EstadoLed);
}

void mensajeBase() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", Pagina);
}

void mensajeError() {
  String mensaje = "<h1>404</h1>";
  mensaje += "Pagina No encontrada</br>";
  mensaje += "Intenta otra pagina</br>";
  server.send(404, "text/html", mensaje);
}

void funcionEncender() {
  EstadoImprimiendo = true;
  Serial.println("Encender LED");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Endendiendo LED");
}

void funcionApagar() {
  EstadoImprimiendo = false;
  Serial.println("Apagar LED");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Apagando LED");
}

void funcionNivel() {

  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    Nivel = atoi(Valor.c_str());
    tira.setBrightness(Nivel);
    Serial << "Cambiando nivel " << Nivel << "\n";
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Nivel Cambiado");
}

void funcionColor() {
  Serial.println("Cambiando color");
  if (server.hasArg("r")) {
    Valor = server.arg("r");
    Rojo = atoi(Valor.c_str());
    Serial << "Rojo " << Rojo << "\n";
  }
  if (server.hasArg("g")) {
    Valor = server.arg("g");
    Verde = atoi(Valor.c_str());
    Serial << "Verde " << Verde << "\n";
  }
  if (server.hasArg("b")) {
    Valor = server.arg("b");
    Azul = atoi(Valor.c_str());
    Serial << "Azul " << Azul << "\n";
  }

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "cambiando color");
}

void funcionArcoiris() {
  if (server.hasArg("estado")) {
    Valor = server.arg("estado");
    if (Valor == "vivo") {
      Arcoiris = true;
    } else {
      Arcoiris = false;
    }

    Serial << "Estado Arcoiris " << Arcoiris << "\n";
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "cambiando Arcoiris");
}

void funcionTelegram() {
  if (server.hasArg("estado")) {
    Valor = server.arg("estado");
    if (Valor == "vivo") {
      EstadoTelegram = true;
    } else {
      EstadoTelegram = false;
    }

    Serial << "Estado Telegram " << EstadoTelegram << "\n";
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "cambiando Telegram");
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando Server Web");

  pinMode(LedEstado, OUTPUT);
  if (!WiFi.config(ip_local, gateway, subnet)) {
    Serial.println("Error en configuracion Red");
  }

  WiFi.mode(WIFI_STA);
  cambiarLed.attach(5, FuncionLed);
  Serial.print("Conectando a Wifi ..");
  WiFi.begin(ssid_1, password_1);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println(".. Conectado");
  Serial.print("SSID: ");
  Serial.print(WiFi.SSID());
  Serial.print(" ID: ");
  Serial.println(WiFi.localIP());

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  Serial.println("Inicial NeoPixel");

  server.on("/" , mensajeBase);

  server.on("/vivo", funcionEncender);
  server.on("/muerto", funcionApagar);

  server.on("/nivel", funcionNivel);
  server.on("/color", funcionColor);
  server.on("/arcoiris", funcionArcoiris);
  server.on("/telegram", funcionTelegram);

  server.onNotFound(mensajeError);

  server.begin();
  Serial.println("Servidor HTTP iniciado");

  tira.begin();
  tira.setBrightness(Nivel);
  tira.show();

  Pagina.replace("%ip", WiFi.localIP().toString());
  cambiarLed.attach(0.5, FuncionLed);
}

void loop() {
  server.handleClient();

  if (EstadoImprimiendo || EstadoTelegram) {
    if (Arcoiris) {
      ColorArcoiris();
    } else {
      uint32_t ColorActual = tira.Color(Rojo, Verde, Azul);
      ColorSimple(ColorActual);
    }
  } else {
    tira.clear();
    tira.show();
  }
  delay(10);

}

void ColorArcoiris() {
  tira.rainbow(HueActual);
  HueActual += 256;
  if (HueActual > 65536) {
    HueActual = 0;
  }
  tira.show();
}

void ColorSimple(uint32_t ColorActual) {
  for (int i = 0; i < tira.numPixels(); i++) {
    tira.setPixelColor(i, ColorActual);
  }
  tira.show();
}
