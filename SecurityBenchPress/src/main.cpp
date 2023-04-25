
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
uint8_t phase; // Pour identifier les phases du mouvement

double Setpoint, Input, Output;
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void arreter();
void monter(int vitesse);
void descendre(int vitesse);
float mesure_courant_moteur();
void afficher_mesures(int sens_deplacement, long valeur_encodeur, float courant_moteur);

void setup() {
  Serial.begin(115200);  //Ouvrir le port série pour communiquer avec le PC
  phase = 0; // Initialiser la séquence des déplacement
  //initialize the variables we're linked to PID controler
  Input = 0;
  Setpoint = 0;
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  memTime = millis(); // Récupérer le temps courant

}

void loop() {
  static int cpt_boucle; // Pour mémoriser le nombre de boucles effectuées 
  // Effectuer une tâche toutes les 50 ms
  if (millis() - memTime > 50) {
    memTime = millis();
    int sens_deplacement = enc.getStateEncLT();
    long courant_moteur = mesure_courant_moteur();
    float valeur_encodeur = enc.getCountsEncLT();
    Input = valeur_encodeur;
    afficher_mesures(sens_deplacement, valeur_encodeur, courant_moteur);
    /*
    Si la barre n'a pas atteint la position de consigne avec un écart inférieur à 10 points
      descendre la charge de 20mm
      afficher la valeur de l'encodeur et la valeur de Imoteur sur le port série
      
    Sinon attendre 20 boucles
    monter la barre de 20 mm
    Afficher la valeur de l'encodeur et la valeur de Imoteur
    si l'écart entre la consigne et la valeur réelle est inférieur à 10 points
      Attendre 20 boucles
    Tant que Imot < 1.5 A
      monter la barre
      
    */
    
    if ((valeur_encodeur <= 500) && (phase == 0)) {
      // Si la position basse n'est pas atteinte et qu'on est dans la phase 0 du mouvement
      Setpoint = 500;
      descendre(Output);  // Descendre jusqu'à la position de consigne 
    }
    
    else if (phase == 0) {
      // Sinon si on est toujours dans la phase 0
      cpt_boucle = 0;
      phase = 1;          
    }         
    
    if ((phase == 1) && (cpt_boucle < 40)) {
      // Rester en position pendant 2s (40x50 ms)
      descendre(Output);
    }
    else if (phase == 1) {
      // Sinon si on est toujours dans la phase 1
      phase = 2;
      Setpoint = 0;
    }

    if ((phase == 2) && (valeur_encodeur <= 0)) {
      monter(Output);          
    } 
    else if (phase == 2) {
      cpt_boucle = 0;
      phase = 3;         
    }

    if ((phase == 3) && (cpt_boucle < 40)) {
      monter(Output);
    }
    else if (phase == 3){
      phase = 4;         
    }

    if ((phase == 4) && (courant_moteur < 1.5)) {
      monter(200);
    }
    else if (phase == 4) {
      arreter();
      phase = 5;
      Setpoint = 0;
    }

    if ((phase == 5) && valeur_encodeur >= 0) {
      descendre(Output);
    }
    else if(phase == 5) {
      arreter();
      while(true);
    }

    cpt_boucle ++;  // Incrémenter le compteur de boucles   
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

void descendre(int vitesse) {
  drv.setSpeeds(-vitesse, 0);
}

void monter(int vitesse) {
  drv.setSpeeds(vitesse, 0);
}

void arreter() {
  drv.setSpeeds(0, 0);
}

void afficher_mesures(int sens_deplacement, long valeur_encodeur, float courant_moteur){
  String separateur = F(", ");
  Serial.print(sens_deplacement);
  Serial.print(separateur);
  Serial.print(valeur_encodeur);
  Serial.print(separateur);
  Serial.println(courant_moteur);

}