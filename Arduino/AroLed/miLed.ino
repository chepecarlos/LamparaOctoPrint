void funcionLed() {
  EstadoLed = !EstadoLed;
  digitalWrite(LedEstado, EstadoLed);
}

void actualizarEstado() {
  if (estado != estadoAnterior) {
    Serial.print("Cambiando Estado: ");
    TelnetStream.print("Cambiando Estado: ");

    estadoAnterior = estado;

    switch (estado) {
      case noWifi:
        cambiarLed.attach(0.1, funcionLed);
        Serial.print("No Wifi");
        TelnetStream.print("No Wifi");
        break;
      case noServer:
        cambiarLed.attach(1, funcionLed);
        Serial.print("No Bot");
        TelnetStream.print("No Bot");
        break;
      case conectado:
        cambiarLed.attach(2, funcionLed);
        Serial.print("Conectado");
        TelnetStream.print("Conectado");
        break;
      case consultando:
        cambiarLed.attach(0.5, funcionLed);
        Serial.print("Consultando");
        TelnetStream.print("Consultando");
        break;
      case errorEstado:
        // todo: estado de error
        break;
    }
    Serial.println();
    TelnetStream.println();
  }
}
