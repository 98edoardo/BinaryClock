/************************************************
 * Programma realizzato da Edoardo Savini        *
 * Gruppo Savini-Carloni                        *
 * edoardo.savini.stud@ispascalcomandini.gov.it *
 ************************************************/

#include "OrologioBinario.h"
#include "StrisciaLed.h"
#include <Adafruit_NeoPixel.h>

#define DEBUG 1

//WiFiUDP udp;

//const String SERVER = "time.inrim.it";
OrologioBinario *orologio;

const int PIN_LED = 9;

const int PIN_BTN1 = 2; // TO DO
const int PIN_BTN2 = 3; // TO DO

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(21, PIN_LED, NEO_GRB + NEO_KHZ800);
StrisciaLed *striscia;

int statoAttuale = -1;
uint32_t offsetOre = 0;
uint32_t offsetMinuti = 0;
uint8_t offsetSecondi = 0;

void setup() {

  // Apro la seriale se sono in debug
  #if DEBUG
  Serial.begin(9600);
  #endif
  
  // Avvio la striscia LED
  pixels.begin();
  striscia =  new StrisciaLed(&pixels);

/*
  // Mi collego al WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin("Vodafone-Savini", "scottex.1998");

  delay(500);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    // Informazioni per il debug
    #if DEBUG
    Serial.print(".");
    #endif

    // TO DO: lampeggio secondi per collegamento wifi
    
  }

  // TO DO: accensione di tutti i led per completamento connessione e caricamento orario
  
  // Informazioni per il debug
  #if DEBUG
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  #endif

  // Porta per la ricezione delle risposte UDP
  udp.begin(2390);
*/
  // Istanzio l'orologio utilizzando un server NTP
  // E imposto l'offset per il nostro fuso orario
  orologio = new OrologioBinario(0);

  // Dico all'Arduino che le porte saranno utilizzate in INPUT (con resistenza PULLUP integrata)
  // E collego un interrupt
  pinMode(PIN_BTN1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN1), btn1Interrupt, FALLING);

  pinMode(PIN_BTN2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN2), btn2Interrupt, FALLING);
}

void loop() {

  if(statoAttuale == -1) {
    striscia -> SetBlinking(false, 0);
    striscia -> SetBlinking(false, 1);
    striscia -> SetBlinking(false, 2);
    
  } else if(statoAttuale == 0) {
    striscia -> SetBlinking(true, 0);
    striscia -> SetBlinking(false, 1);
    striscia -> SetBlinking(false, 2);
    
  } else if(statoAttuale == 1) {
    striscia -> SetBlinking(false, 0);
    striscia -> SetBlinking(true, 1);
    striscia -> SetBlinking(false, 2);

  } else if(statoAttuale == 2) {
    striscia -> SetBlinking(false, 0);
    striscia -> SetBlinking(false, 1);
    striscia -> SetBlinking(true, 2);
  }
  
  Serial.println((long)(offsetOre + offsetMinuti + offsetSecondi));
  orologio -> SetOffset((long)(offsetOre + offsetMinuti + offsetSecondi));
  striscia -> SetHours((uint8_t)orologio -> GetHours());
  striscia -> SetMinutes((uint8_t)orologio -> GetMinutes());
  striscia -> SetSeconds((uint8_t)orologio -> GetSeconds());
  striscia -> Show();
  
  //#if DEBUG
  Serial.println(String(orologio -> GetHours()) + ":" + String(orologio -> GetMinutes()) + ":" + String(orologio -> GetSeconds()));
  //#endif
  
  delay(500);
}

void btn1Interrupt() {
  if(statoAttuale == 2) {
    statoAttuale = -1;
  } else {
    statoAttuale++;
  }
}

void btn2Interrupt() {
  switch(statoAttuale) {
    case 0:
      offsetOre += 3600;

      if(offsetOre == (uint32_t)86400)
        offsetOre = 0;
      break;

    case 1:
      offsetMinuti += 60;

      if(offsetMinuti == (uint32_t)3600)
        offsetMinuti = 0;
      break;

    case 2:
      offsetSecondi++;

      if(offsetSecondi >= 60)
        offsetSecondi = 0;
      break;
  }
}
