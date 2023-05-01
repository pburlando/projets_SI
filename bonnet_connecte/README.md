# Programme de test pour le projet bonnet connecté
Programme de test pour envoyer la commande AT+INQ au module BT HC05 maître puis récupérer et traiter la réponse de deux modules bluetooth esclaves.
Affiche la valeur des taux de réception rssi1, rssi2 des modules BT HC05 esclaves ainsi que des données de debug (voir exemple de communication) sur le moniteur série du PC.

### Objectifs
- Envoyer la commande AT+INQ sur un module BT HC05 maître via un moniteur série. 
- Recevoir les réponses et les afficher sur le moniteur série
  - du module maître pour des requêtes internes (Exemple: AT+NAME)
  - du ou des modules BT HC05 esclaves pour des requêtes de type AT+INQ

### Références 
- voir schéma et documents techniques pdf
- voir configuration des modules BT.
- **Remarque** : Quand le module BT maître accepte les commandes AT, sa led clignote rapidement.

### Architecture
- Utilise sofwareSerial pour se connecter au module BT HC05
- Utilise l'entrée **key** du module BT HC05 **indispensable** pour passer en mode commande AT
- Utilise le port série de l'arduino UNO pour se connecter au moniteur série du PC

### Algorithme
**à faire**
voir commentaire dans \src\main.cpp

### A faire
- modifier le schéma pour commander l'alimentation du module BT par un port de sortie car pour passer en mode commande AT, **key** doit être 0 quand le module est alimenté puis doit passer à 1. Actuellement si le module maître se connecte à un esclave (led clignote 2x puis extinction longue), le traitement des commandes AT est interrompu. Il faut alors couper l'alimentation des esclaves puis du maitre et relancer le maître d'abord puis les esclaves.
- Configurer le maître et/ou les esclaves pour qu'ils ne se connectent pas.
- écrire l'algorithme du programme

### Exemple de communication (horodatées)
15:08:16.794 -> Requete AT+INQ envoye
15:08:18.654 -> Chaine lue : 60 bytes : +INQ:98D3:51:FE877B,1F00,FFCE+INQ:98D3:51:FE877B,1F00,FFCFOK
15:08:18.754 -> 206, -1
15:08:21.810 -> Requete AT+INQ envoye
15:08:22.341 -> Chaine lue : 60 bytes : +INQ:98D3:51:FE877B,1F00,FFCF+INQ:98D3:91:FE3DB8,1F00,FFDAOK
15:08:22.408 -> 207, 218
15:08:26.792 -> Requete AT+INQ envoye
15:08:27.124 -> Chaine lue : 60 bytes : +INQ:98D3:91:FE3DB8,1F00,FFD5+INQ:98D3:51:FE877B,1F00,FFCFOK
15:08:27.224 -> 213, 207
15:08:31.807 -> Requete AT+INQ envoye
15:08:32.106 -> Chaine lue : 60 bytes : +INQ:98D3:51:FE877B,1F00,FFCA+INQ:98D3:91:FE3DB8,1F00,FFD7OK
15:08:32.172 -> 202, 215
15:08:36.791 -> Requete AT+INQ envoye
15:08:37.388 -> Chaine lue : 60 bytes : +INQ:98D3:51:FE877B,1F00,FFCC+INQ:98D3:91:FE3DB8,1F00,FFD6OK
15:08:37.455 -> 204, 214
15:08:41.805 -> Requete AT+INQ envoye
15:08:42.074 -> Chaine lue : 60 bytes : +INQ:98D3:51:FE877B,1F00,FFCF+INQ:98D3:51:FE877B,1F00,FFCCOK
15:08:42.174 -> 207, -1
15:08:46.791 -> Requete AT+INQ envoye
15:08:47.024 -> Chaine lue : 60 bytes : +INQ:98D3:91:FE3DB8,1F00,FFD9+INQ:98D3:51:FE877B,1F00,FFD0OK
15:08:47.124 -> 217, 208
15:08:51.773 -> Requete AT+INQ envoye
15:08:52.271 -> Chaine lue : 60 bytes : +INQ:98D3:91:FE3DB8,1F00,FFD7+INQ:98D3:51:FE877B,1F00,FFD0OK
15:08:52.371 -> 215, 208
15:08:56.785 -> Requete AT+INQ envoye
15:08:57.083 -> Chaine lue : 60 bytes : +INQ:98D3:91:FE3DB8,1F00,FFD8+INQ:98D3:51:FE877B,1F00,FFD0OK
15:08:57.183 -> 216, 208

