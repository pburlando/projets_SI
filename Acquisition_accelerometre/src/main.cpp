/*
Acquisition des données d'un accéléromètre
sur carte micro_sd
*/

#include <Arduino.h>
#include <Wire.h>  // nécessaire pour communiquer avec la centrale inertielle par le bus I2C

#define ADRESSE_MPU 0x68  // Adresse I2C MPU

#define ROLL    0
#define PITCH   1
#define YAW     2

#define X       0
#define Y       1
#define Z       2

int accel_raw[3] = {0, 0, 0};
int gyro_raw[3] = {0, 0, 0};

int temperature = 0;




void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  TWBR = 12;
  
}

void loop() {
  // put your main code here, to run repeatedly:
}