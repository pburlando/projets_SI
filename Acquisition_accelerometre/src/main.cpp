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




File monFichier;

MPU6050 mpu6050(Wire);

long timer = 0;

 
void setup() {
  char buffer[80]; // Pour stocker les valeurs à écrire dans la carte SD

  Serial.begin(9600); //Vitesse de transmission

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // Initialisations
  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }


 

  
  // Ecriture dans un fichier
  

  
  SD.begin();

  monFichier = SD.open("testEc.txt", FILE_WRITE); //Attention Nom de fichier format 8.3 (8 caractères pour le nom et 3 pour l'extension pas d'espace ...)
  if(monFichier) {
    int avant = millis();
    // Acquisition des 1024 valeurs
    for(int i = 0; i < 1024; i++) { 
      
      mpu6050.update();

      if(millis() - timer > 20){
        
        // Préparer une chaine de caractère avec Sprintf enregistrer les valeurs brutes en hexadécimal
        sprintf(buffer, "%d \t %d \t %d", mpu6050.getRawAccX(), mpu6050.getRawAccY(), mpu6050.getRawAccZ() );
        Serial.println(buffer);
        monFichier.println(buffer);
        timer = millis();
        
      }

    }
    int temps = millis() - avant;
    monFichier.close(); // Fermer le fichier
    Serial.print(F("1ko ecrit en "));
    Serial.print(temps);
    Serial.println(" ms");
  }
  else {
    Serial.println("Erreur d'écriture");
  }

  
  
  
  //Lecture du fichier
//  Serial.println("Tentavive de lecture");
//  if(monFichier = SD.open("testEc.txt",FILE_READ)) { 
//    while(monFichier.available()) {
//      Serial.write(monFichier.read());  
//    }
//   
//    monFichier.close(); // Fermer le fichier
//  }
//  else
//    Serial.println("Erreur de lecture");
    

  
}
 
void loop() {
  

}