#include <Arduino.h>
#include "RGBConverter.h"


const int tmp236_pin = A0;
const int red_pin = 6;
const int green_pin = 5;
const int blue_pin = 3;
int val_a0;

RGBConverter color;

void set_color(uint8_t red, uint8_t green, uint8_t blue);

void setup()
{
	Serial.begin(9600);
  
}

void loop()
{
  byte rgb[3];
  double saturation = 0.9;
  double value = 0.3;
  val_a0 = analogRead(tmp236_pin);
  float voltage = val_a0 * 5.0 / 1024.0;
  float temperature = 51.27 * voltage - 20.51;
  // double hue = -0.01714 * temperature + 0.6; // centré sur 0 - 35 °C
  double hue = -0.014198 * temperature + 0.497; // pleine échelle de température 
  color.hsvToRgb(hue, saturation, value , rgb);
  set_color(rgb[0], rgb[1], rgb[2]);
  Serial.println("TMP236: ");
  Serial.println(val_a0);
  Serial.println(voltage);
  Serial.println(temperature);
  Serial.println(String(hue) + ", " + String(saturation) + ", " + String(value) + "," + String(rgb[0]) + ", " + String(rgb[1]) + ","  + String(rgb[2]));
  delay(1000);
}

void set_color(uint8_t red, uint8_t green, uint8_t blue)
{
  // Set the color using the provided RGB values
  analogWrite(red_pin, red);
  analogWrite(green_pin, green);
  analogWrite(blue_pin, blue);
}