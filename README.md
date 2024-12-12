# Clack Reader v4
 Esphome component for Clack WS PI (disc valve) Ecosoft LESS and WS1 softener with M5stack TOF sensor saltlevel detection
 
 Relay for control of the chlorinator module from AQMOS
 and Power measurement with ina3221

 Automatic recognision of the softener steps and measure the liters and m3 softened water by reading the clack flowmeter

## 3 Versions / Branches
main: Clack WS PI valve (DiscValve) (ECOSOFT LESS-10 / 15 / 20 from AQMOS)

ws1: Clack WS1 valve (CM(x) from AQMOS)

ws1_usa: Clack WS1 valve with Gallons and Inches / US version

Remark: Make sure to copy water_flow_gallons.h and tof_vl53l1x.h to your esphome directory before compiling the clack.yaml
There have been some bigger updates to the latest code: added L/min and alarm, also lovelace home assistant menu has been updated.

Remark2: 6 august 2024: Added history regeneration and test button motor pulse to step through regeneration (with time delays stil working between steps)

Remark3: 17 august 2024: Added more options to show the regeneration cycle. Pre fill (dry salt tank) and Post fill (wet salt tank)
Both with or without the 2nd backwash programmed  (between brine and rinse)

      # Pre fill no 2nd Backwash  : FILL/SERVICE/BACKWASH/BRINE/RINSE
      # Pre fill 2nd Backwash     : FILL/SERVICE/BACKWASH/BRINE/BACKWASH2/RINSE
      # Post fill no 2nd Backwash : BACKWASH/BRINE/RINSE/FILL
      # Post fill 2nd Backwash   : BACKWASH/BRINE/BACKWASH2/RINSE/FILL   

## English:
[README](/readme/clack_explanation_en.md)

## Contact
Purchase: alphonsuijtdehaag at gmail dot com, if you are interested in a PCB with M5Stack Atom s3 lite

Help: Please open a issue on this git project.

![Example](/readme/pcb_transp.png)


## Setup
```
substitutions:
  # General substitutions:
  name: clack
  friendly_name: Clack
  device_description: "Esphome component for Clack WS1 softener with TOF sensor saltlevel detection"
  timezone: "Europe/Amsterdam"
  # Interval substitutions:
  salt_level_update_interval: 30s
  watermeter_update_interval: 600s # 10 minutes
  tank_percentage_update_interval: 600s
  clack_height_update_interval: 600s

### In order to use other language fix clack-label filename below:
### Currently supported languages are: en (English), nl (Dutch) and fr (French).
### ESP32-Atom-s3 is a M5STACK Atom s3 lite
### tof1 is a VL53L0X M5stack. Distance measure up to 2 mtr. I2C grove cable max 5 mtr length
### tof2 is a VL53L1X M5stack (ToF4M). Distance measure up to 4 mtr. I2C grove cable max 2 mtr length

packages:
  remote_package:
    url: https://github.com/fonske/clack-reader-v4
    ref: ws1_usa
    refresh: 0s
    files: [ 
      esphome/clack_ws1_usa/.clack-base.yaml,
#      esphome/clack_ws1_usa/.tof1.yaml,
      esphome/clack_ws1_usa/.tof2.yaml,
      esphome/clack_ws1_usa/.clack-optional.yaml,
      esphome/clack_ws1_usa/.clack-labels-en.yaml,
      esphome/clack_ws1_usa/board-esp32-atom-s3.yaml,
    ]

### for developing/testing, uncomment local includes and comment out above remote_package packages part.
#packages:
##  substitutions: !include clack_ws1_usa/.clack-labels-en.yaml
#  device_base1:  !include clack_ws1_usa/board-esp32-atom-s3.yaml
#  device_base2:  !include clack_ws1_usa/.clack-base.yaml
##  device_base3: !include clack_ws1_usa/.tof1.yaml
#  device_base3:  !include clack_ws1_usa/.tof2.yaml
#  device_base4:  !include clack_ws1_usa/.clack-optional.yaml

time:
  - platform: sntp
    id: sntp_time
    timezone: ${timezone}
    # Enable this with your own NTP server if your device is on a no internet IOT lan. 
    #servers: 192.168.X.X
#    servers:
#     - 0.pool.ntp.org
#     - 1.pool.ntp.org
#     - 2.pool.ntp.org   

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  fast_connect: true
  power_save_mode: light
  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "${name}"
    password: "configesp"
    ap_timeout: 15s

# Enable Over The Air Updates
ota:
  platform: esphome
#  password: !secret clack_ota_password

# Enable captive portal
captive_portal:
  
# Enable logger
logger:
  baud_rate: 0
  logs:
    esp32.preferences: DEBUG
    sensor: DEBUG
    text_sensor: DEBUG
    binary_sensor: DEBUG
    switch: DEBUG
    button: DEBUG
    number: DEBUG
    component: ERROR

# Enable Web server.
web_server:
  port: 80
```

## Installation
You will first need to do a manual installation by putting the clack.yaml file into your esphome folder then using the modern format in ESPHome to get a local copy of the firmware and finally use https://web.esphome.io/ to install over USB.
Also copy the files tof_vl53l1x.h and water_flow_gallons.h to you're esphome directory as they are needed for the flow meter calculation and distance TOF sensor.

### Method
Download a copy of this code and place it in your esphome folder. Also place the clack.yaml - secrets.yaml - water_flow_gallons.h and tof_vl53l1x.h into the esphome folder, ensuring to change the secrets.yaml details to your own credentials.

From your ESPHome dashboard, create a local copy of the s3 firmware by clicking the three dots > Install > Manual Download > Modern Format

To set the board into flash mode, hold the button on the left of the USB port while plugging in the USB cable that is connected to your desktop machine.

Navigate to https://web.esphome.io/ in a compatible browser (Chrome, Edge etc.) and click "Connect". Select the corresponding COM Port and click connect again. 

When the board has connected, click "install" and select the firmware that you have created, this will erase the board and write the new firmware.

When the firmware has been written to the board you will need to unplug and reconnect the USB cable.

After that you can upload firmwares using WIFI without repeating this process

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/ebbenberg)
