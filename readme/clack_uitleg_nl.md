
# Clack Reader V4 met Atom S3 lite en TOF sensor

## Hardware
De kit bevat:
- [Atom S3 lite](https://www.tinytronics.nl/shop/nl/development-boards/microcontroller-boards/met-wi-fi/m5stack-atom-s3-lite-esp32-s3-development-board) van M5stack
- [TOF1 sensor](https://www.tinytronics.nl/shop/nl/platformen-en-systemen/m5stack/unit/m5stack-tof-unit) (vl53l0x) of [TOF2 sensor](https://shop.m5stack.com/products/time-of-flight-distance-unit-vl53l1x) (vl53l1x) van M5stack
  - TOF1 kan met een Grove kabel tot 5 meter meten (2x 2m + 1x 1m met connectorblokje)
  - TOF2 kan met een Grove kabel tot 3 meter meten (1x 2m + 1x 1m met connectorblokje)
- PCB met connectors ([visueel overzicht](../readme/Clack_reader_v4_connections_on_clack_ws1_EN.pdf) van de aansluitingen)
  - Stroom in/uit - Doorgeefverbindingen om het stroomverbruik en regeneratiestappen te monitoren (4-draads connector)
  - Flowmeter in/uit - Doorgeefverbindingen om het waterverbruik te monitoren (3-draads connector, aan de achterkant van de PCB)
  - DP-SW - Hiermee kan een vertraagde of onmiddellijke regeneratie worden gestart of kan regeneratie worden voorkomen. De functionaliteit hangt af van de instellingen in de waterontharder zelf (raadpleeg de handleiding)
- [3D-geprinte](../readme/clack_tof-holder.stl) TOF sensorbehuizing die met twee roestvrijstalen M4 bouten en moeren aan het deksel van het zoutreservoir kan worden bevestigd

> [!WARNING]
> Het is belangrijk om de TOF sensor aan te sluiten op de 4p Grove connector <ins>dichtbij</ins>, niet op, de Atom S3. Dit moet gebeuren voordat de PCB van stroom wordt voorzien, anders zal de Atom in een herstartlus komen en zal de Wi-Fi hotspot niet beschikbaar zijn om verbinding mee te maken.

## Hardware afbeeldingen
| Afbeelding | Beschrijving |
| --- | --- |
| [pcb_front.jpg](../readme/pcb_front.jpg) | Voorkant van Clack Reader |
| [pcb_back.jpg](../readme/pcb_back.jpg) | Achterkant van Clack Reader |
| [mount_position_image1.jpg](../readme/mount_position_image1.jpg) | Beste locatie voor het monteren van Clack Reader in de Clack WS1 behuizing |
| [mount_position_image2.jpg](../readme/mount_position_image2.jpg) | Beste locatie voor het monteren van Clack Reader in de Clack WS1 behuizing |

## Firmware
De Clack Reader PCB is geprogrammeerd in ESPHome en wordt vooraf geflasht geleverd. Je kunt ervoor kiezen om de meegeleverde versie te gebruiken of opnieuw te flashen vanuit deze Git-repo.

Als je de vooraf geflashte versie gebruikt, schakel dan de Clack Reader in (met de TOF sensor aangesloten), maak verbinding met de `Clack` hotspot met het wachtwoord `configesp`, en voer de Wi-Fi gegevens van je netwerk in.

# Home Assistant

## Lovelace Dashboard
![Voorbeeld](../readme/home_assistant_menu_clack_ws_eng.jpg)

Om het bovenstaande dashboard toe te voegen aan Home Assistant:
1. Maak een map `/www/images` in je `homeassistant` directory
1. Kopieer de afbeeldingen van [/www/images](../www/images) naar de nieuwe map. Deze afbeeldingen ondersteunen de weergave van de zoutniveau-simulatie.
1. Navigeer naar je Home Assistant dashboard
1. Klik rechtsboven op het potlood (dashboard bewerken)
1. Klik op de drie puntjes en selecteer 'Ruwe configuratie editor'
1. Voeg de tekst toe van het juiste dashboard YAML bestand uit de [homeassistant](../homeassistant) map van deze repo (bijv. [lovelace_menu_Post_fill_with_2nd_backwash_dn_brine_NL.yaml](../homeassistant/lovelace_menu_Post_fill_with_2nd_backwash_dn_brine_NL.yaml)) en sla op
1. Je hebt nu een nieuwe Clack weergave in Home Assistant

> [!IMPORTANT]
> Om de statusbalk correct weer te geven, gebruik [HACS](https://hacs.xyz) om de volgende frontend applicaties te installeren (onder voorbehoud van wijzigingen):
> - bar-card  https://github.com/spacerokk/bar-card
> - multiple-entity-row
> - stack-in-card

## Automatiseringen
Het is mogelijk om meldingen te ontvangen op een mobiele telefoon die de Home Assistant app gebruikt. Bijvoorbeeld wanneer een mogelijk waterlek wordt gedetecteerd of wanneer het zout opraakt. Voorbeeldautomatiseringen zijn beschikbaar in [clack_nl.yaml](../homeassistant/clack_nl.yaml). De onderstaande opties kunnen worden gebruikt om deze automatiseringen toe te voegen aan Home Assistant.

> [!IMPORTANT]
> Pas de `action` waarden aan om je eigen mobiele apparaat te gebruiken (bijv. vervang `- action: notify.mobile_app_iphone_van_a_c_a`)

### Optie 1 - Gebruik de Home Assistant [Packages](https://www.home-assistant.io/docs/configuration/packages/) functionaliteit
1. Maak een map onder `/config`, bij voorkeur genaamd `packages`, en kopieer [clack_nl.yaml](../homeassistant/clack_nl.yaml) naar de directory
1. Pas je `configuration.yaml` aan om toe te voegen:

>```yml
> homeassistant:
>   packages: !include_dir_named packages
> ```

3. Herstart Home Assistant

> [!WARNING]
> Hoewel automatiseringen die zijn toegevoegd met behulp van het [Packages](https://www.home-assistant.io/docs/configuration/packages/) patroon zichtbaar zijn in Home Assistant op het `Settings/Automations` scherm, kunnen ze niet worden gewijzigd via de Home Assistant UI. Om ze te wijzigen, bewerk je het package YAML bestand en herstart je Home Assistant.

### Optie 2 - Voeg clack_en.yaml samen met automations.yaml
Voeg de `automations:` sectie van [clack_nl.yaml](../homeassistant/clack_nl.yaml) samen met automations.yaml (let op de inspringingen).

## Configuratie

Gebruik de schuifregelaars op het dashboard om het systeem te configureren.

> [!TIP]
> Een diagram dat de min/max zoutafstand uitlegt, is hier te vinden. [hier.](../readme/min_max_NL.jpg)

| Instelling | Beschrijving | Voorbeeld |
| --- | --- | ---- |
| Vertraging waterlek alarm | Minuten van constante waterstroom die `binary_sensor.clack_leakage_detected` zal activeren om `true` te worden, waardoor automatiseringen (meldingen) kunnen worden geactiveerd | 30 min |
| Min. zoutafstand | Afstand tussen de TOF sensor en het zout <ins>wanneer de zoutcontainer vol is</ins> | 10 cm |
| Max. zoutafstand | Afstand tussen de TOF sensor en het zout <ins>wanneer het zout op het laagste niveau is</ins> (leeg)| 35 cm |
| Vul zoutafstand | Inches zout die nog in de tank zit, waaronder `sensor.clack_fill_salt` `true` wordt en automatiseringen (meldingen) kunnen worden geactiveerd | 4 cm |
| Capaciteit in dagen | Aantal dagen dat de Clack controller is ingesteld om te regenereren als het aantal gallons capaciteit nog niet is overschreden | 14 dagen |
| Capaciteit in liters | Liters capaciteit die de Clack controller weergeeft direct na een regeneratie | 3500 ltr |

## Uitleg werking

### Zoutmeetlogica
Aangezien veel waterontharders water boven het zout hebben (klein zoutreservoir), is er een slimme functie ingebouwd in de afstandsmeting.

De sensor meet de afstand naar beneden tot het zout. Die gemeten afstand zou alleen moeten toenemen naarmate het zout afneemt.
Hierdoor worden kortere afstanden niet geregistreerd als de meest recente waarde, alleen langere. Op deze manier beïnvloedt het stijgende water boven het zout de meting niet.

Er zijn twee metingen: Afstand en TOF Afstand.
Afstand is de meting met de slimme functie. TOF Afstand wordt continu elke 10 seconden gemeten maar heeft geen invloed op het 'Clack zoutniveau percentage' en beïnvloedt de Afstand niet tenzij het groter meet dan de Afstand.

### Zout bijvullen

Bij het bijvullen van je zoutcontainer druk je op de 'Reset fill salt' knop op het dashboard. Dit zal de TOF Afstand en Afstand gelijkmaken, waardoor de afstandsmeting opnieuw wordt ingesteld zodat het nieuwe zoutniveau opnieuw kan worden gemeten en geregistreerd. Onthoud dat de slimme functie anders de Afstand niet zou laten toenemen.
