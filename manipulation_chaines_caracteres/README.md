# Commande AT pour module HC05

### Objectifs
- Programme de test pour récupérer et traiter la réponse à la commande AT+INQ de deux modules bluetooth esclaves
- Le programme attend une chaine de caractères (voir tout en bas) modifiable pour simuler les defférentes réponses possibles des modules BT 

### Architecture
- Utilise le port série de l'arduino pour se connecter au moniteur série du PC

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

### Exemple de réponse
  -- +INQ:98D3:51:FE877B,1F00,FFC2\r\n+INQ:98D3:91:FE877B,1F00,FFBC\r\nOK