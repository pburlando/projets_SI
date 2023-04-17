/*
Acquisition des données d'un accéléromètre
sur carte micro_sd
*/

#include <Arduino.h>
#include <SPI.h> //Librairie
#include <SD.h> //Librairie
#include <Wire.h>
#include <MPU6050_tockn.h>

// set up variables using the SD utility library functions:
Sd2Card card;


// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 10;

const uint32_t tsample = 20;
const uint16_t trecord = 20;
const uint32_t iterations = trecord*1000/tsample;



String full_path;

MPU6050 mpu6050(Wire);

long memTime = 0L;
uint8_t nb_files = 0;

uint16_t nb_samples;

bool ledState;

uint8_t countFiles(File dir);
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600); //Vitesse de transmission

  Wire.begin();
  mpu6050.begin();
  //digitalWrite(LED_BUILTIN, HIGH);
  //mpu6050.calcGyroOffsets(true);  // Laisser la centrale inertielle immobile pendant le calibrage
  //digitalWrite(LED_BUILTIN, LOW);


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
    Serial.println("Too many files");
    while (true);    
  }

// Ecriture dans un fichier
  String file_name = "/Acq";
  String extension = ".txt";
  full_path = file_name + nb_files + extension; 
  Serial.print("filename: ");
  Serial.println(full_path.c_str());

  File monFichier = SD.open(full_path.c_str(), FILE_WRITE); //Attention Nom de fichier format 8.3 (8 caractères pour le nom et 3 pour l'extension pas d'espace ...) 
  if(monFichier) {
    monFichier.println("Demarrage...");
    monFichier.close();
  }
  else {
    Serial.println("Erreur IO");
  }
}
 
void loop() {
  String ligne;
  String sep =",\t";
  if(millis() - memTime >= tsample){
    memTime = millis();
    mpu6050.update();
    // Préparer une chaine de caractère avec Sprintf enregistrer les valeurs brutes en hexadécimal
    //sprintf(line, "%d \t %d \t %d \t %d", nb_samples, mpu6050.getRawAccX(), mpu6050.getRawAccY(), mpu6050.getRawAccZ() );
 
    ligne += nb_samples;
    ligne += sep;
    ligne += mpu6050.getAccX();
    ligne += sep;
    ligne += mpu6050.getAccY();
    ligne += sep;
    ligne += mpu6050.getAccZ();

    Serial.println(ligne);
    File monFichier = SD.open(full_path.c_str(), FILE_WRITE);
    if(monFichier) {
      monFichier.println(ligne);
      monFichier.close();
    }
    else {
      Serial.println("Erreur IO");
    }
    
    nb_samples++;
  }

  if (nb_samples%10 == 0) {
    // Tous les 10 échantillons
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }

  if (nb_samples == iterations) {
    while(true);
  }

}


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