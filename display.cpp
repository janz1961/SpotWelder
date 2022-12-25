#include "Display.h"

int Display::m_DisplayCount = 0;

bool Display::Check() {
  Wire.beginTransmission(I2C_DISPLAY);

  int current = (Wire.endTransmission() == 0);
  
  bool bDisplayChanged = (m_IsPresent != current);
  m_IsPresent = current;

  if (!bDisplayChanged && !m_IsPresent) {
    Serial.println("Display still not present");
    return m_IsPresent;         // Still no display present
  }

#ifdef ALWAYSSHOWDISPLAYSTATE
  // Show display state
  Serial.print("Display ");
  if (!bDisplayChanged) Serial.print("still ");
  if (!m_IsPresent) Serial.print("dis");
  Serial.println("connected!");
#else
  // Only show when display state changed
  if (bDisplayChanged) 
  {
    Serial.print("Display still");
    if (!m_IsPresent) Serial.print("dis");
    Serial.println("connected!");
  }
#endif

  if (!m_IsPresent) return;                             // No display present

  if (bDisplayChanged)  {
    begin(SSD1306_SWITCHCAPVCC, I2C_DISPLAY);       //Start the OLED display
  }
  
  clearDisplay();

  setTextSize(2);                    
  setTextColor(WHITE, BLACK);             
  // dim(true);  
  display();

  return m_IsPresent;  
}

bool Display::IsPresent() {
  return m_IsPresent;
}

void Display::Update(char *s) {
  static char last[80] = "";

  if (!strcmp(last, s) || !Check()) return;

  strcpy(last,s);
  clearDisplay();
  setCursor(0, 10);
  print(last);

  display();
}
