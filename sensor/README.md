# BLE sensor module
The sensor module runs on the WRL-13990 Bluetooth module, which is built around the nRF52832. Its purpose is to take readings from the temperature/humidity sensor and relay those readings back to the HomeKit bridge (see the bridge module below).

## Using this code
This is firmware code for the nRF52832. To use it, you need to compile it to binary code and then flash it to your device.

### Requirements
- The nRF5 SDK, which provides the necessary compilation resources and tools. Instructions to download it are in step 1 below.
- Installation of **version 0.5.2** of [Nordic's nrfutil program](https://github.com/NordicSemiconductor/pc-nrfutil). Installation instructions are provided at the source.

### Compilation & flashing

1. Download the [nRF5 SDK version 11.0.0](https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v11.x.x/nRF5_SDK_11.0.0_89a8197.zip).

       wget https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v11.x.x/nRF5_SDK_11.0.0_89a8197.zip

2. Unzip the SDK.
        
       unzip nRF5_SDK_11.0.0_89a8197.zip

3. Navigate to this path within the unzipped SDK folder: `examples/ble_peripheral/`.

       cd nRF5_SDK_11.0.0_89a8197/examples/ble_peripheral/

4. There, copy the entire folder containing this file, with the directory name `ble_app_temp_humid`.

       cp -R /path/to/repo/sensor ./ble_app_temp_humid

5. Build the code.

       cd ble_app_temp_humid/wrl13990/s132/armgcc
       make

   You should see build output as follows:

       rm -rf _build
       echo  Makefile
       Makefile
       mkdir _build
       Compiling file: app_button.c
       Compiling file: app_error.c
       Compiling file: app_error_weak.c
       Compiling file: app_fifo.c
       Compiling file: app_timer.c
       Compiling file: app_trace.c
       Compiling file: app_util_platform.c
       Compiling file: fstorage.c
       Compiling file: nrf_assert.c
       Compiling file: nrf_log.c
       Compiling file: retarget.c
       Compiling file: nrf_delay.c
       Compiling file: nrf_drv_common.c
       Compiling file: nrf_drv_gpiote.c
       Compiling file: pstorage.c
       Compiling file: bsp.c
       Compiling file: bsp_btn_ble.c
       Compiling file: main.c
       Compiling file: dht22.c
       Compiling file: RTT_Syscalls_GCC.c
       Compiling file: SEGGER_RTT.c
       Compiling file: SEGGER_RTT_printf.c
       Compiling file: ble_advdata.c
       Compiling file: ble_advertising.c
       Compiling file: ble_bas.c
       Compiling file: ble_bps.c
       Compiling file: ble_conn_params.c
       Compiling file: ble_dis.c
       Compiling file: ble_srv_common.c
       Compiling file: device_manager_peripheral.c
       Compiling file: system_nrf52.c
       Compiling file: softdevice_handler.c
       Assembly file: gcc_startup_nrf52.s
       Linking target: nrf52832_xxaa_s132.out
       Preparing: nrf52832_xxaa_s132.bin
       Preparing: nrf52832_xxaa_s132.hex

         text	   data	    bss	    dec	    hex	filename
         9332	    124	    768	  10224	   27f0	_build/nrf52832_xxaa_s132.out

6. Connect the WRL13990 to your computer and begin flashing the compiled firmware. I used the following command to accomplish that using a FT232RL-based USB-to-serial board.

       nrfutil dfu genpkg --application _build/*.hex pkg.zip && nrfutil dfu serial --package=pkg.zip --port=/dev/cu.usbserial-00000000 --baudrate=38400
   
   With the following output.

       Zip created at pkg.zip
       Upgrading target on /dev/cu.usbserial-00000000 with DFU package /Users/danielgross/Desktop/ard-temp-sensor/nrf52/nRF5_SDK_11.0.0_89a8197/examples/ble_peripheral/ble_app_temp_humid/wrl13990/s132/armgcc/pkg.zip. Flow control is disabled.
         [####################################]  100%          
       Device programmed.