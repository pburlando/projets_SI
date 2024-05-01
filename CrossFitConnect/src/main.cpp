#include <Arduino.h>

#define CaptFlex A2
#define TE 100

unsigned long mem_time = 0L;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mem_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - mem_time > TE) {
    mem_time = millis();
    int val_num = analogRead(CaptFlex);
    Serial.println(val_num);

  }
}

