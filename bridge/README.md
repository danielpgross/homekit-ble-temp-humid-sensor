# HomeKit bridge module
The bridge module can run on any device capable of Bluetooth and wifi communication, as well as running node.js. The bridge's purpose is to receive the temperature readings broadcast by the sensor module and retransmit those over the HomeKit protocol.

## Using this code
The code is a node.js module that is written to be integrated into an instance of [HAP-NodeJS](https://github.com/KhaosT/HAP-NodeJS).

### Requirements
The following applications and libraries need to be present in order for this module to work:
- node.js and npm
- Libraries for building HomeKit and Bluetooth components. These can be installed on Ubuntu/Debian using: 

      apt-get install build-essential python libavahi-compat-libdnssd-dev
      npm install -g node-gyp

### Installation
If you have an instance of HAP-NodeJS, you can skip to step 3.

1) Install HAP-NodeJS.

       git clone https://github.com/KhaosT/HAP-NodeJS.git
       cd HAP-NodeJS

2) (recommended) Remove sample accessories from HAP-NodeJS instance.

       rm accessories/*_accessory.js

3) Copy TempHumidSensor_accessory.js into accessories folder.

       cp /path/to/repo/bridge/TempHumidSensor_accessory.js accessories/

4) Install node modules.

       npm install

4) Manually install the `noble` BLE module in the HAP-NodeJS root. This is required for the temperature/humidity accessory.

       npm install noble

5) Finally, start HAP-NodeJS.

       node Core.js