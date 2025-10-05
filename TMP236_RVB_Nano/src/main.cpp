/* Arduino Nano ATmega328P  
   Program to read temperature from a TMP236 sensor and display it using an RGB LED.
   The color of the LED changes based on the temperature reading.
   The TMP236 outputs 750mV at 0°C and increases by 10mV per °C.
   The RGB LED is connected to pins 9 (Blue), 10 (Green), and 11 (Red).
   The TMP236 sensor is connected to analog pin A0.
   Vcc = 4.95V (from a Vbat=7.4V Li-Po battery connected to Arduino nanoVin) or 4.74V from USB power supply.
   Ibat = 40 mA @ Vbatt = 8V @ value = 0.3

  Add OLED display SSD1306 128x32 via I2C to show temperature value.
    Connections:
    OLED    Arduino
    VCC     VCC (5V)
    GND     GND
    SCL     A5 (SCL)
    SDA     A4 (SDA)

  Warning : RGB LED close to TMP236 maycause temperature reading to be higher than actual temperature.
*/

#include <Arduino.h>

/* lib to convert HSV to RGB */
#include "RGBConverter.h"

/* lib to drive I2C OLED 132x32display */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Create display object

/* Pin declarations */
const int tmp236_pin = A2;  // Analog pin connected to TMP236 output
const int red_pin = 11; // red pin of the RGB LED
const int green_pin = 10; // green pin of the RGB LED
const int blue_pin = 9; // blue pin of the RGB LED

/*global variables*/
int val_a0; // to store the analog value read from TMP236
long previousMillis = 0L; // to store the last time temperature was read in milliseconds
const long interval = 1000L; // Interval between readings (in milliseconds)

RGBConverter color; // Create an RGBConverter object

// function prototypes
void set_color(uint8_t red, uint8_t green, uint8_t blue); // Function to set the RGB LED color
void afficher_temperature(float temperature);  // Function to display the temperature on the OLED screen

/* Initializations */
void setup()
{
	Serial.begin(9600);  // Initialize serial communication at 9600 bits per second

  /* Start OLED initialization
    SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally */
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  delay(200); // Pause for 0.2 seconds
  display.clearDisplay();
  previousMillis = millis(); 
}

/* Main loop */
void loop()
{
  /* Read temperature and update LED color every 'interval' milliseconds */
  if(millis() - previousMillis >= interval) {
    previousMillis = millis();  // Save the last time temperature was read
    byte rgb[3];  // local array to hold RGB values
    double saturation = 0.9;  // saturation of the LED between 0 and 1
    double value = 0.3;       // brightness of the LED between 0 and 1
    val_a0 = analogRead(tmp236_pin);  // Read the analog value from TMP236
   
    float voltage = val_a0 * 4.95 * 1000 / 1024.0;  // Convert the analog reading to voltage (mV) Power supply Vcc=4.95V have to adjust according to measured voltage
    float temperature = (10 * voltage - 4000) / 195.0; // to calculate temperature in °C from voltage in mV for TMP236

    double hue = -0.014198 * temperature + 0.497; // to calculate hue from temperature
    color.hsvToRgb(hue, saturation, value , rgb); // to calculate RGB values from HSV values
    set_color(rgb[0], rgb[1], rgb[2]); // Set the RGB LED color

    /* Print values to the serial monitor for debugging */
    Serial.println("TMP236: ");
    Serial.println(val_a0);
    Serial.println(voltage);
    Serial.println(temperature);
    Serial.println(String(hue) + ", " + String(saturation) + ", " + String(value) + "," + String(rgb[0]) + ", " + String(rgb[1]) + ","  + String(rgb[2]));

    afficher_temperature(temperature);  // Display temperature on the OLED screen
  }
}

void set_color(uint8_t red, uint8_t green, uint8_t blue)
{
  // Set the color using the provided RGB values
  analogWrite(red_pin, red);
  analogWrite(green_pin, green);
  analogWrite(blue_pin, blue);
}

void afficher_temperature(float temperature) {
  // Display temperature on the OLED screen
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.print("T=");
  display.print(temperature,1);
  display.print(" ");
  display.print((char)9);  //display degree symbol
  display.println("C");
  display.display();
}