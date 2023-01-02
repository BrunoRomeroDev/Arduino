/*
      Programa Aciona LED via WiFI
      Conexão do NodeMCU no Wifi e aciona o Built-In LED por uma página Http local

      Componentes:
        - NodeMCU ESP8266

      Versão 1.0 - Versão inicial via HTTP Local - 28/Jan/2021

 *    * Criado por Cleber Borges - FunBots - @cleber.funbots  *     *

      Instagram: https://www.instagram.com/cleber.funbots/
      Facebook: https://www.facebook.com/cleber.funbots
      YouTube: https://www.youtube.com/channel/UCKs2l5weIqgJQxiLj0A6Atw
      Telegram: https://t.me/cleberfunbots

*/

// Inclusão das bibliotecas
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Configração do WiFi
const char* ssid = "ROMERO_2G";  // SSID Wifi
const char* password = "17052015";  // Senha Wifi

// Variáveis de Server e Status do LED
ESP8266WebServer server(80);
bool LEDstatus = LOW;

void setup() {
  // Inicia Serial e LED
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Inicia Conexão WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Aguarda Conexão e Informa IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Rede WiFi: ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  delay(100);

  // Configura Handles do Server e Inicia Server
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("Servidor HTTP iniciado!");

}

void loop() {
  server.handleClient();    // Faz o Handle
  if (LEDstatus)            // Checa se LED deve acender
    digitalWrite(LED_BUILTIN, HIGH);  
  else
    digitalWrite(LED_BUILTIN, LOW);
}

// FUNÇÕES HANDLE PARA HTML SERVER

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_ledon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true));
}

void handle_ledoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_ledchange() {
  if (LEDstatus){
      LEDstatus = HIGH;
      server.send(200, "text/html", SendHTML(true));
  }else{
      LEDstatus = LOW;
      server.send(200, "text/html", SendHTML(false));
    }
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led) {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "<title>Controle do LED</title>\n";

  ptr += "<style>\n";
  ptr += "                  body {\n";
  ptr += "              margin: 0;\n";
  ptr += "              padding: 0;\n";
  ptr += "              font-family: verdana;\n";
  ptr += "          }\n";
  ptr += "          .center {\n";
  ptr += "              position: absolute;\n";
  ptr += "              top: 50%;\n";
  ptr += "              left: 50%;\n";
  ptr += "              transform: translate(-50%, -50%);\n";
  ptr += "          }\n";
  ptr += "          a {\n";
  ptr += "              text-decoration: none;\n";
  ptr += "              text-transform: uppercase;\n";
  ptr += "              font-size: 30px;\n";
  ptr += "          }\n";
  ptr += "          a input {\n";
  ptr += "              padding: 15px;\n";
  ptr += "              transition: .5s;\n";
  ptr += "              position: relative;\n";
  ptr += "          }\n";
  ptr += "          a input:nth-child(1) {\n";
  ptr += "              color: #fff;\n";
  ptr += "              background: #262626;\n";
  ptr += "          }\n";
  ptr += "          a input:nth-child(2) {\n";
  ptr += "              color: #fff;\n";
  ptr += "              background: #ff3636;\n";
  ptr += "          }\n";
  ptr += "          a input:nth-child(1):before {\n";
  ptr += "              content: attr(data-attr);\n";
  ptr += "              position: absolute;\n";
  ptr += "              top: 0;\n";
  ptr += "              left: 0;\n";
  ptr += "              background: #ff3636;\n";
  ptr += "              padding: 15px;\n";
  ptr += "              transition: 0.5S;\n";
  ptr += "              transform-origin: top;\n";
  ptr += "              transform: rotateX(90deg) translateY(-50%);\n";
  ptr += "          }\n";
  ptr += "          a:hover input:nth-child(1):before {\n";
  ptr += "              transform: rotateX(0deg) translateY(0%);\n";
  ptr += "          } \n";
  ptr += "          a input:nth-child(2):before {\n";
  ptr += "              content: attr(data-attr);\n";
  ptr += "              position: absolute;\n";
  ptr += "              top: 0;\n";
  ptr += "              left: 0;\n";
  ptr += "              background: #262626;\n";
  ptr += "              padding: 15px;\n";
  ptr += "              transition: 0.5S;\n";
  ptr += "              transform-origin: bottom;\n";
  ptr += "              transform: rotateX(90deg) translateY(50%);\n";
  ptr += "          }\n";
  ptr += "          a:hover input:nth-child(2):before {\n";
  ptr += "              transform: rotateX(0deg) translateY(0%);\n";
  ptr += "          } \n";
  ptr += "          a  input:nth-child(1):after {\n";
  ptr += "          content: attr(data-attr);\n";
  ptr += "          padding: 15px;\n";
  ptr += "          position: absolute;\n";
  ptr += "          top: 0;\n";
  ptr += "          left: 0;\n";
  ptr += "          background: #262626;\n";
  ptr += "          transform-origin: bottom;\n";
  ptr += "          transform: rotateX(0deg) translateY(0%);\n";
  ptr += "          transition: 0.5s;\n";
  ptr += "          }\n";
  ptr += "          a:hover input:nth-child(1):after {\n";
  ptr += "          transform: rotateX(90deg) translateY(50%);\n";
  ptr += "          }\n";
  ptr += "          a input:nth-child(2):after {\n";
  ptr += "              content: attr(data-attr);\n";
  ptr += "              position: absolute;\n";
  ptr += "              top: 0;\n";
  ptr += "              left: 0;\n";
  ptr += "              background: #ff3636;\n";
  ptr += "              padding: 15px;\n";
  ptr += "              transition: 0.5S;\n";
  ptr += "              transform-origin: top;\n";
  ptr += "              transform: rotateX(0deg) translateY(0%);\n";
  ptr += "          }\n";
  ptr += "          a:hover input:nth-child(2):after {\n";
  ptr += "              transform: rotateX(90deg) translateY(-50%);\n";
  ptr += "          } \n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<form method=\"get\">\n";
  if (led){
      ptr += "<div class='center'>\n";
      ptr += "    <a href='/ledoff'><input data-attr='Led off' type=\"button\" value=\"LED OFF\" onclick=\"window.location.href='/ledoff'\"></a>\n";
      ptr += "</div>\n";
  }
  else{
      ptr += "<div class='center'>\n";
      ptr += "    <a href='/ledon'><input style='background: #ff3636' data-attr='Led on' type=\"button\" value=\"LED ON\" onclick=\"window.location.href='/ledon'\"></a>\n";
      ptr += "</div>\n";
      }
  ptr += "</form>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  
  
  
  return ptr;
}
