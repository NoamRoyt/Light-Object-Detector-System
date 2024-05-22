# Light-Object-Detector-System
This project, "Detector System for Light Sources and Object Proximity," is designed to detect light sources and monitor object proximity using an MCU-based system. The system integrates an ultrasonic distance meter and light sensors (LDR) with a servo motor to scan the environment within a 180-degree range. 


## Introduction

This project involves designing and implementing a system based on an MCU to detect light sources and monitor objects in space. It communicates with a PC via asynchronous serial communication (RS-232), enabling user interface interaction and data presentation.

## Features

- **Object Detection:** Scans the environment to detect objects with high precision.
- **Telemeter:** Measures and displays the distance of an object from the user at a specific angle.
- **Light Detection:** Monitors light sources within the environment, presenting light intensity at different angles.
- **Combined Detection:** Simultaneously detects both objects and light sources in a single scan.

## Hardware Components

- MCU (Microcontroller Unit)
- Ultrasonic Sensor for distance measurement
- LDR Sensors for light detection
- Servo Motor for rotational scanning
- RS-232 for PC communication

## Software Components

- PC Interface: Written in Python, allows users to initiate scans, view results, and interact with the system.
- MCU Firmware: Written in C, handles sensor data acquisition, motor control, and communication with the PC.
