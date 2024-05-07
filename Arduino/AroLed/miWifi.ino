
IPAddress ip_local(192, 168, 50, 123);
IPAddress gateway(192, 168, 50, 1);
IPAddress subnet(255, 255, 255, 0);

void conectarWifi() {
  Serial.println();
  Serial.print("Conectando a Wifi ..");
  if (!WiFi.config(ip_local, gateway, subnet)) {
    Serial.println("Error en configuracion");
  }
  WiFi.begin(ssid_1, password_1);
  WiFi.mode(WIFI_STA);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    estado = noServer;
    actualizarEstado();
    delay(500);
  }
  Serial.println(".. Conectado");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print(" IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("MDNS: ");
  Serial.print(NombreESP);
  Serial.println(".local");

  if (!WiFi.config(ip_local, gateway, subnet)) {
    Serial.println("Error en configuracion Red");
    estado = noWifi;
  }

  MDNS.begin(NombreESP);

  configurarOTA();

  TelnetStream.begin();
  MDNS.addService("telnet", "tcp", 23);
}

void actualizarWifi() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi No Conectada!");
    delay(500);
    estado = noWifi;
    return;
  } else {
    estado = noServer;
  }

#if defined(ESP8266)
  MDNS.update();
#endif
  ArduinoOTA.handle();
  delay(10);
}


void LeerTelnet() {
  if (TelnetStream.available()) {
    char Letra = TelnetStream.read();
    switch (Letra) {
      case 'e':
      case 'E':
        TelnetStream.println("Estado: ");
        TelnetStream.print(" Imprimiendo: ");
        TelnetStream.println((EstadoImprimiendo ? "Activo" : "Apagado"));
        TelnetStream.print(" EstadoTelegram: ");
        TelnetStream.println((EstadoImprimiendo ? "Activo" : "Apagado"));
        TelnetStream.print(" Nivel: ");
        TelnetStream.println(Nivel);
        TelnetStream.print(" Rojo: ");
        TelnetStream.println(Rojo);
        TelnetStream.print(" Verde: ");
        TelnetStream.println(Verde);
        TelnetStream.print(" Azul: ");
        TelnetStream.println(Azul);
        TelnetStream.println();
        break;
      case 'f':
      case 'F':
        if (!LittleFS.format()) {
          TelnetStream.println("Error formatiando");
          return;
        } else {
          TelnetStream.println("Se boro todo");
        }
        break;
    }
  }
}
