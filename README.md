**GCScale: Gravity Center Scale**


## Overview

GCScale is a project that began with the aim of creating a balance scale inspired by the Wii Balance Board for use in physiotherapy. Faced with a choice between utilizing the Wii Balance Board and extracting data using Java or Python, or building custom hardware, I opted to create a bespoke solution based on an ESP8266, the HX711 load cell module, and full-bridge load cells. 

This unique scale not only displays the user's body weight but also visualizes their center of gravity within a coordinate system. The scale includes recording capabilities and features an automatic power-off function after 5 minutes of inactivity.

The software technology employed encompasses HTML, Javascript, JSON, and the Arduino IDE.

## Key Features

- **Weight Measurement:** GCScale accurately measures the user's weight, providing essential data for monitoring health and progress.

- **Center of Gravity Visualization:** The standout feature of GCScale is its ability to visualize the user's center of gravity within a coordinate system. This feature aids in understanding weight distribution and balance.

- **Hardware Choice:** GCScale is built on an ESP8266 microcontroller, HX711 load cell module, and full-bridge load cells, representing a tailored hardware solution.

- **Recording Capability:** GCScale allows users to record their measurements during a session to show the movement during exercise.
  
- **Auto Power-Off:** To conserve energy, the scale automatically powers off after 5 minutes of inactivity.

## Repository Contents

- **/hardware:** This directory contains the hardware design files, including schematics, PCB layouts, and relevant documentation.

- **/software:** Here, you'll find the software code necessary to operate GCScale. This includes the user interface code and algorithms for calculating the center of gravity.

## Getting Started

To begin using GCScale:

1. **Clone the Repository:** Clone this repository to your local machine.

2. **Hardware Setup:** Consult the documentation in the `/hardware` directory for instructions on assembling the custom hardware components.

3. **Software Installation:** Follow the instructions in the `/software` directory to set up and run the software interface.

4. **Contribute:** If you're interested in contributing to GCScale, you're welcome to fork the repository, make enhancements, and submit pull requests.

---

Thank you for your interest in GCScale! We hope this project proves to be a valuable asset in the realm of physiotherapy and beyond.
