#include "Arduino.h"
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>
#include "LowPower.h"

#define POTENTIOMETER 0
#define BUSY_PIN 4
#define POWER_PIN 5

class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
  }
  static void OnPlayFinished(uint16_t track)
  {
  }
  static void OnCardOnline(uint16_t code)
  {
  }
  static void OnCardInserted(uint16_t code)
  {
  }
  static void OnCardRemoved(uint16_t code)
  {
  }
};

SoftwareSerial secondarySerial(2, 3); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);
long busy_counter = 0;

void setup()
{
  mp3.begin();
  pinMode(BUSY_PIN, INPUT);
  pinMode(POWER_PIN, OUTPUT);
  delay(500);
  mp3.setVolume(0);
}

void loop()
{
  int potentiometer = 0;
  int volume;

  digitalWrite(POWER_PIN, HIGH);

  potentiometer = analogRead(POTENTIOMETER); //0 - 1023

  // Speaker to powerful, setting max volume to 20. DFMiniMp3 volume range is 0 - 30.
  //volume = (int)(((float)potentiometer - 0.0)*(30.0 - 0.0)/(1023.0 - 0.0) + 0.0);
  volume = (int)(((float)potentiometer - 0.0)*(20.0 - 0.0)/(1023.0 - 0.0) + 0.0);

  mp3.setVolume(volume);

  if (digitalRead(BUSY_PIN) == 0)
  {
    busy_counter = 0;
  }
  else
  {
    busy_counter++;
  }

  if (busy_counter > 1200 ) // ((5 * 60 * 1000) / 250) 5 min
  {
    mp3.sleep();
    digitalWrite(POWER_PIN, LOW);
    delay(200);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  }

  LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
}
