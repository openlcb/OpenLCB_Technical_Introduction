# Getting Started with OpenMRN

This chapter will get you to the point where you have your own application, in your own directory, working as an OpenLCB node. We'll start with some OpenMRN sample applications to ensure you have the hardware working. Then we'll set you up with your own application, including your own node IDs.

## Setting up the Nucleo

The jumper JP5 controls the power supply. Make sure it's set to E5V so it uses the external 5V from the DevKit for power instead of power from the USB (Nucleo).

## Setting up Nucleo Software

I chose to start with the STM32F303 along with Balazs' DevKit board. The first step is to get a sample application running from OpenMRN to make sure you have everything set up correctly. If this is your first time building for the Nucleo, you'll need to install some software from ST.

Got to openmrn/applications/io_board/targets/freertos.armv7m.st-stm32f303re-nucleo
Type make

* Open https://www.st.com/en/embedded-software/stm32cubef3.html
* Click on **Get Software** to download their SDK

Next we'll copy the directory inside this ZIP file into the `/opt` directory. We need to set this up in a specific way that we'll explain shortly.

* `cd /opt`
* `sudo mkdir st`
* `sudo chown john st` -- you'll want to replace `john` with your user name
* `cd st`
* `mkdir STM32Cube_FW_F3`
* Copy the folder in the downloaded ZIP into the `STM32Cube_FW_F3` folder
* create a symblink called /opt/st/default to this directory. For example:
  
  /opt/st/STM32Cube_FW_F3$ `ln -s STM32Cube_FW_F3_V1.11.0/ default`

The build system for OpenMRN will look for the files in `/opt/st/STM32Cube_FW_F3/default`.

### Setting up to use CAN

Change to the following directory in OpenMRN: `applications/io_board/targets/freertos.armv7m.st-stm32f303re-nucleo` and the open `main.cxx`. You'll want to change the defines to use CAN, as shown here:

```cpp
// These preprocessor symbols are used to select which physical connections
// will be enabled in the main(). See @ref appl_main below.
// #define SNIFF_ON_SERIAL
//#define SNIFF_ON_USB
#define HAVE_PHYSICAL_CAN_PORT
```

Next you'll want to build it. Inside the folder above, type `make` to build the `bin` file. Once it's built, you'll want to copy it to the Nucleo board, which you can do by copying the `bin` file to the thumbdrive that appeared when you attached the Nuleo. On my machine, it looks like this:

```bash
cp io_board.bin /media/john/NODE_F303RE/

```

You'll see the large LDI/COM LED blink between green and red a few times while the `bin` file is copied down. Once it finishes copying, disconnect the USB and then power it back up with power from the DevKit. If the DevKit is connected to the LCC bus (I'm using the RR-CirKits LCC Buffer-USB), you should see the terminator blick blue a couple of times at startup. You can see this again by pressing the reset button on the Nucleo, which will result in the startup message being sent again.

You can also press the blue button, which will send a message on press, and a message on release. Again you should see the blue LED on the terminator flash.

## Monitoring Messages in JMRI

You'll need to create an OpenLCB connection in JMRI:

* System manufacturer: OpenLCB
* System connection: CAN via LCC Buffer-USB
* Serial port: ttyACM0

## Setting up your Application

* Create a directory
* git init


* Copy files from applications/empty_app into your new directory

You'll need to let your application to know where to find OpenMRN. You can do this by creating a `openmrnpath.mk` with content something like this:

```makefile
OPENMRNPATH=$(HOME)/openmrn
```

Next we need to copy over the target files for the Nucleo so we can build the `bin` for the Nucleo:

```bash
~/src/openlcb/sample$ cp -L -r ~/tcs_booster/deps/openmrn/applications/nucleo_io/targets/freertos.armv7m.st-stm32f303re-nucleo-dev-board targets/
```

The `-L` will copy the actual files that are pointed to by a symlink into your directory.

Edit `targets/Makefile`:

```makefile
SUBDIRS = \
 freertos.armv7m.st-stm32f303re-nucleo-dev-board

#	freertos.armv7m.ek-tm4c123gxl \
#	freertos.armv7m.ek-tm4c1294xl \
#	freertos.armv7m.lpc1768-mbed \
#	linux.x86 \

include $(OPENMRNPATH)/etc/recurse.mk
```

Run make again and then copy onto your Nucleo:

```bash
targets/freertos.armv7m.st-stm32f303re-nucleo-dev-board$ make -j3
targets/freertos.armv7m.st-stm32f303re-nucleo-dev-board$ cp sample.bin /media/john/NODE_F303RE/
```

Press the reset button on the Nucleo and it should blink the blue LEDs again.

You should commit your changes to GIT.cd

## Node IDs

You'll need to ensure that the node IDs on your nodes are unique. Every NMRA member and DCC manufacturer already has a block of node IDs allocated to them. You can also register your own range if in case you don't have an NMRA or DCC number.
