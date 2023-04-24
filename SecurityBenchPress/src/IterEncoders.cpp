/*!
 * \file IterEncoders.cpp
 * \brief 	Classe pour utiliser les encodeurs en quadrature des moteurs gauche et droite
 * 			A base de capteurs à effet Hall. Pas de rebonds à traiter
 * \date 25 oct. 2017
 * \author: patrice
 */

#include "IterEncoders.h"


void isrLeft()   {
	/**
	 * \brief Routine d'interruption encodeur gauche
	 */
	bool nouvLeftB = digitalRead(LEFT_B);
	bool nouvLeftC = digitalRead(LEFT_XOR);
	bool nouvLeftA =  nouvLeftC ^ nouvLeftB;
	int result = (lastLeftA ^ nouvLeftB) - (nouvLeftA ^ lastLeftB);
	if (nouvLeftA) {
		countLeft += result;
		clockWiseLeft = true;
	}
	else {
		countLeft -= result;
		clockWiseLeft = false;
	}

	if((lastLeftA ^ nouvLeftA) & (lastLeftB ^ nouvLeftB))
		errorLT = true;
	else
		errorLT = false;

	lastLeftA = nouvLeftB;
	lastLeftB = nouvLeftA;
}


void isrRight()   {
	/**
	 * \brief Routine d'interruption encodeur droit
	 */
	bool nouvRightB = digitalRead(RIGHT_B);
	bool nouvRightA = digitalRead(RIGHT_XOR)  ^ nouvRightB;
	int result = (lastRightA ^ nouvRightB) - (nouvRightA ^ lastRightB);
	if (nouvRightA) {
		countRight -= result;
		clockWiseRight = false;
	}
	else {
		countRight += result;
		clockWiseRight = true;
	}

	if((lastRightA ^ nouvRightA) & (lastRightB ^ nouvRightB))
		errorRT = true;
	else
		errorRT = false;

	lastRightA = nouvRightB;
	lastRightB = nouvRightA;

}


IterEncoders::IterEncoders() {
	/**
	 * \brief Constructeur de la classe IterEncoders
	 */
    countLeftOld = 0;
	countRightOld = 0;
	 /** Initialisation des interruptions externes
	 * pour les encodeurs droit et gauche
	 */
	pinMode(LEFT_XOR, INPUT_PULLUP);
	pinMode(LEFT_B, INPUT_PULLUP);
	pinMode(RIGHT_XOR, INPUT_PULLUP);
	pinMode(RIGHT_B, INPUT_PULLUP);



	/**
	 * Initialisation de l'état des encodeurs
	 *
	 */
	lastLeftB = digitalRead(LEFT_B);
	lastLeftA = digitalRead(LEFT_XOR) ^ lastLeftB;
	errorLT = 0;

	lastRightB = digitalRead(RIGHT_B);
	lastRightA = digitalRead(RIGHT_XOR) ^ lastRightB;
	errorRT = 0;

	attachInterrupt(digitalPinToInterrupt(LEFT_B), isrLeft, CHANGE);  /// Détection des deux types de fronts
	attachInterrupt(digitalPinToInterrupt(RIGHT_B), isrRight, CHANGE);
}

void IterEncoders::stopEnc() {
	/**
	 * \brief Arrêter la prise en compte des interruptions externes
	 * 			et initialise les compteurs d'impulsions
	 */

	detachInterrupt(digitalPinToInterrupt(LEFT_B));
	detachInterrupt(digitalPinToInterrupt(RIGHT_B));
	countRight = 0;
	countLeft = 0;
}

long IterEncoders::getCountsEncRT(void) {
	/**
	 * \brief Renvoie l'état du compteur d'impulsion de la roue droite
	 * \return un long (32 bits signé) correspondant aux nombres d'impulsions générées par l'encodeur droit
	 * 6 impulsions par tour moteur\n
	 * réducteur 3952:33\n
	 * soit 718 impulsions par tour de roue\
	 */
	noInterrupts();
	int counts = countRight;
	interrupts();
	return counts;

}

long IterEncoders::getCountsEncLT(void) {
	/**
	 * \brief Renvoie l'état du compteur d'impulsion de la roue gauche
	 * \return un long (32bits signé) correspondant aux nombres d'impulsions générées par l'encodeur droit
	 * 6 impulsions par tour moteur\n
	 * réducteur 3952:33\n
	 * soit 718 impulsions par tour de roue\
	 */
	noInterrupts();
	int counts = countLeft;
	interrupts();
	return counts;

}

void IterEncoders::resetCountsEncLT(void) {
	/**
	 * \brief Remet à zéro le compteur d'impulsion de la roue gauche
	 */
	noInterrupts();
	countLeft = 0;
	interrupts();

}

void IterEncoders::resetCountsEncRT(void) {
	/**
	 * \brief Remet à zéro le compteur d'impulsion de la roue droite
	 */
	noInterrupts();
	countLeft = 0;
	interrupts();

}

int IterEncoders::getEncLT_state() {
	long countLeftNew = this->getCountsEncLT();
	if(countLeftNew - countLeftOld == 0) {
		countLeftOld = countLeftNew;
		return 0;
	}
	else if(clockWiseLeft) {
		return 1;
	}
	else {
		return -1;
	}

}

int IterEncoders::getEncRT_state() {
	long countRightNew = this->getCountsEncRT();
	if(countRightNew - countRightOld == 0) {
		countRightOld = countRightNew;
		return 0;
	}
	else if(clockWiseRight) {
		return 1;
	}
	else {
		return -1;
	}

}