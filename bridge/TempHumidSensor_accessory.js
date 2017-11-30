var Accessory = require('../').Accessory;
var Service = require('../').Service;
var Characteristic = require('../').Characteristic;
var uuid = require('../').uuid;

var noble = require('noble');

// Global
let sensorVals = {
  hum: null,
  temp: null
};

console.log("begin scanning...");

noble.on('stateChange', (state) => {
  if (state == 'poweredOn') {
    init();
  }
});

noble.on('discover', handleServicesDiscover);

function init() {
  noble.startScanning(['0x7B18'], true);
}

function handleServicesDiscover(peripheral) {
  if (peripheral.advertisement.serviceData
    && peripheral.advertisement.serviceData[0]
    && peripheral.advertisement.serviceData[0].uuid == '7b18') {

    let data = peripheral.advertisement.serviceData[0].data;

    let hum = convertBytesToDecimal(data[0], data[1]);
    let temp = convertBytesToDecimal(data[2], data[3]);

    console.log('Humidity: ' + hum + '%, Temp: ' + temp + ' deg. C');

    sensorVals.hum = hum;
    sensorVals.temp = temp;
  }

}

function convertBytesToUint16(msb, lsb) {
  return (msb << 8) | lsb;
}

function convertReadingToDecimal(intVal) {
  return intVal / 10;
}

function convertBytesToDecimal(msb, lsb) {
  return convertReadingToDecimal(convertBytesToUint16(msb, lsb));
}

// here's the sensor device that we'll expose to HomeKit
var TEMP_HUMID_SENSOR = {
  currentTemperature: 0,
  getTemperature: function () {
    console.log("Getting the current temperature!");
    return FAKE_SENSOR.currentTemperature;
  },
}


// Generate a consistent UUID for our Temperature Sensor Accessory that will remain the same
// even when restarting our server. We use the `uuid.generate` helper function to create
// a deterministic UUID based on an arbitrary "namespace" and the string "temperature-sensor".
var sensorUUID = uuid.generate('hap-nodejs:accessories:temperature-sensor');

// This is the Accessory that we'll return to HAP-NodeJS that represents our fake lock.
var sensor = exports.accessory = new Accessory('Temperature Sensor', sensorUUID);

// Add properties for publishing (in case we're using Core.js and not BridgedCore.js)
sensor.username = "D1:5D:3A:AE:5E:FA";
sensor.pincode = "031-45-154";

// Add the actual TemperatureSensor Service.
// We can see the complete list of Services and Characteristics in `lib/gen/HomeKitTypes.js`
sensor
  .addService(Service.TemperatureSensor)
  .getCharacteristic(Characteristic.CurrentTemperature)
  .on('get', function (callback) {

    // return our current value
    callback(null, TEMP_HUMID_SENSOR.getTemperature());
  });

// randomize our temperature reading every 3 seconds
setInterval(function () {

  TEMP_HUMID_SENSOR.currentTemperature = sensorVals.temp;

  // update the characteristic value so interested iOS devices can get notified
  sensor
    .getService(Service.TemperatureSensor)
    .setCharacteristic(Characteristic.CurrentTemperature, TEMP_HUMID_SENSOR.currentTemperature);

}, 1000);
