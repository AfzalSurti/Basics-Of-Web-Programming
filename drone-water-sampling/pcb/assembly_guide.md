# PCB Assembly Guide - ESP32 Water Quality System

## Overview

This guide provides step-by-step instructions for assembling the ESP32-based water quality monitoring PCB. The board integrates sensors, communication modules, and power management for the drone water sampling system.

## Tools Required

- Soldering iron (temperature controlled, 350°C)
- Solder (lead-free recommended, 0.8mm diameter)
- Flux pen
- Solder wick or desoldering pump
- Tweezers
- Wire cutters
- Wire strippers
- Multimeter
- Magnifying glass or microscope (for SMD work)
- ESD wrist strap
- Helping hands/PCB holder

## Components Overview

### Main Components
- ESP32-WROOM-32 module
- L298N motor driver
- Voltage regulators (3.3V and 5V)
- RFM95W LoRa module
- NEO-6M GPS module
- MCP3008 ADC
- Various passive components

### External Sensors
- pH sensor with BNC connector
- Turbidity sensor (analog/I2C)
- DS18B20 temperature sensor

## Assembly Steps

### Step 1: PCB Inspection
1. Inspect the bare PCB for any defects
2. Check for proper copper plating and no shorts
3. Clean the PCB with isopropyl alcohol if needed
4. Identify component placement using silkscreen markings

### Step 2: SMD Components (Optional)
If using SMD components, solder these first:

1. **Apply flux** to the pads
2. **Solder voltage regulators** (U3, U4):
   - Apply solder to one pad
   - Place component and heat the pre-soldered pad
   - Solder remaining pads
   - Check for proper alignment

3. **Solder 0805 resistors and capacitors**:
   - Use tweezers to place components
   - Heat one pad while applying solder
   - Solder the opposite pad
   - Verify values with multimeter

### Step 3: Through-Hole Components

#### 3.1 Resistors and Diodes
1. **Install resistors R1-R13**:
   - Bend leads to fit PCB spacing
   - Insert from component side
   - Solder from solder side
   - Clip excess leads
   - Double-check resistor values!

2. **Install diode D1**:
   - **Note polarity** (cathode band towards marked end)
   - Insert and solder
   - Clip leads

#### 3.2 Capacitors
1. **Ceramic capacitors C1-C4**:
   - Non-polarized, can be installed either way
   - Insert, solder, clip

2. **Electrolytic capacitors C5-C10**:
   - **Observe polarity** (negative marking on body)
   - Long lead = positive
   - Insert carefully
   - Solder and clip

#### 3.3 IC Sockets (Recommended)
1. **Install DIP-16 socket for U7 (MCP3008)**:
   - Note notch orientation
   - Press firmly into place
   - Solder opposite corners first
   - Solder remaining pins

