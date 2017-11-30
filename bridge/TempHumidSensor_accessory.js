var Accessory = require('../').Accessory;
var Service = require('../').Service;
var Characteristic = require('../').Characteristic;
var uuid = require('../').uuid;

var noble = require('noble');

var TEMP_HUMID_SENSOR = {
  currentTemperature: 0,
  currentHumidity: 0,
  getTemperature: function () {
    console.debug("Getting temperature for HomeKit");
    return TEMP_HUMID_SENSOR.currentTemperature;
  },
  getHumidity: function () {
    console.debug("Getting humidity for HomeKit");
    return TEMP_HUMID_SENSOR.currentHumidity;
  },
}

console.debug("begin scanning for BLE readings...");

noble.on('stateChange', (state) => {
  if (state == 'poweredOn') {
    init();
  }
});

noble.on('discover', handleServicesDiscover);

function init() {
  noble.startScanning([], true);
}

function handleServicesDiscover(peripheral) {
  if (peripheral.advertisement.serviceData
    && peripheral.advertisement.serviceData[0]
    && peripheral.advertisement.serviceData[0].uuid == '7b18') {

    const data = peripheral.advertisement.serviceData[0].data;

    const hum = convertBytesToDecimal(data[0], data[1]);
    const temp = convertBytesToDecimal(data[2], data[3]);

    console.debug('Humidity: ' + hum + '%, Temp: ' + temp + ' deg. C');

    TEMP_HUMID_SENSOR.currentHumidity = hum;
    TEMP_HUMID_SENSOR.currentTemperature = temp;
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

var sensorUUID = uuid.generate('hap-nodejs:accessories:temperature-humidity-sensor');

var sensor = exports.accessory = new Accessory('Temperature/Humidity Sensor', sensorUUID);

sensor.username = "D1:5D:3A:AE:5E:FB";
sensor.pincode = "031-45-154";

sensor
  .addService(Service.TemperatureSensor)
  .getCharacteristic(Characteristic.CurrentTemperature)
  .on('get', function (callback) {
    callback(null, TEMP_HUMID_SENSOR.getTemperature());
  });

sensor
  .addService(Service.HumiditySensor)
  .getCharacteristic(Characteristic.CurrentRelativeHumidity)
  .on('get', function (callback) {
    callback(null, TEMP_HUMID_SENSOR.getHumidity());
  });

setInterval(function () {
  sensor
    .getService(Service.TemperatureSensor)
    .setCharacteristic(Characteristic.CurrentTemperature, TEMP_HUMID_SENSOR.getTemperature());

  sensor
    .getService(Service.HumiditySensor)
    .setCharacteristic(Characteristic.CurrentRelativeHumidity, TEMP_HUMID_SENSOR.getHumidity());
}, 3000);
