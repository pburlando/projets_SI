/*
Acquisition des données d'un accéléromètre
sur carte micro_sd
Arduino Nano
*/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h> 
#include <Wire.h>
#include <MPU6050_tockn.h>


// set up variables using the SD utility library functions:
Sd2Card card;


// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 10;

const uint32_t tsample = 50; // Période d'échantillonage en ms
const uint16_t trecord = 5; // Temps d'enregistrement en seconde
const uint32_t iterations = trecord*1000/tsample; // Nombre d'enregistrement à réaliser

const int ledPin = 2;



MPU6050 mpu6050(Wire);

// Variables globales
unsigned long memTime = 0L;
unsigned long chrono = 0L;
uint16_t nb_samples;
bool ledState;
String full_path;
uint8_t nb_files = 0;
const String sep = ",\t";

// Fonctions
uint8_t countFiles(File dir);

 
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(250000);
  Wire.begin();
  mpu6050.begin();
  digitalWrite(ledPin, HIGH);
  mpu6050.calcGyroOffsets(true);  // Laisser la centrale inertielle immobile pendant le calibrage
  digitalWrite(ledPin, LOW);


  // Initialisation de la carte SD
  Serial.println(F("Initializing SD card..."));

  if (!SD.begin(chipSelect)) {
    Serial.println(F("initialization failed!"));
    while(true);
  }

  Serial.println(F("initialization done."));
  
  // Compter le nombre de fichiers à la racine de la carte SD
  File root = SD.open("/");
  nb_files = countFiles(root);
  if (nb_files == 99) {
    Serial.println(F("Too many files"));
    while (true);    
  }

  // Ecriture dans un fichier
  // Préparation de la chaine du chemin complet du fichier
  String file_name = F("/Acq");
  String extension = F(".txt");
  full_path = file_name + nb_files + extension;

  // Préparation de la chaine qui indique la période échantillonage
  String te = F("Te = ");
  te += tsample;
  te += F(" ms");

  Serial.println(te);

  // Préparation de la chaine d'en-tête
  String header = F("Num, \t AccX(G), \t AccY, \t AccZ, \t gyroX(deg/s), \t gyroY, \t gyroZ, \t, angleX(deg), \t angleY, \t angleZ, \t temp(degC) ");
  Serial.print("filename: ");
  Serial.println(full_path.c_str());

  File monFichier = SD.open(full_path.c_str(), FILE_WRITE); //Attention Nom de fichier format 8.3 (8 caractères pour le nom et 3 pour l'extension pas d'espace ...) 
  if(monFichier) {
    monFichier.println(te);
    monFichier.println(header);
    monFichier.close();
  }
  else {
    Serial.println(F("Erreur IO"));
  }
  memTime = millis();
  chrono = memTime;
}
 
void loop() {
  
  if(millis() - memTime > tsample){
    memTime = millis();
    String ligne;
    mpu6050.update();
    ligne += nb_samples;
    ligne += sep;
    ligne += mpu6050.getAccX();
    ligne += sep;
    ligne += mpu6050.getAccY();
    ligne += sep;
    ligne += mpu6050.getAccZ();
    ligne += sep;
    ligne += mpu6050.getGyroX();
    ligne += sep;
    ligne += mpu6050.getGyroY();
    ligne += sep;
    ligne += mpu6050.getGyroZ();
    ligne += sep;
    ligne += mpu6050.getAngleX();
    ligne += sep;
    ligne += mpu6050.getAngleY();
    ligne += sep;
    ligne += mpu6050.getAngleZ();
    ligne += sep;
    ligne += mpu6050.getTemp();  
    Serial.println(ligne);
    File monFichier = SD.open(full_path.c_str(), FILE_WRITE);
    if(monFichier) {
      monFichier.println(ligne);
      monFichier.close();
    }
    else {
      Serial.println(F("Erreur IO"));
    }
    
    if ((nb_samples%10) == 0) {
      // Tous les 10 échantillons changer l'état de la diode
      digitalWrite(ledPin, ledState);
      ledState = !ledState;
    }
    
    nb_samples++;
  }

  if (nb_samples == iterations) {
    unsigned long record_time = millis() - chrono;
    Serial.print(F("Recording time "));
    Serial.print(record_time);
    Serial.println(F(" ms."));
    digitalWrite(ledPin, 0);
    while(true);
  }

}

/************************************************
 * Fonction pour compter le nombre de fichiers
 * à la racine de la carte SD
 * **********************************************/

uint8_t countFiles(File dir) {
  // Retourne le nombre de fichier d'un dossier
  uint8_t nbFiles = 0;
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      return nbFiles;
    }

    if (entry.isDirectory() == 0) {
      // entry is a file
      nbFiles++;
    } 
    entry.close();
  }
}