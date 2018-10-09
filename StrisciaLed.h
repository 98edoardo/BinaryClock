#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class StrisciaLed {
  
  public:
    StrisciaLed(Adafruit_NeoPixel *pixels);
    void SetHours(uint8_t ore);
    void SetMinutes(uint8_t minuti);
    void SetSeconds(uint8_t secondi);
    void Show();
    void SetBlinking(bool blinking, int pos);

  private:
    int _ore = 0;
    int _minuti = 0;
    int _secondi = 0;

    Adafruit_NeoPixel *_pixels = NULL;
    
    bool _toDraw[3][7] = {
      { false, false, false, false, false, false, false },
      { false, false, false, false, false, false, false },
      { false, false, false, false, false, false, false }
    };

    bool _blinking[3] = { false, false, false };
    bool _rawBlinking[3] = { false, false, false };

    static const bool READ_MAP[];

    // Colori per i bit accesi
    uint32_t _coloreOreOn;
    uint32_t _coloreMinutiOn;
    uint32_t _coloreSecondiOn;

    // Colori per i bit spenti
    uint32_t _coloreOreOff;
    uint32_t _coloreMinutiOff;
    uint32_t _coloreSecondiOff;

    void CalcolaLED(uint8_t num, bool reverse, bool *out);
};
