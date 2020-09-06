### Making a water level indicator with single LED using AJ-SR04M Ultrasonic Sensor

## Requirments
*   Ultrasonic Sensor AJ-SR04M
*   Node-mcu (0.9 used)
*   Node-Red to Easily create Mqtt Pub/Sub Client

## Wiring
# Node-MCU -  AJ-SR04M  -   LED
D1      -   Trig
D2      -   Echo
D3      -    -    - LED(+ve)
5V/VU   -   5V
GND     -   GND   - GND

## NODE-RED
MQTT in[/ha/spub]   -to-    Exit Text Node //To get RAW Distance Data
Input Text          -to-    MQTT out[/ha/sred]  //Input int Value to Set Distance
MQTT in[/ha/sred]   -to-    Exit Text Node  //MQTT Out Data
MQTT in[/ha/limit]   -to-    Exit Text Node //Defined Limit Distace

## Versions and files
# Using Arduino and Node-mcu
Ultrasonic_ESP
Ultrasonic_UNO

# Using Only Node-mcu
UltraSonic_OnlyESP

# Using Only Node-MCU with EEPROM Limit Saving
UltraSonic_OnlyESP_EEPROM

# Picture of Node-Red
NODE-MAIN
NODE-UI

# Picture of Project
MAIN
EXTRAS-*
