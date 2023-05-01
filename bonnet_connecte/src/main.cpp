/**
 * @brief Programme de test pour envoyer la commande AT+INQ au module BT HC05 maître
 * puis récupérer et traiter la réponse de deux modules bluetooth esclaves 
 * Affiche la valeur des taux de réception rssi1, rssi2
 * des modules BT HC05 esclaves d'adresses :
 * 98D3:91:FE3DB8
 * 98D3:51:FE877B
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
const int keyPin = 5; // key à brancher sur le module HC05

SoftwareSerial btSerial(10, 11); // RX, TX arduino à brancher au TX, RX sur le module HC05 (lignes croisées)
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

const int adresse_module_bt1 = 0x51;
const int adresse_module_bt2 = 0x91;

int rssi1; // Niveau rssi du module 1 (-1 si aucune réception, 255 si réception maximale)
int rssi2;

unsigned long memTime = 0L;  // Pour envoyer la requête à intervalle régulier

unsigned int hex_to_int(const char* chaine);

void setup() 
{
  // reserver 200 octets pour la chaine d'entrée:
  inputString.reserve(200);
  // Passage en commande AT logiciellement Datasheet HC05 page 1 How to get to the AT mode way 1 
  pinMode(keyPin, OUTPUT);
  digitalWrite(keyPin, LOW);
  delay(200);
  digitalWrite(keyPin, HIGH); 
  delay(800);
  Serial.begin(9600); // Port série connecté entre un PC avec moniteur série et un arduino  
  btSerial.begin(9600);  // Port série connecté entre un arduino et un module HC05
  btSerial.setTimeout(500); //Timeout de la communication avec le module BT à 0.5 s
  memTime = millis(); // Enregistrer le temps courant en ms dans memTime
  

}

void loop() {
  // Envoyer la requête toutes les 5 secondes
  if((millis() - memTime) > 5000)
  {
    memTime = millis();
    Serial.println(F("Requete AT+INQ envoye"));
    btSerial.print("AT+INQ\r\n");  //Envoyer la requête sur le module BT HC05 Maitre
  }
  
  if (btSerial.available() > 0) {
    char inChar = (char)btSerial.read();
    // Ajouter le caractère reçu à inputString sauf si c'est un caractère spécial retour chariot ou aller à ligne
    if((inChar != '\r'))
    {
      if(inChar != '\n')
      {
        inputString += inChar;
      }
    }
  
    if(inputString.lastIndexOf("OK") != -1)
    // Si la chaine reçu se termine par OK
    {
      // La chaine est complete, elle peut être analysée
      stringComplete = true;
    }
    else if(inputString.lastIndexOf("ERROR") != -1)
    // Si la chaine reçu contient ERROR
    {
      if (inChar == '\n')
      // Si on reçoit un caractère aller à la ligne
      {
        inputString.trim();     //Enlever les espaces et les caractères spéciaux de la chaine reçu
        stringComplete = true;  // La chaine est complete, elle peut être analysée
      }
    }

  }

  if(stringComplete == true)
  {
    /*
    *  Affichage de la trame lue
    */
    Serial.print("Chaine lue : ");
    Serial.print(inputString.length());
    Serial.print(" bytes : ");
    Serial.println(inputString);

    /*
    *  Analyse de la trame lue
    *  et extraction des niveaux RSSI des modules esclaves
    *  qui ont répondu
    */
    if(inputString.indexOf("+INQ", 10) != -1)
    // Si la chaine reçu contient la réponse des 2 modules
    {
      int adresse_lue_bt1 = hex_to_int(inputString.substring(10, 12).c_str());
      int adresse_lue_bt2 = hex_to_int(inputString.substring(39, 41).c_str());
      rssi1 = hex_to_int(inputString.substring(27, 29).c_str());
      rssi2 = hex_to_int(inputString.substring(56, 58).c_str());
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
    else if(inputString.indexOf("+INQ") != -1)
    {
      // La chaine reçu contient une seule réponse
      int adresse_lue_bt1 = hex_to_int(inputString.substring(10, 12).c_str());
      rssi1 = hex_to_int(inputString.substring(27, 29).c_str());
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
    Serial.print(rssi1);
    Serial.print(", ");
    Serial.println(rssi2);

    inputString = "";
    stringComplete = false;
  }
}

/**
 * @brief transforme une chaine de 2 caractères hexadécimaux en un entier non signé
*/
unsigned int hex_to_int(const char* chaine) 
{

  return strtoul(chaine, NULL, 16);
}