#### 3.4 Transistors
1. **Install Q1-Q4 (2N2222)**:
   - Match flat side to PCB marking
   - Insert gently (don't force)
   - Solder and clip

### Step 4: Connectors

#### 4.1 Power Connectors
1. **Install J6 (screw terminal)**:
   - Insert from component side
   - Ensure it sits flush
   - Solder all pins

2. **Install BAT1 (battery connector)**:
   - Observe polarity markings
   - Solder securely

#### 4.2 Sensor Connectors
1. **Install J1 (BNC connector)**:
   - Insert and press firmly
   - Solder all pins and shield tabs
   - Check mechanical stability

2. **Install J2-J4 (JST-XH connectors)**:
   - Insert connectors
   - Solder pins
   - Test with mating connectors

#### 4.3 MicroSD Socket
1. **Install J5 (MicroSD socket)**:
   - Align carefully with pads
   - Solder data pins first
   - Solder mechanical anchors
   - Test card insertion

### Step 5: Switches and Indicators

#### 5.1 LEDs
1. **Install D2-D6 (status LEDs)**:
   - **Critical: Observe polarity**
   - Flat side = cathode (shorter lead)
   - Insert through holes
   - Solder and clip
   - Test with multimeter diode mode

#### 5.2 Switches
1. **Install SW1 (tactile button)**:
   - Press firmly into holes
   - Solder all four pins

2. **Install SW2 (slide switch)**:
   - Insert and solder
   - Test sliding action

#### 5.3 Buzzer
1. **Install BZ1 (piezo buzzer)**:
   - Observe polarity (+/- marking)
   - Insert and solder

### Step 6: Modules

#### 6.1 ESP32 Module
1. **Prepare ESP32-WROOM-32**:
   - Check for bent pins
   - Apply flux to pads if needed

2. **Solder ESP32**:
   - Align module with footprint
   - Solder opposite corner pins first
   - Check alignment
   - Solder remaining pins
   - Use generous solder for good connection

#### 6.2 Motor Driver
1. **Install L298N module or IC**:
   - If using module: solder header pins first
   - If using IC: use socket recommended
   - Ensure proper orientation

#### 6.3 LoRa Module
1. **Prepare RFM95W**:
   - Solder header pins to module
   - Align with PCB footprint

2. **Install RFM95W**:
   - Insert into PCB
   - Solder all pins
   - Attach antenna connector if needed

#### 6.4 GPS Module
1. **Install NEO-6M**:
   - Solder header pins to module
   - Insert into PCB
   - Solder connections
   - Attach GPS antenna

### Step 7: External Connections

#### 7.1 DS18B20 Temperature Sensor
1. **Prepare sensor wires**:
   - Red: VCC (3.3V)
   - Black: GND
   - Yellow/White: Data
   - Strip 5mm from each wire

2. **Connect to J4**:
   - Crimp or solder JST connector
   - Test continuity

#### 7.2 pH Sensor
1. **Connect BNC cable** to J1
2. **Route cable** through cable gland in housing
3. **Connect to pH probe**

#### 7.3 Turbidity Sensor
1. **Connect to J2** (4-pin connector)
2. **Wire mapping**:
   - Pin 1: VCC (5V)
   - Pin 2: GND
   - Pin 3: Signal (Analog out)
   - Pin 4: NC (Not connected)

### Step 8: Testing and Validation

#### 8.1 Visual Inspection
1. Check all solder joints for:
   - Proper wetting
   - No cold joints
   - No solder bridges
   - No loose connections

2. Inspect with magnifying glass

#### 8.2 Electrical Tests
1. **Continuity tests**:
   - Check GND plane continuity
   - Verify no shorts between VCC and GND
   - Test all connector pins

2. **Power rail tests**:
   - Connect 7.4V to battery input
   - **Before powering on**: Set multimeter to voltage mode
   - Measure 5V rail (should be 4.8-5.2V)
   - Measure 3.3V rail (should be 3.2-3.4V)
   - If voltages incorrect, disconnect power immediately

3. **LED test**:
   - Power on board
   - Observe power LED
   - Test other LEDs with firmware

#### 8.3 Module Communication Tests
1. **ESP32 test**:
   - Connect USB-Serial adapter
   - Upload blink sketch
   - Verify operation

2. **Sensor tests**:
   - Upload sensor test firmware
   - Read values from each sensor
   - Verify reasonable readings

3. **LoRa test**:
   - Upload LoRa test code
   - Test transmission with second module
   - Verify range

4. **GPS test**:
   - Place near window or outdoors
   - Wait for GPS lock (2-5 minutes)
   - Verify NMEA data

### Step 9: Final Assembly

1. **Clean the PCB**:
   - Remove flux residue with isopropyl alcohol
   - Dry thoroughly
   - Inspect for any remaining flux

2. **Apply conformal coating** (optional):
   - Mask connectors
   - Apply thin coat
   - Allow to cure per manufacturer instructions

3. **Install in enclosure**:
   - Use anti-vibration mounts
   - Secure with M3 screws
   - Route cables through glands

4. **Label connections**:
   - Mark all connectors
   - Create wiring diagram
   - Store in enclosure

## Troubleshooting

### No Power
- Check fuse F1
- Verify battery voltage
- Check polarity
- Test voltage regulators

### ESP32 Not Responding
- Check solder joints on ESP32
- Verify 3.3V rail
- Try external programmer
- Check EN (enable) pin

### Sensor Not Reading
- Verify sensor power
- Check connector wiring
- Test with multimeter
- Try different ADC channel

### LoRa Not Transmitting
- Check antenna connection
- Verify SPI connections
- Test with known-good module
- Check frequency setting

### GPS No Fix
- Verify antenna connection
- Check for indoor use (may not work)
- Wait longer (up to 10 minutes first time)
- Verify UART connections

## Safety Warnings

⚠️ **Important Safety Information**:

1. Always use ESD protection when handling components
2. Never apply reverse polarity to battery
3. Check all voltages before connecting sensors
4. Do not exceed voltage ratings of components
5. Ensure proper heatsinking for voltage regulators
6. Keep away from water during assembly and testing
7. Use proper ventilation when soldering
8. Disconnect power before making any changes

## Calibration

After successful assembly and testing, proceed to sensor calibration:
- See `docs/calibration.md` for detailed procedures
- pH sensor requires 3-point calibration
- Turbidity sensor needs baseline calibration
- Temperature sensor typically factory calibrated

## Support

For assembly issues:
1. Review troubleshooting section
2. Check component orientation
3. Verify solder joints
4. Test with minimal components first
5. Refer to schematic.pdf for circuit details

## Revision History

- v1.0 (2024-01-15): Initial assembly guide
