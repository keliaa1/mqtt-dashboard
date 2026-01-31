.

📡 ESP8266 IoT Monitoring & Control System (MQTT + Web Dashboard)
📖 Project Description

This project is a complete IoT monitoring and control system built using an ESP8266 microcontroller, MQTT messaging, and a web-based dashboard hosted on a VPS.

The ESP8266 collects real-time temperature and humidity data from a DHT11 sensor and publishes it to an MQTT broker. A public web dashboard connects to the broker using MQTT over WebSockets, allowing users to monitor live sensor data, control an LED remotely, and view the device’s online/offline status.

The system implements MQTT best practices such as unique device identification, Last Will and Testament (LWT) for presence detection, and retained messages to ensure reliability and real-time synchronization between the device and dashboard.

⚙️ Technologies Used
🔹 Hardware

ESP8266 (NodeMCU)

DHT11 Temperature & Humidity Sensor

LED + Resistor

Breadboard & Jumper Wires

🔹 Firmware & Embedded

MicroPython

MQTT (raw TCP)

ESP8266 Wi-Fi networking

🔹 Backend / Messaging

Mosquitto MQTT Broker

MQTT over TCP (port 1883)

MQTT over WebSockets / Secure WebSockets (port 9002)

VPS hosting (Benax VPS)

🔹 Frontend / Dashboard

HTML, CSS, JavaScript

MQTT.js

MQTT over WebSockets (browser-compatible)

Real-time data visualization

Remote LED ON/OFF control

Device presence and “Last Updated” timestamp display

🚀 Key Features

📊 Live temperature & humidity monitoring

💡 Remote LED control with state feedback

🌐 Publicly accessible web dashboard

🔄 Real-time MQTT communication

🟢 Online/Offline device status via MQTT LWT

🕒 Accurate “Last Updated” timestamps

🆔 Unique ESP8266 device identity

🔗 Live Demo

👉 Public Dashboard:
https://broker.benax.rw/ (replace with your exact dashboard URL)
