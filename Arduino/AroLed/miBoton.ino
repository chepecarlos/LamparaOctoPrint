void iniciarBotones() {
  pinMode(BotonRojoTemperatura, INPUT);
  pinMode(BotonAmarilloCambiarColor, INPUT);
  pinMode(BotonAzulReImprimir, INPUT);
}

//Agregar funcion de Apagar arro
void actualizarBotones() {
  boolean EstadoTemperatura = digitalRead(BotonRojoTemperatura);
  boolean EstadoCambiarColor = digitalRead(BotonAmarilloCambiarColor);
  boolean EstadoReImprimir = digitalRead(BotonAzulReImprimir);

  if (EstadoTemperatura) {
    cambiarTemperatura();
  } else if (EstadoCambiarColor) {

  } else if (EstadoReImprimir) {
    reImprimir();
  }
}

void cambiarTemperatura() {
  int estadoAnterior = estado;
  estado = consultando;
  Serial.println("Boton Cambiar Temperatura");
  TelnetStream.println("Boton Cambiar Temperatura");
  int TemperaturaEstrusor = obtenerTemperatura();
  if (TemperaturaEstrusor >= 210) {
    Serial.println("Apagar Estrusor");
    TelnetStream.println("Apagar Estrusor");
    consultaPost("printer/tool", "{\"command\":\"target\",\"targets\":{\"tool0\":0}}");
  } else {
    Serial.println("Apagar Estrusor");
    TelnetStream.println("Apagar Estrusor");
    consultaPost("printer/tool", "{\"command\":\"target\",\"targets\":{\"tool0\":220}}");
  }
  estado = estadoAnterior;
  actualizarEstado();
}

void reImprimir() {
  int estadoAnterior = estado;
  estado = consultando;
  Serial.println("Boton ReImprimir");
  TelnetStream.println("Boton ReImprimir");
  bool estadoImprimiendo = obtenerImprimiendo();
  if (estadoImprimiendo) {
    consultaPost("job", "{\"command\": \"start\"}");
  } else {
    Serial.println("No se puede re-imprimir");
    TelnetStream.println("No se puede re-imprimir");
  }
  estado = estadoAnterior;
  actualizarEstado();
}

void cambiarFilamento() {
  int estadoAnterior = estado;
  estado = consultando;
  actualizarEstado();
  Serial.println("Boton Color");
  TelnetStream.println("Boton Color");
  bool estadoImprimiendo = obtenerImprimiendo();
  if (estadoImprimiendo) {
    consultaPost("printer/command", "{\"command\":\"M600\"}");
  } else {
    consultaPost("printer/command", "{\"command\":\"M104 S210 T0\"}");
    consultaPost("printer/command", "{\"command\":\"G28 X Y\"}");
    consultaPost("printer/command", "{\"command\":\"G91\"}");
    consultaPost("printer/command", "{\"command\":\"G0 X-100 F4000\"}");
    consultaPost("printer/command", "{\"command\":\"M109 S210 T0\"}");
    consultaPost("printer/command", "{\"command\":\"M83\"}");
    consultaPost("printer/command", "{\"command\":\"G1 E10 F300\"}");
    consultaPost("printer/command", "{\"command\":\"M82\"}");
    consultaPost("printer/command", "{\"command\":\"G90\"}");
    consultaPost("printer/command", "{\"command\":\"M600\"}");
  }
  estado = estadoAnterior;
  actualizarEstado();
}