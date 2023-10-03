
void configurarServer() {
  server.on("/" , mensajeBase);

  server.on("/vivo", funcionEncender);
  server.on("/muerto", funcionApagar);

  server.on("/nivel", funcionNivel);
  server.on("/color", funcionColor);
  server.on("/arcoiris", funcionArcoiris);
  server.on("/telegram", funcionTelegram);

  server.onNotFound(mensajeError);

  server.begin();
  MDNS.addService("http", "tcp", 80);

  Serial.println("Servidor HTTP iniciado");

  Pagina.replace("%ip", WiFi.localIP().toString());

  estado = conectado;
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

  char pollo[10];
  String pollo_tmp = String(Nivel);
  pollo_tmp.toCharArray(pollo, 10);
  escrivirArchivo("/nivel.txt", pollo);
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

  char pollo[10];
  String pollo_tmp = String(Rojo);
  pollo_tmp.toCharArray(pollo, 10);
  escrivirArchivo("/rojo.txt", pollo);

  pollo_tmp = String(Verde);
  pollo_tmp.toCharArray(pollo, 10);
  escrivirArchivo("/verde.txt", pollo);

  pollo_tmp = String(Azul);
  pollo_tmp.toCharArray(pollo, 10);
  escrivirArchivo("/azul.txt", pollo);
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
