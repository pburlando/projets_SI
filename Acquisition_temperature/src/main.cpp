/*
DataLogger température arduino nano

Enregistrement de température et hygrométrie
capteur DHT22
sur dataLogger Arduino Nano, carte SD et RTC DS1307

*/


#include <Arduino.h>

// Bibliothèque et initialisation capteur DHT22
#include <SimpleDHT.h>

int pinDHT22 = 10;
SimpleDHT22 dht22(pinDHT22);

// Bibliothèque et initialisation horloge DS1307
#include "RTClib.h"

RTC_DS1307 rtc;

// Bilbliothèque et initialisation carte micro SD
#include <SPI.h>
#include <SD.h>

File myFile;

long mem_time = 0L;

void setup() {
  // Démarrage liaison série
  Serial.begin(9600);

  // Démarrage et mise à l'heure RTC DS1307
  if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      while (1) delay(10);
    }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  DateTime now = rtc.now();
  char date[] = "YY-MM-DD";
  char time[] = "hh:mm:ss";
  String record_date = now.toString(date);
  String record_time = now.toString(time);
  String line = String("Record on " + record_date + " at " + record_time + " local time.");
  Serial.println("------");
  Serial.println(line);
  
  mem_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // formatage de la chaine d'horodatage des échantillons 
  String date = ""; 
  DateTime now = rtc.now();
  char sample_date[] = "YYYY/MM/DD, hh:mm:ss";


  
  // read without samples.
  // @remark We use read2 to get a float data, such as 10.1*C
  //    if user doesn't care about the accurate data, use read to get a byte data, such as 10*C.
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;

  String ligne; // Ligne de données à afficher
  ligne = "";
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DHT22 failed, err="); Serial.print(SimpleDHTErrCode(err));
    //Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(2000);
    ligne += now.toString(sample_date);
    ligne += ", ";
    ligne += "Read DHT22 failed, err = ";
    ligne += SimpleDHTErrCode(err);
    ligne += ", ";
    ligne += SimpleDHTErrDuration(err);
  }
  else {
    ligne += now.toString(sample_date);
    ligne += ", ";
    ligne += ((float)temperature);
    ligne += ", ";
    ligne += ((float)humidity);
  }
  
  //Serial.print((float)temperature); Serial.print(" *C, ");
  //Serial.print((float)humidity); Serial.println(" RH%");
  Serial.println(ligne);
  
  // DHT22 sampling rate is 0.5HZ.
  delay(2500);
}