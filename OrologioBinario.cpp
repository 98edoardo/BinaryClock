#include "OrologioBinario.h"
/*
OrologioBinario::OrologioBinario(String server, WiFiUDP *udp) {
  _server = server;
  char tmp[_server.length() + 1]; 
  _server.toCharArray(tmp, _server.length() + 1);
  _udp = udp;
  
  WiFi.hostByName(tmp, _serverIp); // Ricavo l'indirizzo IP del server, se ne è presente più di uno questo viene scelto casualmente

  // Nel caso non fossi riuscito a risolvere l'host
  // Utilizzo un server dell'INRIM
  if(_serverIp == IPAddress(0, 0, 0, 0))
    _serverIp = IPAddress(193, 204, 114, 105);
  
  SendNTPpacket();
  
  delay(1000);
  
  // Se non ricevo nessuna risposta,
  // Continuo a provare :-)
  while(!_udp -> parsePacket()) {
    SendNTPpacket();
    delay(1000);
  }
  
  // Risposta ricevuta :-D
  _udp -> read(_packetBuffer, NTP_PACKET_SIZE);

  // Il dato è diviso in due word (se non sbaglio è questione di retrocompatibilità)
  // Le unisco e memorizzo l'offset dall'avvio per calcoli futuri
  unsigned long highWord = word(_packetBuffer[40], _packetBuffer[41]);
  unsigned long lowWord = word(_packetBuffer[42], _packetBuffer[43]);
  _start = (highWord << 16 | lowWord) - 2208988800UL;
  _offsetIniziale = millis();
}

*/
OrologioBinario::OrologioBinario(long start) {
  _server = "";
  _start = start;
  _offsetIniziale = millis();
}
/*
void OrologioBinario::SendNTPpacket() {
  memset(_packetBuffer, 0, NTP_PACKET_SIZE);
  
  // Tratto dagli esempi :-) Bisogna altrimenti approfondire il protocollo NTP
  // ** Initialize values needed to form NTP request **
  // (see URL above for details on the packets)
  _packetBuffer[0] = 0xE3;   // LI, Version, Mode
  _packetBuffer[1] = 0;     // Stratum, or type of clock
  _packetBuffer[2] = 6;     // Polling Interval
  _packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  _packetBuffer[12]  = 49;
  _packetBuffer[13]  = 0x4E;
  _packetBuffer[14]  = 49;
  _packetBuffer[15]  = 52;

  _udp -> beginPacket(_serverIp, 123);
  _udp -> write(_packetBuffer, NTP_PACKET_SIZE);
  _udp -> endPacket();
}

String OrologioBinario::GetNTPServer() {
  return _server;
}
*/
long OrologioBinario::GetOffset() {
  return _offsetUtente;
}

void OrologioBinario::SetOffset(long offset) {
  _offsetUtente = offset;
}

int32_t OrologioBinario::GetActualTimeStamp() {
  return _start + (millis() - _offsetIniziale) / 1000;
}

int OrologioBinario::GetHours() {
   return ((_start + _offsetUtente + (millis() - _offsetIniziale) / 1000)  % 86400L) / 3600;
}

int OrologioBinario::GetMinutes() {
   return ((_start + _offsetUtente + (millis() - _offsetIniziale) / 1000)  % 3600) / 60;
}

int OrologioBinario::GetSeconds() {
   return (_start + _offsetUtente + (millis() - _offsetIniziale) / 1000) % 60;
}
/*
void OrologioBinario::Sync() {
  if(_server == "")
    return;
  
  SendNTPpacket();
  
  delay(1000);
  
  // Se non ricevo nessuna risposta,
  // Continuo a provare :-)
  while(!_udp -> parsePacket()) {
    SendNTPpacket();
    delay(1000);
  }
  
  // Risposta ricevuta :-D
  _udp -> read(_packetBuffer, NTP_PACKET_SIZE);

  // Il dato è diviso in due word (se non sbaglio è questione di retrocompatibilità)
  // Le unisco e memorizzo l'offset dall'avvio per calcoli futuri
  unsigned long highWord = word(_packetBuffer[40], _packetBuffer[41]);
  unsigned long lowWord = word(_packetBuffer[42], _packetBuffer[43]);
  _start = (highWord << 16 | lowWord) - 2208988800UL;
  _offsetIniziale = millis();
}
*/
