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

### Algorithme
- Ouvrir la carte SD
- Récuperer le nombre de fichiers à la racine
- créer un fichier avec le nom "Acq" + (nb_fichier+1)
- ouvrir le fichier créé
- calculer le nombre d'itérations à enregistrer selon la période d'échantillonage
- répéter pour le nombre d'itérations nécessaires :
 - Enregistrer dans la carte SD les accélérations sur X, Y et Z
 - Envoyer les données sur le moniteur série

### A faire
- indiquer la configuration de la centrale inertielle
- enregistrer toutes les données

### Configuration du MPU6050 avec la bibliothèque tockn/MPU6050_tockn@^1.5.2
- writeMPU6050(MPU6050_SMPLRT_DIV, 0x00); // Configuration de la fréquence d'échantillonage des mesures à Fs_gyro = 8 kHz
  - rem: à cette frequence d'échantillonage, l'accéléromètre donnera 8 fois la même valeur car sa fréquence d'échantillonage reste à 1 kHz
- writeMPU6050(MPU6050_CONFIG, 0x00) // filtrage passe bas accéléromètre et gyroscope :
  - Gyroscope : Bande passante de 256 Hz pour un échantillonage à Fs = 8 kHz (Tsample = 125us) Tdelay = 0.98 ms
  - Accéléromètre : Bande passante de 260 Hz, Fs = 1 kHz, Tdelay = 0
- writeMPU6050(MPU6050_GYRO_CONFIG, 0x08); // pleine échelle des gyroscopes +-500°/s (1,4 tr/s)
- writeMPU6050(MPU6050_ACCEL_CONFIG, 0x00); // Pleine échelle de l'accéléromètre +- 2G (+- 19.62 m/s^2)
- writeMPU6050(MPU6050_PWR_MGMT_1, 0x01); // Horloge mems gyro axe X

### Données du MPU6050 avec la bibliothèque tockn/MPU6050_tockn@^1.5.2

- **rawAccX, Y, Z** : Accélérations brutes issues du convertisseur A.N. 16 bits
- **rawTemp** : température brute
- **rawGyroX, Y, Z** : Vitesses angulaires issues du convertisseur A.N. 16 bits
- **temp** : Température en °C
- **getAccX, Y, Z** : Accélérations mises à l'échelle en G
- **gyroX, Y, Z** : Vitesses angulaires mises à l'échelle et avec correction d'offset si le calibrage est demandé.
- **angleX, Y, Z** : angle X et Y corrigés par rapport à l'inclinaison calculée par rapport à l'accéléromètre et à l'angle calculé calculé par intégration du gyro du même axe. AngleZ uniquement calculé par intégration du gyroZ. Unité : degré

### tests
- **Sans lecture capteur écriture SD**
  - Tsample = 20 ms, Trecord = 5000 ms, Tchrono = 5319 ms
  - Tsample = 50 ms, Trecord = 5000 ms, Tchrono = 5117 ms
  - Tsample = 100 ms, Trecord = 5000 ms, Tchrono = 5054 ms

- **Lecture toutes données sans écriture SD**
  - Tsample = 20 ms, Trecord = 5000 ms, Tchrono = 5318 ms
  - Tsample = 50 ms, Trecord = 5000 ms, Tchrono = 5118 ms
  - Tsample = 100 ms, Trecord = 5000 ms, Tchrono = 5055 ms

- **Lecture toutes données et écriture SD**
  - Tsample = 20 ms, Trecord = 5000 ms, Tchrono = 5954 ms
  - Tsample = 50 ms, Trecord = 5000 ms, Tchrono = 5138 ms
  - Tsample = 100 ms, Trecord = 5000 ms, Tchrono = 5072 ms

** Conclusions du test **
- La bibliothèque utilisée limite la fréquence de lecture des données de la centrale inertielle car la méthode void MPU6050::update() provoque la lecture de toutes les grandeurs physiques fournies par le capteur et le calcul de toutes les grandeurs physiques calculées à partir des mesures.
- L'écriture des données dans la carte SD prend un temps non négligeable et limite la fréquence d'échantillonage à 20 fois par seconde (Tsample = 50 ms)
- L'envoie des données sur le port série n'a pas d'impact en regard de la méthode void MPU6050::update() si on utilise une vitesse de transmission élevé (250000 bit/s)
- **Pour utiliser une fréquence d'échantillonage plus importante, il faut écrire une bibliothèque qui ne lit que les données indispensables.**