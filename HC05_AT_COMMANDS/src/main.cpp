/**
 * @brief - Envoyer des commandes AT sur un module BT HC05 maître via un moniteur série
 *        - Recevoir les réponses du ou des modules BT HC05 esclaves sur le moniteur série 
 * 
*/
#include <Arduino.h>
#include <SoftwareSerial.h>
const int keyPin = 5; // key à brancher sur le module HC05

SoftwareSerial mySerial(10, 11); // RX, TX arduino à brancher au TX, RX sur le module HC05 (lignes croisées)

void setup() 
{
  
  pinMode(keyPin, OUTPUT);
  digitalWrite(keyPin, LOW);
  delay(1000); 
  Serial.begin(9600); // Port série connecté entre un PC avec moniteur série et un arduino 
  Serial.println("Enter AT commands:"); 
  mySerial.begin(9600);  // Port série connecté entre un arduino et un module HC05
  digitalWrite(keyPin, HIGH); // Passage en commande AT logiciellement Datasheet HC05 page 1 How to get to the AT mode way 1 

}

void loop()
{

  if (mySerial.available())   // S'il y a des données dans le buffer du port arduino-HC05  
    Serial.write(mySerial.read()); //Lire les données du port arduino-HC05 et les écrires sur le port Arduino-PC
  
  if (Serial.available())         // S'il y a des données dans le buffer du port Arduino-PC
    mySerial.write(Serial.read());  // Lire les données du port Arduino-PC et les écrires sur le port Arduino-HC05

}