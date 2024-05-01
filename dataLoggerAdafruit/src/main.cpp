/*
* Programme de test du shield data logger d'adafruit
* https://learn.adafruit.com/adafruit-data-logger-shield
* @brief échantillone l'entrée AD0 à une fréquence de 50 Hz
*        enregistre 500 valeurs dans une mémoire tampon (10s)
*        ecrit les 500 valeurs dans un fichier CSV
*        nom du dossier = jj_mm_aa
*        nom du fichier = hh_mn_ss
*/

#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}