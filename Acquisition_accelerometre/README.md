#Acquisition des données d'une centrale initertielle MPU6050

Objectifs :
  Enregistrer sur une carte SD les données d'une centrale inertielle à intervalle régulier.
  
Architecture :
  Carte datalogger nano www.deek-robot.com: carte SD, RTC DS1307
  MPU6050 www.joy-it.net
  ! RTC DS1307 inutilisable car utilise la même adresse I2C que le MPU6050 

Bibliothèques : voir platformio.ini
