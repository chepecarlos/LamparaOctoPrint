
void escrivirArchivo(const char *ruta, const char *texto) {
  Serial << "Escribiendo en archivo: " << ruta << "\n";

  LittleFS.begin();
  File archivo = LittleFS.open(ruta, "w");
  if (!archivo) {
    Serial.println("Archivo no se puede abir");
    return;
  }

  if (archivo.print(texto)) {
    Serial.println("Archivo Escribiendo");
  } else {
    Serial.println("Error Escribiendo");
  }
  archivo.close();
  LittleFS.end();
}


uint32_t leerArchivo(const char *ruta) {

  LittleFS.begin();
  File archivo = LittleFS.open(ruta, "r");
  if (!archivo) {
    Serial.println("Archivo no se puede abir");
    return -1;
  }

  String Mensaje;
  while (archivo.available()) {
    Mensaje = archivo.readString();

  }
  archivo.close();
  LittleFS.end();

  return Mensaje.toInt();
}
