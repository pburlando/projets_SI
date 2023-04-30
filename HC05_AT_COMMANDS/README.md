# Commande AT pour module HC05

### Objectifs
- Envoyer des commandes AT sur un module BT HC05 maître via un moniteur série
- Recevoir les réponses du ou des modules BT HC05 esclaves sur le moniteur série

### Architecture
- Utilise sofwareSerial pour se connecter le module BT HC05
- Utilise l'entrée **key** du module BT HC05 pour passer en mode commande AT
- Utilise le port série de l'arduino NANO pour se connecter au moniteur série du PC

### Communication
- requête "AT+INQ\r\n"
- réponses possibles :
  -- Les deux modules répondent :
    --- +INQ:98D3:91:FE3DB8,1F00,FFC7
    --- +INQ:98D3:51:FE877B,1F00,FFC7
    --- OK
  -- Un des deux modules répond :
    --- +INQ:98D3:51:FE877B,1F00,FFC2
    --- +INQ:98D3:51:FE877B,1F00,FFBC
    --- OK
  -- Aucun module ne répond (temps d'attente 33 s max )

### Algorithme
- Répéter toutes les 2s
  --  lancer la requête
  --  Tant qu'il y a des caractères disponibles sur sofwareSerial
    --- Accumuler les caratères dans la chaine de réception (66 char)
    --- Si les deux derniers forment la sous chaine OK :
       ---- séparer en sous chaine avec les caractères \r\n

+INQ:98D3:51:FE877B,1F00,FFC2\r\n+INQ:98D3:91:FE877B,1F00,FFBC\r\nOK