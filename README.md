Wearable Air Quality Pendant 🌬️
================================

A compact, battery-powered wearable device that continuously monitors air quality using TVOC detection. Get real-time feedback about your breathing environment through intuitive LED indicators and audio alerts.

![Wearable Air Quality Pendant](https://claude.ai/chat/images/main-project-image.jpg)

Project Overview
-------------------

This DIY air quality monitor pendant uses professional-grade sensors to track Total Volatile Organic Compounds (TVOC) in real-time. Perfect for monitoring indoor air quality at home, office, or industrial environments.

### Key Features

-    **Real-time TVOC monitoring** using SGP40 sensor
-    **Ultra-low power design** with STM32U083KCU6 microcontroller
-    **Intuitive feedback** - Color-coded LEDs and musical alerts
-    **Wearable design** - Compact 32.5mm diameter pendant
-    **All-day battery life** with efficient power management
-    **Professional accuracy** with industrial-grade sensor

 Air Quality Classifications
------------------------------

| TVOC Level | Status | LED Color | Audio Alert |
| --- | --- | --- | --- |
| 0-200 | Good | 🟢 Green | Soft tone |
| 201-300 | Warning | 🟡 Yellow | Warning melody |
| 301-500 | Alarm | 🔴 Red | Urgent beeping |
| >500 | Error | 🔴 Flashing Red | Error tone |

 Hardware Components
-----------------------

### Main Components

-   **STM32U083KCU6** - Ultra-low-power microcontroller
-   **SGP40-D-R4** - Professional TVOC air quality sensor
-   **MCP73832T** - Li-Po battery charge controller
-   **ADPL44002** - Low-dropout voltage regulator
-   **EAST1616RGBA1** - RGB LEDs (4x)
-   **PKMCS0909E4000-R1** - Piezoelectric buzzer

### Power Management

-   Li-Po battery charging via USB-C
-   Regulated 2.5V supply for stable operation
-   Ultra-low power consumption for all-day use

📁 Repository Structure
-----------------------

```
├── Code/
│   └── Wearable_Air_Quality_Pendant/
│       ├── Wearable_Air_Quality_Pendant.ino
│       └── libraries/
├── PCB/
│   ├── Wearable-Air-Quality-Pendant.kicad_pro
│   ├── Schematic.pdf
│   └── Gerber_Files/
├── 3D_Models/
│   ├── Pendant_Top.stl
│   ├── Pendant_Bottom.stl
│   └── Assembly_Guide.pdf
├── Documentation/
│   ├── Circuit_Diagram.pdf
│   ├── Assembly_Instructions.pdf
│   └── BOM.csv
└── images/

```

🚀 Quick Start Guide
--------------------

### 1\. Hardware Assembly

1.  **Order PCB**: Use provided Gerber files
2.  **Solder components**: Follow the detailed BOM
3.  **3D print enclosure**: Use provided STL files
4.  **Assemble**: Insert PCB into printed enclosure

### 2\. Software Setup

1.  **Install Arduino IDE** (version 1.8.19 or newer)
2.  **Add STM32 board support**
3.  **Install required libraries**:
    -   DFRobot_SGP40
    -   PlayRtttl
    -   ptScheduler
4.  **Upload firmware** using ST-Link programmer


 Required Libraries
---------------------

Add these libraries through Arduino IDE Library Manager:

```
#include <DFRobot_SGP40.h>     // SGP40 sensor communication
#include <PlayRtttl.hpp>       // Musical tone generation
#include <ptScheduler.h>       // Non-blocking task scheduling

```

 Programming the Device
------------------------

### Hardware Requirements

-   **ST-Link V2 programmer** (required for STM32)
-   **USB-C cable** for power and charging
-   **3.3V power supply** during programming

### Upload Process

1.  Connect ST-Link to SWD pins (SWDIO, SWCLK)
2.  Select board: "STM32U0 series" in Arduino IDE
3.  Select programmer: "STM32CubeProgrammer (SWD)"
4.  Upload firmware

 PCB Specifications
---------------------

-   **Dimensions**: 32.5mm diameter
-   **Layers**: 2-layer PCB
-   **Thickness**: 1.6mm
-   **Components**: SMD (0603/0402 packages)
-   **Connector**: USB-C for charging

 3D Printing Guide
---------------------

### Print Settings

-   **Layer Height**: 0.2mm
-   **Infill**: 20%
-   **Supports**: Minimal (only for overhangs)
-   **Material**: PLA or PETG recommended

### Post-Processing

-   Light sanding for smooth finish
-   Ensure USB-C port alignment
-   Test fit before final assembly

Acknowledgments
------------------

-   **CircuitDigest** - Project development and documentation
-   **DFRobot** - SGP40 sensor library
-   **STMicroelectronics** - STM32 microcontroller support
-   **DigiKey** - Sponsors

Support
----------

-   **Detailed Tutorial**: [CircuitDigest Article](https://circuitdigest.com/videos/wearable-air-quality-pendant)
-   **Video Guide**: [YouTube Tutorial](https://youtube.com/watch?v=c5lP1kzYoBA)
-   **Issues**: Report bugs via GitHub Issues
-   **Discussion**: Join our community forum

Version History
-------------------

| Version | Date | Changes |
| --- | --- | --- |
| v1.0 | 2024-12 | Initial release |
| v1.1 | 2025-01 | Bug fixes, improved battery life |

* * * * *

**⭐ If this project helped you, please star the repository!**

**🔗 Related Projects:**

-   [Arduino Air Quality Monitor](https://github.com/Circuit-Digest/Arduino-Air-Quality-Monitor)
-   [IoT Air Quality System](https://github.com/Circuit-Digest/IoT-Air-Quality-System)

**📧 Contact:** support@circuitdigest.com
