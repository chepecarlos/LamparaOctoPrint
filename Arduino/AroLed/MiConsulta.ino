#include <ArduinoJson.h>

const char* serverBase = "http://prusamini.local/api/";

void consultaPost(String uri, String consulta) {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  WiFiClient clienteWifi;
  HTTPClient http;

  http.begin(clienteWifi, serverBase + uri);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Api-Key", token_octoprint);

  int respuestaColsulta = http.POST(consulta);

  Serial.print("Respuesta Codigo HTTP: ");
  Serial.println(respuestaColsulta);

  TelnetStream.print("Respuesta Codigo HTTP: ");
  TelnetStream.println(respuestaColsulta);

  http.end();
}

String consultaGet(String uri) {
  if (WiFi.status() != WL_CONNECTED) {
    return "";
  }

  WiFiClient clienteWifi;
  HTTPClient http;

  http.begin(clienteWifi, serverBase + uri);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Api-Key", token_octoprint);

  int respuestaColsulta = http.GET();
  String MensajeGet = http.getString();

  Serial.print("Respuesta Codigo HTTP: ");
  Serial.println(respuestaColsulta);
  Serial.println();
  Serial.println(MensajeGet);

  TelnetStream.print("Respuesta Codigo HTTP: ");
  TelnetStream.println(respuestaColsulta);
  TelnetStream.println();
  TelnetStream.println(MensajeGet);

  http.end();
  return MensajeGet;
}

int obtenerTemperatura() {
  String Respuesta = consultaGet("printer");
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, Respuesta);
  JsonObject data = doc.as<JsonObject>();
  int numero = data["temperature"]["tool0"]["target"].as<int>();
  Serial.print("Temeratura Estrusor: ");
  Serial.println(numero);
  return numero;
}

bool obtenerImprimiendo() {
  String Respuesta = consultaGet("printer");
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, Respuesta);
  JsonObject data = doc.as<JsonObject>();
  int estado = data["state"]["flags"]["printing"].as<bool>();
  Serial.print("Imprimiendo: ");
  Serial.println(estado ? "Si" : "No");
  return estado;
}