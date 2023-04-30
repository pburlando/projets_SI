/**
 * @brief Programme de test pour récupérer et traiter la réponse
 * à la commande AT+INQ de deux modules bluetooth esclaves
 * Le programme attend une chaine de caractères  
 * 
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
const int keyPin = 5; // key à brancher sur le module HC05

SoftwareSerial mySerial(10, 11); // RX, TX arduino à brancher au TX, RX sur le module HC05 (lignes croisées)

const int adresse_module_bt1 = 0x51;
const int adresse_module_bt2 = 0x91;

int rssi1; // Niveau rssi du module 1 (-1 si aucune réception)
int rssi2;

unsigned int hex_to_int(const char* chaine);

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
  if(Serial.available()) 
  {
    // Réception de la réponse des modules BT
    String chaine_recu = Serial.readStringUntil('\n');
    Serial.println("Chaine recu");
    Serial.println(chaine_recu);
    if(chaine_recu.lastIndexOf("OK") != -1) 
    {
      // Si la réponse se termine par "OK"
      if(chaine_recu.indexOf("+INQ", 10) != -1)
      // Si la chaine reçu contient la réponse des 2 modules
      {
        int adresse_lue_bt1 = hex_to_int(chaine_recu.substring(10, 12).c_str());
        int adresse_lue_bt2 = hex_to_int(chaine_recu.substring(43, 45).c_str());
        rssi1 = hex_to_int(chaine_recu.substring(27, 29).c_str());
        rssi2 = hex_to_int(chaine_recu.substring(60, 62).c_str());
        if(adresse_lue_bt1 == adresse_lue_bt2) 
        {
          // Si les adresses sont identiques
          if(adresse_lue_bt1 == adresse_module_bt1)
          {
            // le module 2 n'a pas répondu
            rssi2 = -1;
          }
          else 
          {
            // le module 1 n'a pas répondu
            rssi1 = -1;
          }          
        }
      }
      else if(chaine_recu.indexOf("+INQ") != -1)
      {
        // La chaine reçu contient une seule réponse
        int adresse_lue_bt1 = hex_to_int(chaine_recu.substring(10, 12).c_str());
        rssi1 = hex_to_int(chaine_recu.substring(27, 29).c_str());
        if(adresse_lue_bt1 == adresse_module_bt1)
        {
          // Seul le module 1 a répondu
          rssi2 = -1;
        }
        else
        {
          // Seul le module 2 a répondu
          rssi2 = rssi1;
          rssi1 = -1;
        }
      }
      else
      {
        // La chaine reçu ne contient aucune réponse
        rssi1 = -1;
        rssi2 = -1;
      }
    }
    Serial.print(rssi1);
    Serial.print(", ");
    Serial.println(rssi2);
  } 
}

/**
 * @brief transforme une chaine de 2 caractères hexadécimaux en un entier non signé
*/
unsigned int hex_to_int(const char* chaine) {

  return strtoul(chaine, NULL, 16);
}