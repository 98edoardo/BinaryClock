/************************************************
 * Programma realizzato da Edoardo Savini       *
 * Gruppo Savini-Carloni                        *
 * edoardo.savini.stud@ispascalcomandini.gov.it *
 ************************************************/
#ifndef _LIB_NEOPIXEL
  #define _LIB_NEOPIXEL
  #include <Adafruit_NeoPixel.h>
#endif

// Mettere a 1 se siamo in modalità di debug
#define DEBUG 0

// Mettere a 0 per orologio normale
// Mettere a 1 per orologio con sincronizzazione NTP
// Mettere a 2 per orologio con programma esterno per sincronizzazione
#define CLOCK_MODE 1

// Inserire il server da cui ricavare l'orario
//#define SERVER_ADDR "ntp1.inrim.it"
#define SERVER_ADDR IPAddress(193, 204, 114, 232)

#include "OrologioBinario.h"
#include "StrisciaLed.h"

OrologioBinario *orologio;

const int PIN_LED = 9;
const int PIN_BTN1 = 2;
const int PIN_BTN2 = 3;

// Memorizzo il MAC address e
// Importo le librerie necessarie alla comunicazione ethernet
// Se quest'ultima viene utilizzata
#if CLOCK_MODE == 1 || CLOCK_MODE == 2
  const byte MAC_ADDRESS[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

  #ifndef _LIB_SPI
    #define _LIB_SPI
    #include <SPI.h>
  #endif

  #ifndef _LIB_ETHERNET
    #define _LIB_ETHERNET
    #include <Ethernet.h>
  #endif
#endif

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(21, PIN_LED, NEO_GRB + NEO_KHZ800);
StrisciaLed *striscia;

int statoAttuale = -1;
uint32_t offsetOre = 0;
uint32_t offsetMinuti = 0;
uint8_t offsetSecondi = 0;

unsigned long timer;

#if CLOCK_MODE == 1 || CLOCK_MODE == 2
  EthernetUDP udp;
#endif

// Se son in modalità con programma mi metto in ascolto sulla 3306
#if CLOCK_MODE == 2
  EthernetServer server = EthernetServer(3306);
  String letturaInCorso = "";
#endif

void setup() {

  // Apro la seriale se sono in debug
  #if DEBUG
    Serial.begin(9600);
  #endif

  // Avvio la striscia LED
  pixels.begin();
  striscia =  new StrisciaLed(&pixels);
  
  // Se sto usando la libreria ethernet memorizzo il MAC address
  // L'indirizzo IP verrà richiesto tramite DHCP
  #if CLOCK_MODE == 1 || CLOCK_MODE == 2
    Ethernet.begin(MAC_ADDRESS);
         
    // Lo shield ethernet non è connesso... Se sono in modalità di debug lo scrivo sulla seriale
    // In ogni caso rimango in attessa all'infinito.
    if(Ethernet.hardwareStatus() == EthernetNoHardware) {
      
      #if DEBUG
        Serial.println("Ethernet shield non connesso!");
      #endif
      
      while(true) {
        delay(100);
      }
    }

    // Se il cavo di rete non è connesso faccio lampeggiare i minuti...
    while(Ethernet.linkStatus() == LinkOFF) {

      // Se sono in debug lo scrivo sulla seriale
      #if DEBUG
        Serial.println("Cavo ethernet non connesso!");
      #endif

      // Attendo...
      delay(200);
    }

    // Se sono in modalità di debug e sto usando l'ethernet stampo l'IP sulla seriale
    #if DEBUG
      Serial.println(Ethernet.localIP());
    #endif
  #endif

  // Orologio in modalità NTP
  #if CLOCK_MODE == 1
    // Porta per la ricezione delle risposte UDP
    udp.begin(2390);

    // Istanzio l'orologio utilizzando un server NTP
    // E imposto l'offset per il nostro fuso orario
    orologio = new OrologioBinario(SERVER_ADDR, &udp);

    // Faccio lampeggiare velocemente il primo led a dx delle ore
    // Fino a quando non ottengo 
    striscia -> SetBlinking(true, 0);
    striscia -> SetHours(1);
    striscia -> SetMinutes(0);
    striscia -> SetSeconds(0);
    
    while(orologio -> Connected()) {
      striscia -> Show();
      delay(100);
    }

  // Orologio in modalità programma
  #elif CLOCK_MODE == 2
    server.begin();
    orologio = new OrologioBinario(0);

  // Orologio classico
  #else
    orologio = new OrologioBinario(0);
  #endif

  // Dico all'Arduino che le porte saranno utilizzate in INPUT (con resistenza PULLUP integrata)
  // E collego un interrupt
  pinMode(PIN_BTN1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN1), btn1Interrupt, FALLING);

  pinMode(PIN_BTN2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN2), btn2Interrupt, FALLING);
}

void loop() {

  #if CLOCK_MODE == 2
    EthernetClient client = server.available();

    // Se è presente un client leggo
    if (client) {
          
      char tmp = client.read();
      
      // Incremento i secondi di 1
      if(tmp == 'S') {
        offsetSecondi++;
      
        if(offsetSecondi >= 60)
          offsetSecondi = 0;
      
      // Terminatore dell'orario in formato UNIX Time Stamp
      } else if(tmp != ';')
        letturaInCorso += tmp;

      // Salvo il nuovo orario
      else {
        orologio = new OrologioBinario((long)letturaInCorso.toInt());
        letturaInCorso = "";
      }
    }
  #endif

  // Aggiorno la striscia ogni 500ms
  // Non uso il delay altrimenti rallenterei la ricezione dei dati di rete
  if(millis() - timer >= 500) {
    timer = millis();
    
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

    // Se sono in debug stampo l'offset attuale
    #if DEBUG
      Serial.println("Offset: " + String((long)(offsetOre + offsetMinuti + offsetSecondi)));
    #endif
  
    // Scrivo l'offset rispetto all'orario attuale dell'Arduino
    orologio -> SetOffset((long)(offsetOre + offsetMinuti + offsetSecondi));
  
    // Scrivo l'orario sulla striscia
    striscia -> SetHours((uint8_t)orologio -> GetHours());
    striscia -> SetMinutes((uint8_t)orologio -> GetMinutes());
    striscia -> SetSeconds((uint8_t)orologio -> GetSeconds());
  
    // Invio i dati alla striscia
    striscia -> Show();
  
    // Se sono in debug scrivo l'orario attuale
    #if DEBUG
      Serial.println("Orario attuale: " + String(orologio -> GetHours()) + ":" + String(orologio -> GetMinutes()) + ":" + String(orologio -> GetSeconds()));
    #endif
  }
  
}

// Interrupt del bottone di cambio modalità
void btn1Interrupt() {
  if(statoAttuale == 2) {
    statoAttuale = -1;
  } else {
    statoAttuale++;
  }
}

// Interrupt del bottone di incremento dell'orario
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
