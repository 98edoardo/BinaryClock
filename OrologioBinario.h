#ifndef _LIB_ARDUINO
  #define _LIB_ARDUINO
  #include <Arduino.h>
#endif

#ifndef _LIB_ETHERNETUDP
  #define _LIB_ETHERNETUDP
  #include <EthernetUdp.h>
#endif

#ifndef _LIB_DNS
  #define _LIB_DNS
  #include <Dns.h>
#endif

#define NTP_PACKET_SIZE 48

class OrologioBinario {
  public:
    // Doppia possibilità di inzializzazione
    OrologioBinario(String server, EthernetUDP *udp); // Da server NTP (dominio)
    OrologioBinario(IPAddress server, EthernetUDP *udp); // Da server NTP (ip)
    OrologioBinario(long start);    // Utilizzando un valore preimpostato
    
    void SetOffset(long offset);    // Se si utilizza un server NTP può essere comodo un offset per cambiare fuso orario
    long GetOffset();               // Metodo che restituisce l'offset impostato
    String GetNTPServer();
    int32_t GetActualTimeStamp();       // Calcola e restituisce il timestamp attuale
    int GetHours();
    int GetMinutes();
    int GetSeconds();
    bool Connected();
    
  private:
    long _offsetIniziale;  // Memorizzo la distanza tra l'accensione del device con quello dell'impostazione dell'orario
    long _offsetUtente;
    long _start;           // TS inizio
    String _server;        // Server utilizzato dall'utente
    IPAddress _serverIp;
    EthernetUDP* _udp;
    byte _packetBuffer[NTP_PACKET_SIZE];

    void SendNTPpacket(); // Metodo per inviare
};
