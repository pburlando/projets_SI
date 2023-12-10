/*#####################################################################
Acquisition de la position angulaire d'un potentiomètre
avec une période d'échantillonage TE en ms
*/

// directives de compilation
#include <Arduino.h>

#define TE 100      // Période d'échantillonage en ms

// défintion des variables globales
const int analogPin = 0;        // Le potentiomètre grove branché sur A0 

unsigned long mem_time = 0L;    // >Contient le temps courant pour exécuter une tâche à intervalle de temps régulier

// Executer une seule fois à l'initialisation
void setup()
{
  Serial.begin(9600);          //  initialisation de la connexion série débit 9600 bits par seconde
  mem_time = millis();         //  initialisation du timer à la valeur du temps courant
}

// Boucler en permanence
void loop()
{
  // Tâche exécutée toutes les TE ms
  if ((millis() - mem_time) >= TE)
  {
    mem_time = millis();              //  initialisation du timer à la valeur du temps courant
    int val = analogRead(analogPin);  // lit la valeur de la tension analogique présente sur la broche  
    Serial.println(val);              // écrit la valeur (comprise en 0 et 1023) sur la liaison série
  }
}