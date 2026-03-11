Baremetal Behavioural Identification for IoT Devices
Overview

This project aims to develop a behaviour-based identity framework for low-end embedded IoT devices. Instead of relying only on traditional identifiers such as MAC addresses or device certificates, the system attempts to characterize a device by observing its runtime behaviour and system characteristics.

The project is currently focused on microcontroller platforms such as ESP32-S3 and STM32U585, but the architecture is designed to be portable across other low-power embedded systems.

A lightweight C-based firmware collects a wide range of system, network, and environmental data directly from the device, aggregates this information into a structured format, and transmits it to a remote server. The collected behavioural dataset can then be used to generate a device behavioural profile, enabling identification, anomaly detection, or security monitoring.

The implementation currently runs on bare-metal or RTOS-based embedded firmware, with minimal resource usage to ensure compatibility with constrained devices.

Project Objective

The primary objective of this project is to explore an alternative approach to device identification based on device behaviour rather than static identifiers.

Each IoT device exhibits unique characteristics during operation, including:

Resource usage patterns

Communication behaviour

Peripheral activity

Environmental sensor readings

Runtime system parameters

By collecting and analysing these characteristics over time, it becomes possible to create a behavioural identity model for each device.

Such an approach may be useful in areas such as:

IoT device authentication

Behaviour-based device fingerprinting

Anomaly detection in embedded systems

Security monitoring in distributed IoT networks

Asset identification in large-scale deployments

Current Implementation

The current firmware implementation is written in C and designed to run on embedded microcontrollers with minimal overhead.

The firmware performs the following operations:

Collects system-level data from the device.

Collects network-related parameters from the active connection.

Reads environmental sensor values from connected sensors.

Aggregates all collected information into a structured JSON payload.

Transmits the payload to a remote server using MQTT.

This information represents a snapshot of the device's behavioural state at a given time.

Data Collected from the Device

The firmware collects several categories of information from the embedded system.

1. Static Device Information

Static information refers to device properties that generally remain constant throughout the lifetime of the device.

Examples include:

Device identifier

Firmware version

Hardware platform

CPU frequency

Number of processor cores

Flash size

Chip revision

These parameters help identify the hardware and firmware configuration of the device.

2. System Resource Information

Runtime resource information provides insight into how the device is currently operating.

Examples include:

Total memory available

Free heap memory

Minimum heap ever available

CPU usage indicators

System uptime

This data helps characterize how the device utilizes its internal resources over time.

3. Network Information

Network data describes the device’s communication behaviour and connectivity state.

Examples include:

Connected WiFi SSID

Local IP address

MAC address

Network interface status

Packet transmission activity

These parameters are useful for understanding how the device interacts with the network and other systems.

4. Peripheral and Port Activity

The system can also monitor the status of communication interfaces available on the microcontroller.

Examples include:

I2C activity

SPI activity

UART communication status

CAN interface status

The system attempts to determine whether these interfaces are:

Active

Inactive

If activity is detected, packet or transaction information can also be recorded.

5. Environmental Sensor Data

Sensor data contributes to the behavioural fingerprint by describing the physical environment observed by the device.

Currently supported sensors include:

BME280

Temperature

Humidity

Atmospheric pressure

MQ135

Air quality indicator (AQI approximation)

Sensor data is periodically read and incorporated into the telemetry payload.

Data Transmission

All collected data is aggregated into a structured JSON payload before transmission.

Example structure:

{
  "device": {...},
  "system": {...},
  "network": {...},
  "sensors": {...},
  "ports": {...}
}

The payload is transmitted to a remote server using MQTT.

The current implementation uses standard MQTT, but the system is being designed to support Secure MQTT (sMQTT) in future iterations to provide encrypted communication and certificate-based authentication.

Communication Workflow

The current firmware follows this operational sequence:

Device boots and initializes system resources.

WiFi connection is established.

MQTT client connects to the configured broker.

Sensors and data collectors are initialized.

Behavioural data is collected at periodic intervals.

Data is formatted into a JSON structure.

JSON payload is transmitted to the remote server.

This process repeats periodically to continuously monitor the device's behaviour.

Project Architecture

The firmware is organized into several modules to maintain separation of responsibilities.

Typical modules include:

System Modules

Static data collection

Dynamic runtime data collection

Sensor Modules

BME280 environmental sensor driver

MQ135 gas sensor interface

Sensor manager

Network Modules

WiFi manager

MQTT manager

Data Handling

JSON builder

Behavioural data aggregator

This modular design allows new sensors or behavioural data sources to be added without modifying the core system.

Future Work

Several improvements are planned for future versions of this project.

Secure Communication

Implementation of Secure MQTT (sMQTT) using TLS encryption and certificate authentication.

Expanded Behavioural Metrics

Additional behavioural signals may include:

Peripheral bus traffic analysis

Power consumption patterns

Interrupt frequency

Task scheduling patterns (for RTOS systems)

Cross-Platform Support

The architecture will be extended to support multiple microcontroller platforms including:

STM32

ESP32

other ARM Cortex-M microcontrollers

Behavioural Identity Model

The collected behavioural data will eventually be used to develop machine learning models capable of identifying devices based on their behavioural patterns.

Target Platforms

The project is currently being tested on:

ESP32-S3

STM32U585

However, the architecture is designed to remain portable across other embedded platforms.

Current Status

The project is currently in the prototype stage.

Core features implemented so far include:

Behavioural data collection

Sensor integration

JSON data aggregation

MQTT-based data transmission

Further work is ongoing to improve modularity, security, and scalability.

Purpose of the Repository

This repository contains the firmware implementation for the behavioural data collection system running on embedded IoT devices.

The goal is to build a lightweight, extensible framework for behavioural telemetry collection that can be used to explore new methods of device identification and security monitoring in IoT environments.