# Water-Turbidity

This project monitors the turbidity level of water using a turbidity sensor connected to an Arduino Uno. The turbidity level is displayed on a 16x2 LCD screen and indicated by LEDs of various colors.

## Description

The project uses a turbidity sensor to measure water quality. The readings are displayed on an LCD screen, and the turbidity level is indicated through the following LEDs:
- Green LED: Clean water
- Yellow LED: Cloudy water
- Red LED: Dirty water

## Hardware

- 1x Arduino Uno
- 1x Turbidity sensor
- 1x 16x2 LCD screen (with I2C module)
- 3x LEDs (green, yellow, red)
- 3x Resistors
- 1x Breadboard
- 1x Connection cable and wires

## Connections

- Sensor module: Pins 1, 2, 3
- Sensor module to board: Analog pin A0
- I2C LCD: SDA -> A4, SCL -> A5
- Green LED: Digital pin 2
- Yellow LED: Digital pin 4
- Red LED: Digital pin 6
