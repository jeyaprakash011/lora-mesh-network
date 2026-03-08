# lora-mesh-network
ESP32 LoRa Mesh Network for long-range sensor communication with multi-hop routing, GPS telemetry, PIR motion detection, sound detection, and temperature monitoring.
# ESP32 LoRa Mesh Network with Multi-Hop Communication

## Project Overview

This project implements a **LoRa Mesh Communication Network using ESP32 and LoRa modules** to transmit sensor data over long distances using **multi-hop routing**.

The system consists of:

• **Sender Node** – collects sensor data  
• **Relay Nodes** – forward packets across the network  
• **Receiver Node** – receives and displays final data  

Each node acts as a **router**, allowing the system to extend communication range beyond a single LoRa link.

The sender node collects environmental data including:

- GPS location
- Temperature
- PIR motion detection
- Sound detection

This data is transmitted through **intermediate LoRa nodes** until it reaches the receiver node.

---

## System Architecture

![System Setup](images/system_setup.png)

The system includes **four nodes**:

1️⃣ Sender Node (Sensor Node)  
2️⃣ Relay Node 1  
3️⃣ Relay Node 2  
4️⃣ Receiver Node  

Packets are forwarded across nodes using **multi-hop mesh communication**.

---

## Key Features

- ESP32 based LoRa communication
- Multi-hop mesh packet forwarding
- Packet routing using node IDs
- GPS location tracking
- Motion detection using PIR sensor
- Sound detection sensor
- Temperature monitoring
- LCD display output
- Long-range wireless communication
- Low power IoT network design

---

## Hardware Components

- ESP32 Development Boards
- SX1278 LoRa Module (433 MHz)
- GPS Module (NEO-6M)
- PIR Motion Sensor
- Sound Sensor
- Temperature Sensor (DS18B20)
- 16x2 LCD Display
- Power Supply
- Connecting Wires

---

## Network Node Design

### Sender Node

The sender node collects environmental data and transmits packets.

Sensors used:

- GPS Module
- PIR Motion Sensor
- Sound Sensor
- Temperature Sensor

The sender transmits packets in the format:

```
SRC | DEST | LAST_HOP | PACKET_ID | HOP | MESSAGE
```

Example:

```
1|4|1|10|0|LT:12.345 LN:76.973 T:27.3
```

---

### Relay Nodes

Relay nodes forward packets to extend network range.

Each relay node:

- Receives packet
- Increments hop counter
- Updates last hop ID
- Forwards packet again

This allows the network to reach nodes outside direct radio range.

---

### Receiver Node

The receiver node receives packets and displays data on the LCD.

Displayed data includes:

- GPS Latitude
- GPS Longitude
- Temperature
- Motion detection alerts
- Sound detection alerts

---

## Packet Structure

Each LoRa packet follows this format:

```
SRC | DEST | LAST_HOP | PACKET_ID | HOP | MESSAGE
```

Where:

SRC → Source node ID  
DEST → Destination node ID  
LAST_HOP → Last forwarding node  
PACKET_ID → Unique packet number  
HOP → Number of hops taken  
MESSAGE → Sensor data

---

## Software

### Microcontroller
ESP32

### Programming Language
Embedded C (Arduino Framework)

### Development Environment
Arduino IDE

### Libraries Used

- SPI
- LoRa
- TinyGPS++
- DallasTemperature
- OneWire
- LiquidCrystal

---

## Working Process

1️⃣ Sender collects sensor data  
2️⃣ Data packet is created  
3️⃣ Packet is transmitted via LoRa  
4️⃣ Relay nodes receive the packet  
5️⃣ Each relay forwards the packet  
6️⃣ Packet eventually reaches receiver node  
7️⃣ Receiver displays the sensor data  

---

## Example Data Transmission

Sensor packet example:

```
LT:12.345 LN:76.973 T:27.3
```

Event packet example:

```
Motion Detected
```

or

```
Sound Detected
```

---

## Applications

- Long range IoT sensor networks
- Agricultural monitoring systems
- Wildlife monitoring
- Smart farming
- Disaster communication networks
- Remote environmental monitoring

---

## Future Improvements

- Dynamic routing algorithm
- Node auto-discovery
- AES encrypted LoRa packets
- Cloud IoT integration
- Mobile dashboard monitoring

---

## Author

**Jeyaprakash V**  
Embedded Systems & IoT Developer
