#include <Arduino.h>
//#include <ESP8266WiFi.h>
//#include <WiFiUdp.h>

#define NTP_PACKET_SIZE 48

class OrologioBinario {
  public:
    // Doppia possibilità di inzializzazione
//    OrologioBinario(String server, WiFiUDP *udp); // Da server NTP
    OrologioBinario(long start);    // Utilizzando un valore preimpostato
    
    void SetOffset(long offset);    // Se si utilizza un server NTP può essere comodo un offset per cambiare fuso orario
    long GetOffset();               // Metodo che restituisce l'offset impostato
 //   String GetNTPServer();
    int32_t GetActualTimeStamp();       // Calcola e restituisce il timestamp attuale
    int GetHours();
    int GetMinutes();
    int GetSeconds();
  //  void Sync();
    
  private:
    long _offsetIniziale;  // Memorizzo la distanza tra l'accensione del device con quello dell'impostazione dell'orario
    long _offsetUtente;
    long _start;           // TS inizio
    String _server;        // Server utilizzato dall'utente
//    IPAddress _serverIp;
//    WiFiUDP* _udp;
    byte _packetBuffer[NTP_PACKET_SIZE];

//    void SendNTPpacket(); // Metodo per inviare
};
