
# Clack Reader V4 with Atom S3 lite and TOF sensor

## Hardware
The kit includes:
- [Atom S3 lite](https://www.tinytronics.nl/shop/nl/development-boards/microcontroller-boards/met-wi-fi/m5stack-atom-s3-lite-esp32-s3-development-board) from M5stack
- [TOF1 sensor](https://www.tinytronics.nl/shop/nl/platformen-en-systemen/m5stack/unit/m5stack-tof-unit)  (vl53l0x) or [TOF2 sensor](https://shop.m5stack.com/products/time-of-flight-distance-unit-vl53l1x) (vl53l1x) from M5stack
  - TOF1 can measure with a Grove cable up to 5 meters (2x 2m + 1x 1m with connector blocks)
  - TOF2 can measure with a Grove cable up to 3 meters (1x 2m + 1x 1m with connector blocks)
- PCB with connectors ([visual overview](../readme/Clack_reader_v4_connections_on_clack_ws1_EN.pdf) of connections)
  - Power in/out - Passthrough connections to monitor power usage and regeneration steps (4-wire connector)
  - Flowmeter in/out - Passthrough connections to monitor water usage (3-wire connector, on back of PCB)
  - DP-SW - Allows for a delayed or immediate regen, or can prevent a regen. Functionality depends on the settings in the softener control itself (refer to the manual)
- [3D printed](../readme/clack_tof-holder.stl)  TOF sensor housing which can be attached to the salt reservoir lid with two stainless steel M4 bolts and nuts

> [!WARNING]
> It is important to connect the TOF sensor to the 4p Grove connector <ins>near</ins>, not on, the Atom S3. This needs to be done before powering the PCB, otherwise the Atom will enter a reboot loop and the Wi-Fi hotspot will not be available to connect to.

## Hardware Images
| Image | Description |
| --- | --- |
| [pcb_front.jpg](../readme/pcb_front.jpg) | Front of Clack Reader |
| [pcb_back.jpg](../readme/pcb_back.jpg) | Back of Clack Reader |
| [mount_position_image1.jpg](../readme/mount_position_image1.jpg) | Best location for mounting Clack Reader inside the Clack WS1 housing |
| [mount_position_image2.jpg](../readme/mount_position_image2.jpg) | Best location for mounting Clack Reader inside the Clack WS1 housing |

## Firmware
The Clack Reader PCB is programmed in ESPHome and arrives preflashed. You can choose to use the supplied version or reflash from this Git repo.

If using the preflashed version, power on the Clack Reader (with the TOF sensor attached), connect to the `Clack` hotspot using the password `configesp`, and enter your network's Wi-Fi details.

# Home Assistant

## Lovelace Dashboard
![Example](../readme/home_assistant_menu_clack_ws_eng.jpg)

To add the dashboard shown above to Home Assistant:
1. Create a `/www/images` folder in your `homeassistant` directory
1. Copy the images from [/www/images](../www/images) into the new folder. These images support the salt level simulation view.
1. Navigate to your Home Assistant dashboard
1. At the top-right, click the pencil (edit dashboard)
1. Click the three dots and select 'Raw configuration editor'
1. Add the text from the appropriate dashboard YAML file from the [homeassistant](../homeassistant) folder of this repo (e.g., [lovelace_menu_Post_fill_with_2nd_backwash_dn_brine_EN.yaml](../homeassistant/lovelace_menu_Post_fill_with_2nd_backwash_dn_brine_EN.yaml)) and save
1. You will now have a new Clack view in Home Assistant

> [!IMPORTANT]
> To properly view the status bar, use [HACS](https://hacs.xyz) to install the following frontend applications (subject to change):
> - bar-card
> - multiple-entity-row
> - stack-in-card

## Automations
It is possible to receive notifications on a mobile phone running the Home Assistant app. For example, when a possible water leak is detected or when salt runs low. Sample automations are provided in [clack_en.yaml](../homeassistant/clack_en.yaml). The options below can be used to add these automations to Home Assistant.

> [!IMPORTANT]
> Modify the `action` values to use your own mobile device (e.g., replace `- action: notify.mobile_app_iphone_van_a_c_a`)

### Option 1 - Use Home Assistant [Packages](https://www.home-assistant.io/docs/configuration/packages/) functionality
1. Create a folder under `/config`, preferably named `packages`, and copy [clack_en.yaml](../homeassistant/clack_en.yaml) to the directory
1. Adjust your `configuration.yaml` to add:

>```yml
> homeassistant:
>   packages: !include_dir_named packages
> ```

3. Restart Home Assistant

> [!WARNING]
> While automations added using the [Packages](https://www.home-assistant.io/docs/configuration/packages/) pattern are visible in Home Assistant on the `Settings/Automations` screen, they are not modifiable via the Home Assistant UI. To modify them, edit the package YAML file and restart Home Assistant.


### Option 2 - Merge clack_en.yaml with automations.yaml
Merge the `automations:` section of [clack_en.yaml](../homeassistant/clack_en.yaml) with automations.yaml (pay attention to indentations).

## Configuration

Use the sliders on the dashboard to configure the system.

> [!TIP]
> A diagram explaining min/max salt distance can be found [here.](../readme/min_max_EN.jpg)

| Setting | Description | Example |
| --- | --- | ---- |
| Delay water leakage alarm | Minutes of constant water flow that will trigger `binary_sensor.clack_leakage_detected` to turn `true`, allowing automations (alerts) to trigger | 30 min |
| Min. salt distance | Distance between the TOF sensor and the salt <ins>when the salt container is full</ins> | 2 in. |
| Max. salt distance | Distance between the TOF sensor and the salt <ins>when the salt is at its lowest level</ins> (empty)| 25 in. |
| Fill salt distance | Inches of salt remaining in the tank, below which `sensor.clack_fill_salt` turns `true` and automations (alerts) may trigger | 4 inches |
| Capacity in days | Number of days the Clack controller is set to regen at if the gallons of capacity has not yet been exceeded | 14 days |
| Capacity in gallons | Gallons of capacity that the Clack controller displays immediately after a regen | 1200 gal. |

## Explanation of Operation

### Salt Measurement Logic
Since many water softeners have water above the salt (small salt reservoir), there is a smart function built into the distance measurement.

The sensor measures the distance downwards to the salt. That measured distance should be expected to only increase as the salt depletes.
Because of this, shorter distances are not recorded as the most recent value, only longer ones. This way, water rising above the salt does not affect the measurement.

There are two readings: Distance and TOF Distance.
Distance is the measurement with the smart function. TOF Distance is continuously measured every 10 seconds but has no effect on the 'Clack salt level percent' and does not affect Distance unless the it measures greater than Distance.

### Refilling Salt

When refilling your salt container, press the 'Reset fill salt' button on the dashboard. This will equalize the TOF Distance and Distance values, thereby resetting the distance measurement so that the new salt level can be measured and recorded again. Recall that the smart function would otherwise not let Distance increase.
