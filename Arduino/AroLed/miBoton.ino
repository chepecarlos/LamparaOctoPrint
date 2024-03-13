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
    Serial.println("Boton Temperatura");
  }
  if (EstadoCambiarColor) {
    Serial.println("Boton Color");
  }
  if (EstadoReImprimir) {
    Serial.println("Boton ReImprimir");
  }
}
