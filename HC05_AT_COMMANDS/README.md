# Commande AT pour module HC05

### Objectifs
- Envoyer des commandes AT sur un module BT HC05 maître via un moniteur série. Ce module maître est capable d'envoyer des requêtes aux modules esclaves dans son environnement. 
- Recevoir les réponses et les afficher sur le moniteur série
  -- du module maître pour des requêtes internes (Exemple: AT+NAME)
  -- du ou des modules BT HC05 esclaves pour des requêtes de type AT+INQ

### Références 
- voir schéma et documents techniques pdf
- **Remarque** : Quand le module BT maître accepte les commandes AT, sa led clignote rapidement.

### Architecture
- Utilise sofwareSerial pour se connecter au module BT HC05
- Utilise l'entrée **key** du module BT HC05 pour passer en mode commande AT
- Utilise le port série de l'arduino UNO pour se connecter au moniteur série du PC

### Algorithme
```
répéter le plus rapidement possible :
  Si un caractère est envoyé par le module BT HC05
    Afficher le caractère sur le moniteur série du PC
  Si un caractère est envoyé par le moniteur série du PC
     Envoyer le caractère au module BT HC05
```

### A faire
- modifier le schéma pour commander l'alimentation du module BT par un port de sortie car pour passer en mode commande AT, **key** doit être 0 quand le module est alimenté puis doit passer à 1. Actuellement si le module maître se connecte à un esclave (led clignote 2x puis extinction longue), le traitement des commandes AT est interrompu. Il faut alors couper l'alimentation des esclaves puis du maitre et relancer le maître d'abord puis les esclaves.
- Configurer le maître et/ou les esclaves pour qu'ils ne se connectent pas.

### Schéma
/HC05_AT_COMMANDS/HC05_uno_commande_AT_bb.jpg
