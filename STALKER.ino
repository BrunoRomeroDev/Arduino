#include <SoftwareSerial.h>
#include <Servo.h>
#include <HCSR04.h>

SoftwareSerial serial(12,13);

//define se envia informações do funcionamento para o monitor serial
#define habilitaDebugSerial true 

//definição de Bluethooth
int pinPortas[6] = {2,3,4,5,6,7};

//definição de leds

#define pinLed8  8
#define pinLed9  9
#define pinLed10 10

//definição de buzzer
#define pinBuzzer A0

//definição de servo motor 
#define pinServmot A5
Servo servo1;

// DEFINIÇÕES DE PINOS
#define pinTrig A2
#define pinEcho A1

// INSTANCIANDO OBJETOS
HCSR04 sensorHCSR04(pinTrig,pinEcho);

// DEFINIÇÃO DE PARAMETROS
#define distObstaculo 15  // DISTANCIA EM CM PARA CONSIDERAR QUE HÁ OBSTÁCULO A FRENTE

// DECLARAÇÃO DE VARIÁVEIS
boolean       parar = false;
int           vdireita = 0;
int           vesquerda = 0;



//definição do buzzer
#define c  261
#define d  294
#define e  329
#define f  349
#define g  391
#define gS  415
#define a  440
#define aS  455
#define b  466
#define cH  523
#define cSH  554
#define dH  587
#define dSH  622
#define eH  659
#define fH  698
#define fSH  740
#define gH  784
#define gSH  830
#define aH  880

//definição do botao - modo de operação

#define pinBotao   A4

// definição variavel de chaveamento

boolean botaoAtu = false;
boolean clicked = false; 

void setup() {

//controle de Bluethooth
  serial.begin(9600);
  for (int nP=0;nP<6; nP++){
    pinMode(pinPortas[nP], OUTPUT);
    
  }
  
//controle de leds
  pinMode(pinLed8,  OUTPUT);
  pinMode(pinLed9,  OUTPUT);
  pinMode(pinLed10, OUTPUT);
  
//controle de mootores   
   digitalWrite(pinPortas[0], HIGH); //Roda Direita Enable
   digitalWrite(pinPortas[5], HIGH); //Roda Esquerda Enable
 
// controle do buzzer

  pinMode(pinBuzzer, OUTPUT);

//controle botao

  pinMode(pinBotao, INPUT);

  Serial.begin(115200);

//controle de Servo Motor
  servo1.attach(pinServmot);

}

void loop() {

// Chaveamento de modo de operação Automatico/manual
    int veririf = analogRead(pinBotao);
    if(veririf > 835 ){ 
      clicked = true;
     delay(5000);
    }else{
      clicked = false;
    }
    
    if (clicked == 1){
      if(botaoAtu == 1){
          botaoAtu = 0;
      }else{
          botaoAtu = 1;
      }
    }

    if(botaoAtu == 1){
          modoAutomatico();
          Serial.println("Automatico");
    }else{
          modoManual();
          Serial.println("Manual");
    }
   
 
}

