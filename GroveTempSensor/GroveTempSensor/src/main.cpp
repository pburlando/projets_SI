/**************************************************************************************************************************************
* Mesure la température grace à une thermistance grove temperature sensor https://wiki.seeedstudio.com/Grove-Temperature_Sensor_V1.2/
* Affiche la température sur un afficheur LCD RGB https://wiki.seeedstudio.com/Grove-LCD_RGB_Backlight/
* Donne une indication du confort thermique par une couleur du rétro-éclairage de l'afficheur
* Envoie la température vers le PC grace au port série
***************************************************************************************************************************************/

#include <Arduino.h>

// Bibliothèques pour utiliser l'afficheur LCD avec rétroéclairage RGB
#include <Wire.h>
#include "rgb_lcd.h"

           
const int B = 4200;               // B value of the thermistor
const double R0 = 100000;         // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

// Création de l'objet lcd pour utiliser l'afficheur et son rétroéclairage
rgb_lcd lcd;

// Définition de la couleur orange du rétroéclairage de l'afficheur
const int colorR = 225;  // intensité du rouge de 0 à 255
const int colorG = 53;   // intensité du vert de 0 à 255
const int colorB = 16;   // intensité du bleu de 0 à 255

float calculer_temperature(int a);  // Fonction pour calculer la température à partir du signal du capteur

void afficher_temperature(float temperature); // Fonction pour gérer l'affichage de la température

void gerer_couleur(float temperature); // Fonction pour gérer la couleur du rétroéclairage selon la température

void communiquer_au_pc(float temperature);

/***********************************************
 * Fonction d'initialisation exécutée une fois
 * à la mise sous tension de l'arduino
 **********************************************/
void setup()
{
    Serial.begin(9600); // Démarrage de la liaison série pour communiquer avec le PC
    
    lcd.begin(16, 2);  // Initialisation de l'afficheur LCD RGB

    lcd.setRGB(colorR, colorG, colorB);  // Eclairer l'afficheur en orange
}

/***********************************************
 * Fonction principale exécutée en boucle
 * aussi vite que possible
 **********************************************/
void loop()
{
    int a = analogRead(pinTempSensor);  // Numériser le signal électrique reçu sur la broche A0 de l'arduino

    float temperature = calculer_temperature(a);  // Calculer la température à partir du signal électrique numérisé
        
    afficher_temperature(temperature);  // Afficher la température sur l'afficheur

    gerer_couleur(temperature);  // Indiquer le niveau de confort sur l'afficheur

    communiquer_au_pc(temperature);

    delay(500);  // Attendre 0,5s avant de recommencer une mesure
}

/***********************************************
 * Fonctions définies par l'utilisateur
 **********************************************/
float calculer_temperature(int a) {
    // Calcul de la température selon les informations données par le constructeur du capteur
    float R = 1023.0/a-1.0;
    R = R0*R;
    float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15;
    return temperature;
}

void afficher_temperature(float temperature){
    // Affiche la température, valeur et unité sur l'afficheur 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp = ");
    lcd.print(temperature);
    lcd.print(" ");
    lcd.write(0xDF);  // Affiche le symbole du degré
    lcd.print("C");    
}


void gerer_couleur(float temperature) {
    // Colore le rétroéclairage de l'afficheur selon le niveau de confort
    if (temperature < 19){
        lcd.setRGB(0,0,255);
    }
    else if(temperature < 21) {
        lcd.setRGB(225, 53, 16);
    }
    else {
        lcd.setRGB(255,0,0);
    }
}


void communiquer_au_pc(float temperature) {
    // Communication de la température mesurée au PC
    Serial.print("temperature = ");
    Serial.println(temperature);
}