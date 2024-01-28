#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "ROMERO_2G"
#define STAPSK "17052015"
#endif

//DEFINIÇÕES DAS NOTAS
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
#define pinBuzzer  13
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

bool LEDstatus = LOW;
bool BuzzerStatus = LOW;

void setup() {
  // Inicia Serial e LED
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinBuzzer,  OUTPUT);

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
  server.on("/buzzerclickon", handle_buzzerclickon);
  server.on("/buzzerclickoff", handle_buzzerclickoff);
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

  if (BuzzerStatus) {           // Checa se LED deve acender
    tone(pinBuzzer,440,400);
   }
  else{
    digitalWrite(pinBuzzer, LOW); 
  } 
}

// FUNÇÕES HANDLE PARA HTML SERVER

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false,false));
 
}

void handle_ledon() {
  LEDstatus = HIGH;
  if(BuzzerStatus){
  server.send(200, "text/html", SendHTML(true,true));
  }else{
    server.send(200, "text/html", SendHTML(true,false));
  }
}

void handle_ledoff() {
  LEDstatus = LOW;
   if(BuzzerStatus){
    server.send(200, "text/html", SendHTML(false,true));
  }else{
    server.send(200, "text/html", SendHTML(false,false));
  }
}

void handle_buzzerclickon() {
  BuzzerStatus = HIGH;
  if(LEDstatus){
    server.send(200, "text/html", SendHTML(true,true));
  }else{
    server.send(200, "text/html", SendHTML(false,true));
  }
}
void handle_buzzerclickoff() {
  BuzzerStatus = LOW;
  if(LEDstatus){
    server.send(200, "text/html", SendHTML(true,false));
  }else{
    server.send(200, "text/html", SendHTML(false,false));
  }
}
/*
void handle_ledchange() {
  if (LEDstatus){
      LEDstatus = HIGH;
      server.send(200, "text/html", SendHTML(true,false));
  }else{
      LEDstatus = LOW;
      server.send(200, "text/html", SendHTML(false,false));
    }
}
void handle_buzzerchange() {
  if (BuzzerStatus){
      BuzzerStatus = HIGH;
      server.send(200, "text/html", SendHTML(false,true));
  }else{
      BuzzerStatus = LOW;
      server.send(200, "text/html", SendHTML(false,false));
    }
}
*/
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led,uint8_t buz) {
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
  ptr += "              top: 25%;\n";
  ptr += "              left: 60%;\n";
  ptr += "              transform: translate(-50%, -50%);\n";
  ptr += "          }\n";
  ptr += "          .right {\n";
  ptr += "              position: absolute;\n";
  ptr += "              top: 25%;\n";
  ptr += "              left: 40%;\n";
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

   if (buz){
      ptr += "<div class='right'>\n";
      ptr += "    <a href='/buzzerclickoff'><input data-attr='Led off' type=\"button\" value=\"Buzzer OFF\" onclick=\"window.location.href='/buzzerclickoff'\"></a>\n";
      ptr += "</div>\n";
    
  }
  else{
      ptr += "<div class='right'>\n";
      ptr += "    <a href='/buzzerclickon'><input style='background: #ff3636' data-attr='Led on' type=\"button\" value=\"Buzzer ON\" onclick=\"window.location.href='/buzzerclickon'\"></a>\n";
      ptr += "</div>\n";
   
      }
  ptr += "</form>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  
  
  
  return ptr;
}
