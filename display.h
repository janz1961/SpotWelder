#ifndef _Display_H_
#define _Display_H_


#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_DISPLAY 0x3C

//////// Display Class

class Display : public Adafruit_SSD1306 {
  public:
    Display() : Adafruit_SSD1306 (128, 32, &Wire, -1)
    {
      Serial.println("Create display");

      m_IsPresent = false;
      Wire.begin();
      if (m_DisplayCount > 0)
      {
        Serial.println("Only one Display instance allowed");
        delete this;
        return;
      }
      
      m_DisplayCount++;

      if (!Check())
      {
        Serial.println("No display available");
        return;
      }
    }

    ~Display() {
      m_DisplayCount--;
    }

    virtual void Update(char *s);
    
    bool Check();
    bool IsPresent();

  private:
    static int m_DisplayCount;
    
    long m_MillisConnected;
    long m_MillisUpdate;
    bool m_IsPresent = false;
    
};

#endif