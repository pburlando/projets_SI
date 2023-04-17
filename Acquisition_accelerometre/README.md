# Acquisition des données d'une centrale initertielle MPU6050

### Objectifs :
  - Enregistrer sur une carte SD les données d'une centrale inertielle à intervalle régulier.
  
### Architecture :
  - Carte datalogger nano www.deek-robot.com: carte SD, RTC DS1307
  - MPU6050 www.joy-it.net
  - ! RTC DS1307 inutilisable car utilise la même adresse I2C que le MPU6050 

### Bibliothèques : 
  - voir platformio.ini
  - tockn/MPU6050_tockn@^1.5.2 https://github.com/tockn/MPU6050_tockn
  - arduino-libraries/SD@^1.2.4 https://github.com/arduino-libraries/SD

#### Algorithme
Ouvrir la carte SD
Récuperer le nombre de fichiers à la racine
créer un fichier avec le nom "Acquis" + (nb_fichier+1)
ouvrir le fichier créé
calculer le nombre d'itérations à enregistrer selon la période d'échantillonage