void modoManual(){

  digitalWrite(pinLed8 ,LOW);
  digitalWrite(pinLed9 ,HIGH);

   //controle de Bluethooth
  
  if(serial.available()){

    char byteRecebido = serial.read();
     Serial.println(byteRecebido);

     if (byteRecebido == 'A'){ //SEGUIR EM FRENTE

      servo1.write(90);
      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 

      analogWrite (pinPortas[3], 420); //Roda Esquerda Frente
      analogWrite (pinPortas[1], 420); //Roda Direita Frente

      
    }
      if (byteRecebido == 'H'){  //controle do buzzer
               
      tone(pinBuzzer,g,100);
      delay(100);
      noTone(pinBuzzer);
      
      }
     if (byteRecebido == 'B'){ // PARAR
      
      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 
     
      
    }
     if (byteRecebido == 'C'){ // VIRE A ESQUERDA

       servo1.write(179);
      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 
      
      analogWrite (pinPortas[3], 350); //Roda Esquerda Frente
      analogWrite (pinPortas[1], 420); //Roda Direita Frente
     
    }
     if (byteRecebido == 'a'){ // VIRE A DIREITA

       servo1.write(10);      
      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 
      
     analogWrite (pinPortas[3], 420); //Roda Esquerda Frente
     analogWrite (pinPortas[1], 350); //Roda Direita Frente
    }
     if (byteRecebido == 'b'){ //RÉ

      servo1.write(90);
      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 
 
      digitalWrite (pinPortas[4], HIGH); //Roda Esquerda Tras
      digitalWrite (pinPortas[2], HIGH); //Roda Direita Tras


    }


     //controle de leds
     if (byteRecebido == 'e'){
      digitalWrite(pinLed8,  HIGH);
    }
     if (byteRecebido == 'f'){
      digitalWrite(pinLed9,  HIGH);
    }
     if (byteRecebido == 'g'){
      digitalWrite(pinLed10,  HIGH);
    }  
     if (byteRecebido == 'H'){
      digitalWrite(pinLed8,  LOW);
      digitalWrite(pinLed9,  LOW);
      digitalWrite(pinLed10, LOW);
    }

 }
 
}

void modoAutomatico(){
  
       digitalWrite(pinLed9 ,LOW);
       digitalWrite(pinLed8 ,HIGH);

       
       if (sensorHCSR04.dist() <  distObstaculo){                                  
              parar = true;
                while(parar == true){
                    para();
                    delay(300);
                    re();
                    servo1.write(179);
                    delay(500);
                    vdireita =  sensorHCSR04.dist();
                    delay(500);
                    servo1.write(10);
                    delay(500); 
                    vesquerda = sensorHCSR04.dist();
                    delay(500);
                    servo1.write(90);
                    Serial.println(vesquerda);
                    Serial.println(vdireita); 
                      if(vesquerda >= vdireita){
                         esquerda();
                         frente();
                         parar = false;
                         Serial.println("esquerda");
                      }else{             
                         direita();
                         frente();
                         parar = false;  
                         Serial.println("direita");   
                      }
              }  
 
    }else{
      Serial.println("Frente");
      Serial.println(sensorHCSR04.dist()); 
      frente();
  }
 

}


void esquerda(){

      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW);
      analogWrite (pinPortas[3], 350); //Roda Esquerda Frente
      analogWrite (pinPortas[1], 420); //Roda Direita Frente
      delay(300);
      analogWrite (pinPortas[3], LOW); //Roda Esquerda Frente
      analogWrite (pinPortas[1], LOW); //Roda Direita Frente
  
}
void direita(){

      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 
      analogWrite (pinPortas[3], 420); //Roda Esquerda Frente
      analogWrite (pinPortas[1], 350); //Roda Direita Frente
      delay(300);
      analogWrite (pinPortas[3], LOW); //Roda Esquerda Frente
      analogWrite (pinPortas[1], LOW); //Roda Direita Frente
  
}
void frente(){

      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 
      analogWrite (pinPortas[3], 420); //Roda Esquerda Frente
      analogWrite (pinPortas[1], 420); //Roda Direita Frente
  
}
void re(){

      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 
      digitalWrite (pinPortas[4], HIGH); //Roda Esquerda Tras
      digitalWrite (pinPortas[2], HIGH); //Roda Direita Tras
      delay(400);
      digitalWrite (pinPortas[4], LOW); //Roda Esquerda Tras
      digitalWrite (pinPortas[2], LOW); //Roda Direita Tras
  
}

void para(){
      digitalWrite (pinPortas[1], LOW); 
      digitalWrite (pinPortas[2], LOW); 
      digitalWrite (pinPortas[3], LOW); 
      digitalWrite (pinPortas[4], LOW); 
}
