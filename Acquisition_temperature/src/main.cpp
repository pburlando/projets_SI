/*
DataLogger température arduino nano

Enregistrement de température et hygrométrie
capteur DHT22
sur dataLogger Arduino Nano, carte SD et RTC DS1307

*/

#define SAMPLE_DURATION  5000

#include <Arduino.h>

// Bibliothèque et initialisation capteur DHT22
#include <SimpleDHT.h>

int pinDHT22 = 11;
SimpleDHT22 dht22(pinDHT22);

// Bibliothèque et initialisation horloge DS1307
#include "RTClib.h"

RTC_DS1307 rtc;

// Bilbliothèque et initialisation carte micro SD
#include <SPI.h>
#include <SD.h>

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

  // Initialisation de la carte SD
  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  DateTime now = rtc.now();
  char date[] = "YY-MM-DD";
  char time[] = "hh:mm:ss";
  String record_date = now.toString(date);
  String record_time = now.toString(time);
  String line = String("Record on " + record_date + " at " + record_time + " local time.");
  Serial.println("------");
  Serial.println(line);

  // Creation d'un dossier sur la carte SD
  //SD.mkdir(record_date);
  
  mem_time = millis();
}

void loop() {

  if ((millis() - mem_time) > SAMPLE_DURATION) {

    // formatage de la chaine d'horodatage des échantillons 
    String date = ""; 
    DateTime now = rtc.now();
    char sample_date[] = "YYYY/MM/DD, hh:mm:ss";

    // Lecture du capteur DHT22
    float temperature = 0;
    float humidity = 0;
    int err = SimpleDHTErrSuccess;

    String line; // Ligne de données à afficher
    line = "";
    if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      //Serial.print("Read DHT22 failed, err="); Serial.print(SimpleDHTErrCode(err));
      //Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(2000);
      line += now.toString(sample_date);
      line += ", ";
      line += "Read DHT22 failed, err = ";
      line += SimpleDHTErrCode(err);
      line += ", ";
      line += SimpleDHTErrDuration(err);
    }
    else {
      line += now.toString(sample_date);
      line += ", ";
      line += ((float)temperature);
      line += ", ";
      line += ((float)humidity);
    }

    File dataFile = SD.open("record.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(line);
      dataFile.close();
      Serial.println(line);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening record.txt");
    }
    mem_time = millis();
  }
  
}