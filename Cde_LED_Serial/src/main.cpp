/* Commander une led connectée sur la broche 2 par le port série de l'arduino UNO.
   Quand l'arduino reçoit la chaine on, la led s'allume.
   Quand l'arduino reçoit la chaine off, la led s'éteint.
   La chaine de caractères de commande comporte au maximum 8 caractères et au moins un caractère de fin de ligne \r ou \n
   Le fonctionnement peut être testé avec le moniteur série intégré à Arduino: Outils -> Moniteur Série   
   puis saisir la chaine de caractères on ou off et cliquer sur envoyer
*/

#include <Arduino.h>

const int led_pin = 2;
bool chaine_complete = false;
String chaine = "";

void setup()
{
  pinMode(led_pin,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0) //s'il y a des données qui arrivent
  {
    char caractereRecu = Serial.read(); //Lecture d'une caractere dans le tampon série
    if ((caractereRecu == '\n') | (caractereRecu == '\r')) // Si le caractère reçu est un caractère de fin de ligne
    {
      chaine_complete = true;
    }
    else
    {
      chaine = chaine + caractereRecu; // Accumulation à droite des caractères reçus          
    }
  }
  // Commander la led en fonction de la chaîne reçue
  if (chaine_complete) 
  {
    if (chaine != "") 
    {
      if (chaine=="on")
        {
        digitalWrite(led_pin,HIGH);
        }
      else if (chaine == "off")
        {
        digitalWrite(led_pin,LOW);
        }
      Serial.println(chaine);
    }
    chaine = ""; // On initialise la chaine
    chaine_complete = false;
  }
  
} 
