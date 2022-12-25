#include "arduino.h"
#include "button.h"

bool Button::Check() {
  bool state = (digitalRead(m_Pin) == HIGH);

  Serial.print("Before: ");
  Serial.print(m_State);
  Serial.print(" - ");
  Serial.println(state);

  bool isChanged = (state != m_State);

  if (isChanged) {
    m_MillisChange = millis();
  }  
  
  m_State = state;
  Serial.print("After: ");
  Serial.print(m_State);
  Serial.print(" - ");
  Serial.print(state);
  Serial.print(" - ");
  Serial.println(isChanged);
  return isChanged;
}