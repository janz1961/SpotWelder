#include "display.h" 
#include "button.h" 
#include <EEPROM.h>

// constants won't change. They're used here to set pin numbers:
const int pinButtonWeld = 2;            // the number of the pushbutton pin
const int pinButtonUp = 3;              // the number of the pushbutton pin
const int pinButtonDown = 4;            // the number of the pushbutton pin
const int pinElectrode = 11;            // the number of the LED pin
const int pinPulse = 12;                // the number of the LED pin
const int pinLED = 13;                  // the number of the LED pin

const int latency = 4;                  // Time it taks the solenoid to activate
int pulse = 140;                        // Time the solenoid stays activatged

// Variables will change:
Button buttonWeld(pinButtonWeld);
Button buttonUp(pinButtonUp);
Button buttonDown(pinButtonDown);

bool isArmed = true;

long millisDisplayLast = 0;

int waitFirst = 500;
int waitRepeat = 200;


unsigned long weldStart = 0;

Display *display;

void setup() {
  Serial.begin(115200);
  Serial.println("Start logging for " __FILE__ ". Version " __DATE__ " " __TIME__);

  display = new Display();

  // Read pulse from EEPROM
  pulse = EEPROM.read(0);
  Serial.print("Read from EEPROM. Pulse = ");
  Serial.println(pulse);
  
  Serial.print("Activation will be ");
  Serial.print(pulse + latency);
  Serial.println("ms");  
  
  pinMode(pinPulse, OUTPUT);
  pinMode(pinLED, OUTPUT);

  // set initial LED state
  digitalWrite(pinPulse, LOW);
  char buf[60];
  sprintf(buf, "Pulse: %i", pulse);
  display->Update(buf);
}

void loop() {
  char buf[60];

  long millisNow = millis();

  // First handle pulse
  int weldDuration = latency + pulse;   // TIme the solenoid is powered
  if (weldStart != 0 && (millis() - weldStart) > weldDuration) {

    Serial.println("End pulse");
    sprintf(buf, "Release!", pulse);
    weldStart = 0;
    digitalWrite(pinPulse, LOW);
    digitalWrite(pinLED, LOW);
  }

  // If nothing else happens, show current pulse setting
  sprintf(buf, "Pulse: %i", pulse);

  // First handle pulse button. 
  // Pulse button is a single action button that neds to be released before it can be used again
  // If state changed, reset debounce timer
  if (buttonWeld.Check()) {
    if (buttonWeld.IsActive()){
      if (isArmed) {
        sprintf(buf, "Button armed: start %ims pulse", pulse);
        Serial.println(buf);
        sprintf(buf, "Pulsing");
        isArmed = false;

        digitalWrite(pinPulse, HIGH);
        digitalWrite(pinLED, HIGH);
        weldStart = millis();
      }
      else {
        Serial.println("Button not armed");
        sprintf(buf, "Release!", pulse);
      }
    }
    else {
      Serial.println("Button released: Arm button");
      isArmed = true;
    }
  }
  else {
    if (!isArmed) sprintf(buf, "Release!");
  }
  
  // Handle navigation buttons
  // These buttons increase the pulse lentgth when active. 
  // If kept active for longet that the waitFirst value, they increase the value again
  // If then still kept active for waitRepeat ms, do it again
  static int countMod = 0;
  if (buttonUp.Check()) {
    if (!buttonUp.IsActive()) {
      if (countMod == 0) {
        countMod++;
        pulse++;
      }
    }
    else {
      countMod = 0;
    }
  } 
  else {
    int wait = (countMod == 1) ? 1000 : 300;
    if (buttonUp.IsActive(wait)) {
        countMod++;
        pulse++;
    }
  }

  if (buttonDown.Check()) {
    if (!buttonDown.IsActive()) {
      if (countMod == 0) {
        countMod++;
        pulse--;
      }
    }
    else {
      countMod = 0;
    }
  } 
  else {
    int wait = (countMod == 1) ? 1000 : 300;
    if (buttonDown.IsActive(wait)) {
        countMod++;
        pulse--;
    }
  }


  
  if ((millisNow - millisDisplayLast) >= 1000)
  {
    // Prevents excessive use op EEPROM
    EEPROM.update(0, pulse);
    millisDisplayLast = millisNow;
  }

  display->Update(buf);
}
