# Programme de test pour le projet Security Bench Pressed

### Objectifs :
- Retenir la barre d'un banc de développé couché pour éviter de blesser l'utilisateur.
- Aider la remontée de la barre si elle reste trop longtemps en position basse. 
- Commander un moteur à courant continu dans les deux sens de marche.
  -- Déterminer la position de la barre à l'aide de codeurs incrémentaux en quadrature
  -- Mesurer le courant dans le moteur

### Architecture :
- Maquette de test échelle 1:5
- Moto-réducteur 1:75 6V
- Tambour réduction 10:50
- Pont en H L298N
- Capteur de courant à effet Hall analogique +-5A

### Bibliothèques (voir platformio.ini):
- DRVL298NMotorShield.h
- IterEncoders.h
- br3ttb/PID@^1.2.1

### Algorithme
- Chercher la position haute.
- dérouler la sangle jusqu'au point bas programmé.
- Si la sangle est tendue et la barre en position basse plus de 20 s :
  -- Remonter la barre jusqu'à la position haute.

### A faire
- Améliorer le modèle mécanique de scilab
- tester l'asservissement de position et écrire une fonction deplacement_mm()
- Faire un enregistrement du courant, temps, positions avec une charge de 500 g, sur un mouvement de type :
  -- Descendre la barre jusqu'au point bas
  -- Attendre 5 s
  -- Remonter la barre au point haut
  -- Attendre 5s
  -- Monter la barre jusqu'au blocage
  -- Arreter le mouvement si le courant dépasse 150% du courant en montée