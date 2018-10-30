#include "StrisciaLed.h"

// Mappatura delle righe
// Se parte da sinistra -> false
// Se parte da destra -> true
const bool StrisciaLed::READ_MAP[] = { false, true, false };

StrisciaLed::StrisciaLed(Adafruit_NeoPixel *pixels) {
  _pixels = pixels;
  
  _coloreOreOn = _pixels -> Color(0, 0, 255, 255);
  _coloreMinutiOn = _pixels -> Color(0, 255, 0, 255);
  _coloreSecondiOn = _pixels -> Color(255, 0, 0, 255);
  
  _coloreOreOff = _pixels -> Color(10, 10, 10, 255);
  _coloreMinutiOff = _pixels -> Color(10, 10, 10, 255);
  _coloreSecondiOff = _pixels -> Color(10, 10, 10, 255);
}

void StrisciaLed::SetHours(uint8_t ore) {
  CalcolaLED(ore, READ_MAP[0], _toDraw[0]);
}

void StrisciaLed::SetMinutes(uint8_t minuti) {
  CalcolaLED(minuti, READ_MAP[1], _toDraw[1]);
}

void StrisciaLed::SetSeconds(uint8_t secondi) {
  CalcolaLED(secondi, READ_MAP[2], _toDraw[2]);
}

void StrisciaLed::CalcolaLED(uint8_t num, bool reverse, bool *out) {
  String tmp = String(num, BIN);

  for(int i = tmp.length(); i < 7; i++)
    tmp = '0' + tmp;
    
  if(!reverse) {
    int j = 6;
    for(int i = 0; i <= 6; i++) {
         out[j--] = tmp[i] == '1';
    }
  } else {
    for(int i = 0; i <= 6; i++) {
         out[i] = tmp[i] == '1';
    }
  }
}

void StrisciaLed::SetBlinking(bool blinking, int pos) {
  _blinking[pos] = blinking;
}

void StrisciaLed::Show() {

  int i = 21;
  
  uint32_t accesi[] = { _coloreOreOn, _coloreMinutiOn, _coloreSecondiOn };
  uint32_t spenti[] = { _coloreOreOff, _coloreMinutiOff, _coloreSecondiOff };

  for(int i = 0; i < 3; i++) {
    if(_blinking[i])
      _rawBlinking[i] = !_rawBlinking[i];
  }
  
  for(int j = 2; j >= 0; j--) {
    for(int k = 0; k < 7; k++) {

      uint32_t coloreAttuale;
      
      if(_blinking[j] && _rawBlinking[j])
        coloreAttuale = (_toDraw[j][k] == 1) ? spenti[j] : accesi[j];
      
      else if(_toDraw[j][k] == 1 && !_blinking[j])
        coloreAttuale = accesi[j];

      else
        coloreAttuale = spenti[j];
      //  Serial.println(_toDraw[j][k]);
      _pixels -> setPixelColor(--i, coloreAttuale);
    }
  }
    
  _pixels -> show();
}
