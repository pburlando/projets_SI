
/* Test d'un moteur avec encodeur
 * Attention les bibliothèques sont prévues pour piloter deux moteur 
 * On utilisera la partie gauche
 */


#include <Arduino.h>
#include "DRVL298NMotorShield.h"
#include "IterEncoders.h"
#include <PID_v1.h>

// Brochage

// Les broches 5 et 3 sont réservées à l'encodeur gauche
// Les broches 4 et 2 sont réservées à l'encodeur droit

#define CDEMOTEURDIR 9
#define CDEMOTEURPWM 10

const int currentSensorPin = A0;

DRVL298NMotorShield drv(CDEMOTEURDIR, CDEMOTEURPWM, 7, 8, true, false);  //**  Pilotage du moteur à courant continu, broches 9 et 10 utilisées, broches 7 et 8 réservées
IterEncoders enc;

unsigned long memTime = 0L; // Pour exécuter une tâche à intervalle régulier
double Setpoint, Input, Output;
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(115200);  //Ouvrir le port série pour communiquer avec le PC
  memTime = millis(); // Récupérer le temps courant

}

void loop() {
  static int cpt_boucle; // Pour mémoriser le nombre de boucles effectuées
  // Effectuer une tâche toutes les 50 ms
  if (millis() - memTime > 50) {
    memTime = millis();
    cpt_boucle ++;
    
    if (cpt_boucle < 100) {
      drv.setSpeeds(255, 0);  // Rotation sens + à vitesse max pendant 5s
      Serial.println( enc.getCountsEncLT() ); // Afficher le nombre d'impulsions comptées  
    }
    
    else if (cpt_boucle < 140) {
      drv.setSpeeds(0, 0);  //Arret moteur pendant 2s          
    }         
    
    else if (cpt_boucle < 240) {
      drv.setSpeeds(-255, 0);  // Rotation sens - à vitesse max pendant 5s
      Serial.println(enc.getCountsEncLT());           
    }
    
    else if (cpt_boucle < 280) {
      drv.setSpeeds(0, 0);  //Arret moteur pendant 2s          
    }
    
    else {
      cpt_boucle = 0;         
    }
    
  }  
}

float mesure_courant_moteur() {	
  /**
	 * \brief Mesurer l'intensité du courant dans le moteur pleine échelle +- 5A
   * \return I en Ampère
	 */
  uint16_t sensorValue = analogRead(currentSensorPin);
  return ((float)(sensorValue - 338) * 5 / 0.11) / 1024;
}