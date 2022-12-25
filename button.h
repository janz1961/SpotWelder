#ifndef _BUTTON_H_ 
#define _BUTTON_H_ 

#include <arduino.h>

class Button {
  public:
    Button(int pin, bool needsRelease = false) : m_Pin(pin), m_MillisChange(0) {
      pinMode(m_Pin, INPUT);
      m_State = digitalRead(m_Pin);
    }

    ~Button() {}

  bool Check();

  bool IsActive(int ms = 0) {
    long millisNow = millis();
    if (ms == 0) {
      m_MillisRequest = millisNow;
      return m_State;
    }

    if ((millisNow - m_MillisRequest) < ms) return false;

    m_MillisRequest = millisNow;
    return m_State;
  }

  long GetActiveTime() {
    return millis() - m_MillisChange;
  }

  private:
    int m_Pin;                    // hardware pin
    bool m_State;                 // state at last Check()
    long m_MillisChange;          // time of last change
    long m_MillisRequest;         // time of last request
};

#endif