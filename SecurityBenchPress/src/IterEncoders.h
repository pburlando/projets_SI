/*!
 * \file IterEncoders.h
 *
 * \date 25 oct. 2017
 * \author patrice
 *
 * bibliothèque de gestion des encodeurs en quadrature\n
 * utilisation des interruptions externes pour compter les impulsions générées par les codeurs\n
 * de la roue droite et gauche.\n
 * Sur le front montant de la broche 2/3 on incrémente un compteur
 *
 *
 */

#ifndef ITERENCODERS_H_
#define ITERENCODERS_H_

#include "Arduino.h"

#define LEFT_XOR  5		//! Broche B de l'encodeur gauche
#define LEFT_B  3		//! Broche A de l'encodeur gauche
#define RIGHT_XOR  4	//! Broche B de l'encodeur droit
#define RIGHT_B  2		//! Broche A de l'encodeur droit

static volatile long countLeft;		//! Position (en nombre de pas) du codeur
static volatile long countRight;     //! Position (en nombre de pas) du codeur


static volatile bool lastLeftA;		//! Etat précédent de LeftA
static volatile bool lastLeftB;		//! Etat précédent de LeftB
static volatile bool errorLT;		//! Drapeau d'erreur de l'encodeur gauche

static volatile bool lastRightA;	//! Etat précédent de RightA
static volatile bool lastRightB;	//! Etat précédent de RightB
static volatile bool errorRT;		//! Drapeau d'erreur de l'encodeur droit
static volatile bool clockWiseLeft;        //! L'encodeur gauche tourne en sens horaire
static volatile bool clockWiseRight;       //! L'encodeur droit tourne en sens horaire
static volatile bool stopLeft;             //! L'encodeur gauche est arrêté
static volatile bool stopRight;            //! L'encodeur droit est arrêté
static long countLeftOld;                  //! Mémorisation du nombre de pas de l'encodeur gauche
static long countRightOld;                 //! Mémorisation du nombre de pas de l'encodeur gauche





class IterEncoders {
public:
	IterEncoders();
	void stopEnc(void);
	long getCountsEncRT(void);
	long getCountsEncLT(void);
	void resetCountsEncLT(void);
	void resetCountsEncRT(void);
	int getEncLT_state(void);
	int getEncRT_state(void);

};

#endif /* ITERENCODERS_H_ */
