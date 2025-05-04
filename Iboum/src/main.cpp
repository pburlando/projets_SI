/*
 * Communication avec le robot par un module bluetooth BT-06
 * Le robot reçoit 3 octets
 * octet1 : mode
 * octet2 : vitesse
 * octet3 : écart
 * Connexion Rx et Tx sur les broches 0 et 1
 */

 #include "Arduino.h"
 #include <MotorsIboum17.h>
 
 MotorsIboum17 bot;      // Objet qui permet de commander les mouvements du robot
 byte receivedData[3];   // Tableau de 3 octets 
 
 
 
 void setup()
 {
 // Add your initialization code here
   Serial.begin(9600);  // Lancement de la communication série à 9600 bits par seconde
 }
 
 // The loop function is called in an endless loop
 void loop()
 {
 //Add your repeated code here
   // Si des données arrivent sur le port série
   if(Serial.available()) {
     Serial.readBytes(receivedData,3);  // Lire 3 octets et les enregistrer dans un tableau
 
   }
   // La case 0 du tableau receivedData contient un caractère qui coorespond à une commande
   // Exécuter un cas selon le caractère reçu
   switch (receivedData[0]) {
   case 'F':              	// Si le caractère reçu est un F 
     bot.forward(200);	// Faire avancer le robot
     break;				// Instruction qui permet de sortir de la structure de décision switch. A ne pas oublier 
   case 'L':
     bot.rotateLeft();
     break;
   case 'R':
     bot.rotateRight();
     break;
   case 'B':
     bot.reverse(200);
     break;
   default :				// Si le caractère reçu ne correspond à aucun des cas précédent (cas par défaut)
     bot.forward(0);		// arrêter le robot
 
     break;
   }
   Serial.flush();			// Vider le buffer du port série
   receivedData[0] = 'S';  // Initialiser la commande à 'S'
   delay(100);
 
 }