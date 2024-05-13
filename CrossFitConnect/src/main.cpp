#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

MAX30105 particleSensor;

#define CaptFlex A2
#define TE 100

const byte RATE_SIZE = 8; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
bool heartSensorDetected = false;
bool fingerDetected = false;

unsigned long mem_time = 0L;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
    heartSensorDetected = false;
    }
  else
    {
    heartSensorDetected = true;
    particleSensor.setup(); //Configure sensor with default settings
    particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
    particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
    }
  mem_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  long irValue;
  if (heartSensorDetected) {
    irValue = particleSensor.getIR();
  
    if (checkForBeat(irValue) == true)
    {
      //We sensed a beat!
      long delta = millis() - lastBeat;
      lastBeat = millis();
  
      beatsPerMinute = 60 / (delta / 1000.0);
  
      if (beatsPerMinute < 255 && beatsPerMinute > 20)
      {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable
  
        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }
  }
  
  if (irValue < 50000)
    fingerDetected = false;
  else
    fingerDetected = true;
    
  if (millis() - mem_time > TE) {
    mem_time = millis();
    int val_num = analogRead(CaptFlex);
    if(fingerDetected & (beatAvg > 30)  & (beatAvg < 220))
      Serial.print(beatAvg);
    else Serial.print("-1");
    Serial.print(",");
    Serial.println(val_num);

  }
}

