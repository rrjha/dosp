Introduction
============

With changing life styles and hectic urban living, the sleep quality of people has deteriorated over time. This system aims at providing information on sleep quality, environment parameters to help identify issues and propose automated mechanisms to resolve these.

Details
============
To implement this puses accelerometer to detect motions indicative of activity/poor sleep. It uses temperature sensor to determine the environment temperature.
To achieve this we define and use a new composite sensor called “sleep/active”.
To indicate the actuation of environment it uses LED as symbolic of devices that can be controlled remotely e.g. thermostat or aircon.
This data is periodically collected and sent via publish/subscribe mechanism in an efficient fashion by checking previous state. This data once available can be consumed by application running on edge or can be relayed to subscribers registered using RESTfull APIs (or formatted request structure like C struct for constrained devices).

Interfaces
============
I/O interface uses standard device independent I/O calls to use well tested and minimal Time-To-Market interface.
High level driver developed using this with help of DDL provides more abstracted view without providing internal gory details like pinout for on-board sensors.
In addition to sensors on board, the system also provides a composite sensor service to external entities via partial MQTT implementation that uses publish and subscribe mechanism.
This ensures that devices are only notified of events of interest to each device and spurious events are eliminated.
Discovery is performed through a central broker server to maximize coordination and overall flexibility as opposed to broadcast methods.
The server plays the role of message broker and ensures that the messages are all delivered quickly and efficiently.

IoT Concerns Addressed
======================
Device minimizes server communication using a simple protocol with small 69-byte messages. Most devices will go for many minutes between communications and may choose to sleep for much of that time. Latency, performance, and scalibility are assured by using an event-driven non-blocking server model backed by a popular SQL database capable of handling thousands of clients at a time.
Some of the salient features of bandwidth and energy efficient interface includes:
1) Energy:
a. Utilizes low frequency components e.g. SPI (clocked at 1.5 KHz) accelerome-ter
b. On demand acquisition of sensor data using IO calls (e.g. init, read)
2) Limited data communication bandwidth (and energy) and memory requirement:
a. Use of publish subscribe mechanism (partial MQTT implementation)
b. Small on-board footprint
c. Efficient push after compare logic to push only when needed
3) Scalability: The Daemon is capable of scaling up for numerous more devices(sensors).

HW component and drivers implemented
====================================
Temperature Sensor (Analog):
Temperature sensor uses on-demand data to reduce energy consumption in data acquisition. It utilizes a read call exposed by driver. Development required bringing up ADC module on BBB which utilizes standard driver structure of XINU. Temperature sensor uses one of the ADC pins available (0-7). It uses analog sensor component (TMP36). Temperature sensor provides simple wrapper calls to ADC to make it more intuitive for operations typically supported by sensor. DDL wraps this implementation to provide details like on-board pinout for sensor and calls to underlying LL device driver for sensor.

Accelerometer (Digital):
Accelerometer uses on-demand data to reduce energy consumption in data acquisition. The sensor is connected to low frequency SPI bus clocked at 1.5 KHz to keep the energy requirement to minimum.
The digital accelerometer component used is ADXL345. The effort spent was to enable and configure SPI channel for communication with sensor and setup the device for data acquisition. The SPI driver and accelerometer driver uses standard XINU device independent IO with init and read calls supported.
DDL wraps the driver with simple calls to initialize and read sensor data as required using Xinu read calls.

LED (LFH3360 – Actuator):
LED function is driven by custom driver for BBB that allows using an onboard GPIO and mux the same for output and drive the GPIO output. GPIO driver is implemented in XINU’s device independent IO. LED driver implements a simple wrapper on top of it presenting more intuitive APIs like ledon and ledoff. DDL supplies the wrapper around low level device driver to abstract details like pin config for LED which it obtains from the DDL XML files.

Sleep sensor (Composite sensor):
At the heart of the idea lies a composite sensor, sleep sensor that utilizes the data acquired from accelerometer data over a period to determine the status of sleep of a person (sleeping/active) and also determine environment conditions like (temperature).
This data is published to server for devices trying to listen for sleep service.
Additionally the device also looks if the data being published is different from last status and only then publishes it.
A last state hysteresis is saved to avoid redundant updates, hence saving cost.
This allows powerful abstraction of sleep metrics obtained at a very low energy cost. The listening applications can then interpret this data to provide personalized feed.
This is the basis of value creation and service monetization of the project.

Edge and Web externalization interface
======================================
Publish/Subscribe (Partial MQTT)
Adaptation to existing model of discovery mechanism, to expose ervices provided.
Use of a 'service' based model, to facilitate easy integration with existing industry models. (BLE uses service model)

All address are determined dynamically when a device connects to the server. A simple handshake follows, and the server generates a unique ID which is then set to the device. The server maintains device registrations and provides external access to device functionality through an additional external REST API separate from the P/S system used by devices.
Device configuration is performed dynamically by the server and by devices using the Publish/Subscribe mechanism to indicate topics of interests, simply writing to topics on which the device intends to provide its services.

Connection is performed by subscribers and performed through the REST API for normal entities.
For constrained devices like XINU we support a formatted data stream (like a C struct) that defines subscription data.
The producer of data also notifies its availability to server (broker at edge) via a formatted data (C struct).
The sensor data generated, is published by the device, only when it changes (change exceeding threshold) so that communication is minimal.
The update is relayed by broker sitting on the edge to subscribing entities.
In a similar fashion actuator is exposed as a service that a requesting entity can send a request (REST or formatted input e.g. C struct) to trigger it.

Requirements
============

0. i386 Linux to run the provided cross-compiler
0. Python 3.5
0. aiohttp, installable via ```pip3 install aiohttp```. You may have to run ```sudo aptitude install python3-pip``` to install pip3 or ```sudo pip3 install wheel``` if you encounter errors.
0. requests python library, installable via the same process as above
