
# Clack reader V4 met Atom S3 lite en TOF sensor

## Hardware
Er wordt op de PCB onder andere gebruik gemaakt van 
- de [Atom S3 lite](https://www.tinytronics.nl/shop/nl/development-boards/microcontroller-boards/met-wi-fi/m5stack-atom-s3-lite-esp32-s3-development-board) van M5stack
- de [TOF1 sensor](https://www.tinytronics.nl/shop/nl/platformen-en-systemen/m5stack/unit/m5stack-tof-unit)  (vl53l0x) of [TOF2 sensor](https://shop.m5stack.com/products/time-of-flight-distance-unit-vl53l1x) (vl53l1x) van M5stack
- TOF1 kan met een grove kabeltje van maximaal 5 mtr meten (2x 2mtr + 1x 1mtr met tussenblokjes)
  TOF2 kan met een grove kabeltje van maximaal 3 mtr meten (1x2 mtr + 1x 1mtr met tussenblokjes)
- [3D printed](../readme/clack_tof-holder.stl)  TOF sensor behuizing welke met twee rvs M4 boutjes en moertjes vast gezet kan worden op het deksel van het zout reservoir.

## Software
Alles is in ESPHOME geprogrammeerd.
Eigen wifi gegevens invullen via wifi hotspot met als wachtwoord: configesp

## Lovelace menu
Om het lovelace menu volledig te benutten met de zoutniveau simulatie zoals hieronder in het dashboard te zien is, dient er een map /www/images aangemaakt te worden in je home assistant directory.
Kopieer daar alle simulatie plaatjes in van [/www/images](../www/images)

Voorbeeld dashboard: 
![Example](Printscreen_NL.jpg)

Het dashboard zelf kan geplaatst worden via home assistant drie puntjes - dashboard bewerken - ruwe configuratie editor
Tekst uit bijvoorbeeld [lovelace_menu_Post_fill_with_2nd_backwash_dn_brine_NL.yaml](../home_assistant/lovelace_menu_Post_fill_with_2nd_backwash_dn_brine_NL) daar aan toevoegen en opslaan.
U heeft nu een extra menu Clack in home assistant

Ook om de status balk goed te zien kunt u via HACS een frontend toepassing installeren, zoals bar-card.


## Automations
Het is mogelijk een notificatie te ontvangen op een telefoon waar home assistant app op draait.
Bijvoorbeeld bij waterlekkage of wanneer het zout op is.
Verander dan nog wel de entiteit van uw eigen device in de code.

[clack_nl.yaml](../home_assistant/clack_nl.yaml) samenvoegen met automations.yaml (let op inspringingen) of beter:

### Bestand op eigen lokatie:
[clack_nl.yaml](../home_assistant/clack_nl.yaml) op een eigen lokatie zetten:
configuration.yaml aanpassen naar:

```yml
homeassistant:
  packages: !include_dir_named packages
```

Dan directory /packages aanmaken in /config en daar de automation_saltalarm_nl.yaml in kopieren
HA opnieuw starten

## Uitleg werking

Met de schuifregelaars de juiste hoogtes instellen.
Voorbeeld van de minimale en maximale hoogte vind u [hier.](../README/min_max_NL.jpg) 
Zout bijvullen afstand is de afstand vanaf wanneer er een alarm (automation) zal verstuurd worden (waarde van Bijvullen wordt dan "ja")
Dit is gemeten vanaf de onderkant van de tank, dus vanaf de maximale afstand naar boven toe.

Omdat er veel waterontharders zijn waar het water boven het zout staat (klein zoutreservoir), zit er een slimme functie ingebouwd in de afstandmeting
Namelijk, de sensor meet de afstand naar beneden toe. De te meten afstand wordt dan alleen maar groter als het zout op raakt.
Kortere afstanden worden dan ook niet geregistreerd als laatste waarde, enkel langere afstanden. Zo heeft water wat boven het zout uitkomt geen meet invloed meer.
Er zijn 2 registratie: Afstand en Afstand TOF.
Afstand is de meting met de slimme functie. Afstand TOF meet nog steeds continu elke 10 sec maar heeft geen invloed op de procent schaal of berekeningen daaraan.



### DRUKKNOP
De drukknop heeft twee functies:
kort ingedrukt (led gaat uit): stop de niveaumeting, handig om zelf even te kijken in het zoutvat.
kort ingedrukt (led gaat aam): zet de niveaumeting weer voort.

Lang ingedrukt (voor 3 sec.). Registreer tijd en datum. Handig om te zien wanneer voor het laatst bijgevuld is.
En om de slimme afstand weer te resetten naar 0 en werkelijke meting. Het zout is immers weer bijgevuld

## Noot:
Er is een  [3d print](../README/Saltlevel_Atoms3_TOF.stl)  bestand bijgevoegd om zelf de behuizing te printen.
De behuizing dient met dubbelzijdige tape op het zoutreservoir aan de bovenkant gemonteerd te worden.
