void configurarAro() {
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  Nivel = leerArchivo("/nivel.txt");
  Rojo = leerArchivo("/rojo.txt");
  Verde = leerArchivo("/verde.txt");
  Azul = leerArchivo("/azul.txt");

  Serial.println("Inicial NeoPixel");
  tira.begin();
  tira.setBrightness(Nivel);
  tira.show();
}

void actualizarAro() {
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
