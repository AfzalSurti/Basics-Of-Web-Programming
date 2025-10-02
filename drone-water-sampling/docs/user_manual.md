# Water Sampling System - User Manual

## Table of Contents

1. [Introduction](#introduction)
2. [Safety Information](#safety-information)
3. [System Overview](#system-overview)
4. [Pre-Flight Preparation](#pre-flight-preparation)
5. [Operating Procedures](#operating-procedures)
6. [Troubleshooting](#troubleshooting)
7. [Maintenance](#maintenance)
8. [Specifications](#specifications)

## Introduction

Welcome to the Drone-Based Water Sampling System User Manual. This comprehensive guide will help you safely and effectively operate the water sampling system for environmental monitoring applications.

### Intended Use

This system is designed for:
- Water quality monitoring in lakes, rivers, and reservoirs
- Environmental research and data collection
- Industrial water monitoring
- Agricultural water testing
- Emergency response water assessment

### System Capabilities

- Autonomous GPS-guided flight to sampling locations
- 500mL water sample collection at programmable depths
- Real-time pH, turbidity, and temperature measurements
- Long-range telemetry (up to 10km)
- Automatic data logging to microSD card
- Return-to-home and landing automation

## Safety Information

⚠️ **WARNING: Read all safety information before operating the system**

### Regulatory Compliance

- **FAA Part 107 Certification** required for commercial operations (USA)
- Maintain **visual line of sight** at all times
- Do not fly over people or moving vehicles
- Maximum altitude: 400 feet AGL (or local regulations)
- Check local airspace restrictions before flight

### Operational Safety

1. **Battery Safety**
   - Never fly with damaged or swollen batteries
   - Check battery voltage before each flight
   - Return to launch when battery reaches 30%
   - Store batteries at storage voltage (3.8V per cell)

2. **Weather Limitations**
   - Do not fly in wind speeds above 25 mph (10 m/s)
   - Avoid rain, snow, or heavy fog
   - Do not fly in temperatures below 32°F (0°C) or above 104°F (40°C)
   - Check weather forecast before mission

3. **Water Safety**
   - Maintain minimum 1 meter clearance from water surface during approach
   - Be aware of waves and water surface conditions
   - Do not attempt water landing
   - Keep sampling altitude consistent

4. **Emergency Procedures**
   - Know location of emergency stop button
   - Practice manual override procedures
   - Have backup communication method ready
   - Maintain clear area for emergency landing

### Personal Safety

- Wear safety glasses during operation
- Keep hands and clothing away from propellers
- Never reach towards armed UAV
- Use hearing protection near operating motors
- Work in teams for enhanced safety

## System Overview

### Main Components

1. **UAV Platform**
   - Quadcopter or hexacopter airframe
   - Pixhawk flight controller
   - GPS/compass module
   - 2.4 GHz radio controller
   - LiPo battery pack

2. **Water Sampling Device**
   - 1-liter sample bottle
   - Peristaltic pump
   - Telescopic suction pipe
   - Quick-release bottle mechanism
   - Mounting brackets

3. **Electronics Module**
   - ESP32 microcontroller
   - pH sensor with BNC connector
   - Turbidity sensor
   - DS18B20 temperature sensor
   - LoRa radio module
   - GPS module
   - MicroSD card for logging

4. **Ground Station**
   - LoRa receiver
   - Laptop with control software
   - Radio controller
   - Spare batteries and equipment

### System Architecture

```
┌─────────────────────────────────────────────┐
│              UAV Platform                    │
│  ┌──────────────────────────────────────┐  │
│  │      Flight Controller (Pixhawk)     │  │
│  └──────────────────────────────────────┘  │
│                    │                         │
│  ┌─────────────────┴─────────────────────┐ │
│  │         Water Sampling Module         │ │
│  │  ┌────────────┐    ┌──────────────┐  │ │
│  │  │   Bottle   │    │ Pump Control │  │ │
│  │  └────────────┘    └──────────────┘  │ │
│  │  ┌──────────────────────────────────┐ │ │
│  │  │    ESP32 + Sensors + LoRa        │ │ │
│  │  └──────────────────────────────────┘ │ │
│  └───────────────────────────────────────┘ │
└─────────────────────────────────────────────┘
                    ↕ LoRa (10km)
┌─────────────────────────────────────────────┐
│           Ground Control Station            │
│  ┌───────────────────────────────────────┐ │
│  │      Dashboard (Laptop/Tablet)        │ │
│  │  - Real-time telemetry                │ │
│  │  - Mission planning                   │ │
│  │  - Data visualization                 │ │
│  └───────────────────────────────────────┘ │
└─────────────────────────────────────────────┘
```

## Pre-Flight Preparation

### 1. Equipment Checklist

Before each flight mission, verify:

**UAV System:**
- [ ] Propellers installed and secure (no cracks)
- [ ] Motors spin freely without resistance
- [ ] Flight controller powered and responding
- [ ] GPS has clear view of sky
- [ ] Radio controller fully charged
- [ ] FPV/telemetry links working
- [ ] All wiring connections secure

**Water Sampling System:**
- [ ] Sample bottle clean and empty
- [ ] Bottle properly seated in quick-release
- [ ] Suction pipe extended and locked
- [ ] Pipe filter installed and clean
- [ ] Pump operational (ground test)
- [ ] All connectors secure

**Electronics:**
- [ ] ESP32 powered on and booting
- [ ] Status LEDs indicating normal operation
- [ ] MicroSD card inserted and formatted
- [ ] LoRa antenna connected
- [ ] GPS antenna connected
- [ ] Sensors reading reasonable values

**Batteries:**
- [ ] UAV battery: >90% charge, balanced
- [ ] Sampling system battery: >90% charge
- [ ] No swelling or damage
- [ ] Proper voltage (check with multimeter)
- [ ] Connectors clean and tight

**Ground Station:**
- [ ] Laptop fully charged or on AC power
- [ ] LoRa receiver connected
- [ ] Dashboard software running
- [ ] Telemetry link established
- [ ] Mission parameters loaded
- [ ] Backup communication ready

### 2. Sensor Calibration

Perform sensor calibration every 10 flights or monthly:

**pH Sensor Calibration:**
1. Prepare pH 4.0, 7.0, and 10.0 buffer solutions
2. Rinse sensor with distilled water
3. Immerse in pH 7.0 buffer, wait for stable reading
4. Record voltage
5. Repeat for pH 4.0 and 10.0
6. Calculate calibration coefficients
7. Update firmware constants

**Turbidity Sensor Calibration:**
1. Prepare clear water sample (0 NTU reference)
2. Prepare known turbidity standards (100, 500, 1000 NTU)
3. Measure voltage for each standard
4. Plot calibration curve
5. Update firmware parameters

**Temperature Sensor Calibration:**
- DS18B20 is factory calibrated
- Verify against reference thermometer if needed

### 3. Site Survey

Before flying at a new location:

1. **Airspace Check**
   - Check NOTAMs and TFRs
   - Verify no controlled airspace
   - Note any restrictions

2. **Physical Survey**
   - Walk the area
   - Identify obstacles (trees, power lines)
   - Note emergency landing zones
   - Mark home point
   - Check wind conditions

3. **Water Body Assessment**
   - Water surface conditions (calm/choppy)
   - Depth at sampling point
   - Obstructions below surface
   - Current/flow rate
   - Access for retrieval if needed

### 4. Mission Planning

Using ground control software:

1. Set home location (takeoff point)
2. Mark sampling waypoint(s)
3. Set altitude parameters
4. Configure sampling depth and duration
5. Review flight path
6. Calculate battery requirements
7. Set geofence boundaries
8. Configure failsafe actions

## Operating Procedures

### Standard Sampling Mission

#### Phase 1: Pre-Flight

1. **Power Up Sequence**
   ```
   a. Connect UAV battery (wait for initialization)
   b. Connect sampling system battery
   c. Wait for GPS fix (LED indicator)
   d. Verify all systems green on dashboard
   e. Arm flight controller (safety check)
   ```

2. **System Check**
   - GPS: 8+ satellites, 3D fix
   - Compass: Calibrated, no errors
   - Battery: >90%, cells balanced
   - Sensors: Reading reasonable values
   - Pump: Test operation (3 seconds)
   - Radio: Full signal strength

3. **Final Checks**
   - Clear area for takeoff
   - Wind speed acceptable
   - No incoming aircraft
   - Ground station ready
   - Emergency procedures reviewed

#### Phase 2: Flight to Location

1. **Takeoff**
   ```
   a. Set mode to GUIDED
   b. Arm motors (via radio or GCS)
   c. Initiate takeoff command
   d. Climb to 10m, verify stable
   e. Continue to cruise altitude (20m)
   ```

2. **Navigation**
   - Monitor altitude and position
   - Check battery voltage
   - Verify GPS lock maintained
   - Watch for obstacles
   - Monitor telemetry

3. **Approach**
   - Reduce speed 50m from target
   - Verify target coordinates
   - Check water surface conditions
   - Begin descent when over target

#### Phase 3: Sampling

1. **Descent to Sampling Altitude**
   ```
   a. Descend to 5m AGL
   b. Reduce to 0.5m above water
   c. Stabilize hover (10 seconds)
   d. Monitor position hold
   ```

2. **Deploy Sampling System**
   ```
   a. Verify stable hover
   b. Extend suction pipe to depth
   c. Wait 5 seconds for stabilization
   d. Start peristaltic pump
   ```

3. **Sample Collection**
   ```
   Duration: 5 minutes for 500mL
   
   Monitor:
   - Altitude hold (±0.2m)
   - Position hold (±1m)
   - Battery level
   - Pump operation
   - Sensor readings
   
   Dashboard displays:
   - Progress: 0-100%
   - Volume collected: 0-500mL
   - Time elapsed: 00:00-05:00
   ```

4. **Sensor Measurements**
   ```
   During sampling:
   - pH: Continuous monitoring
   - Turbidity: Every 5 seconds
   - Temperature: Continuous
   
   Data transmitted via LoRa:
   - Real-time values
   - Running averages
   - Min/max values
   ```

5. **Complete Sampling**
   ```
   a. Stop pump
   b. Wait 5 seconds
   c. Retract suction pipe
   d. Final sensor readings
   e. Transmit complete dataset
   ```

#### Phase 4: Return and Landing

1. **Climb and Return**
   ```
   a. Climb to cruise altitude (20m)
   b. Navigate to home point
   c. Monitor battery (should be >30%)
   d. Maintain telemetry link
   ```

2. **Landing Approach**
   ```
   a. Arrive at home point
   b. Descend gradually
   c. Reduce to 1m altitude
   d. Final descent
   e. Motors disarm automatically
   ```

3. **Post-Flight**
   ```
   a. Disconnect batteries
   b. Download data from microSD
   c. Clean sensors
   d. Empty sample bottle
   e. Inspect equipment
   f. Log flight in logbook
   ```

### Data Management

1. **Onboard Logging**
   - All data logged to microSD card
   - CSV format: timestamp, sensors, GPS, status
   - File: `water_data_YYYYMMDD_HHMMSS.csv`

2. **Telemetry Reception**
   - Real-time data via LoRa
   - Ground station logs all packets
   - File: `telemetry_log_YYYYMMDD_HHMMSS.csv`

3. **Data Analysis**
   - Import CSV into Excel/Python
   - Plot sensor trends
   - Compare with historical data
   - Generate reports

## Troubleshooting

### Common Issues

#### GPS Not Locking
**Symptoms:** GPS LED blinking, <6 satellites
**Solutions:**
- Wait 2-5 minutes for initial fix
- Move to area with clear sky view
- Check GPS antenna connection
- Verify GPS power supply
- Update GPS firmware if needed

#### LoRa Link Lost
**Symptoms:** No telemetry updates, "Link Lost" warning
**Solutions:**
- Check ground station antenna
- Verify frequency settings match
- Reduce distance if at edge of range
- Check for interference sources
- Verify LoRa module powered

#### Pump Not Working
**Symptoms:** No water collected, pump silent
**Solutions:**
- Check pump power connection
- Verify motor driver working
- Test pump on ground
- Check for blockages in tubing
- Verify pump direction (forward not reverse)

#### Sensor Readings Invalid
**Symptoms:** pH=0, temperature=-127°C, turbidity=3000
**Solutions:**
- Check sensor connections
- Verify sensor power
- Recalibrate sensors
- Replace faulty sensor
- Check ADC functionality

#### Low Battery Warning
**Symptoms:** Battery <30%, "LOW BATTERY" alert
**Actions:**
- Abort sampling if in progress
- Return to launch immediately
- Land as soon as possible
- Do not attempt additional flights

## Maintenance

### Daily (After Each Flight)

- Clean sensors with distilled water
- Wipe sample bottle interior
- Check tubing for wear
- Inspect propellers
- Check all connectors
- Download and back up data
- Charge batteries

### Weekly

- Calibrate pH sensor
- Clean turbidity sensor optical surfaces
- Lubricate pump mechanism
- Check motor bearings
- Tighten all screws
- Update flight log
- Review telemetry data

### Monthly

- Full system calibration
- Replace tubing if worn
- Check solder joints
- Update firmware if available
- Backup all data
- Review maintenance log
- Replace worn components

### Annual

- Professional inspection
- Replace sample bottle
- Replace all O-rings and gaskets
- Calibrate all sensors professionally
- Update all software
- Review insurance and permits
- Attend refresher training

## Specifications

### Complete System Specifications

**Physical:**
- Total weight: 2.0 kg (sampling system only)
- Dimensions: 250mm × 150mm × 200mm
- Sample capacity: 1 liter
- Pipe reach: 0.3-1.0 meters

**Performance:**
- Flow rate: 100 mL/min
- Sampling time: 5 minutes (500mL)
- Operating temperature: 0-40°C
- Operating altitude: 0-1000m

**Sensors:**
- pH: 0-14 range, ±0.1 accuracy
- Turbidity: 0-3000 NTU, ±2 NTU
- Temperature: -55 to +125°C, ±0.5°C

**Communication:**
- LoRa: 915/868 MHz, 10km range
- GPS: 10Hz update, ±2m accuracy
- Data rate: 1 packet/second

**Power:**
- Sampling system: 7.4V 2200mAh (2S LiPo)
- Runtime: 30-45 minutes
- UAV battery: 4S or 6S (separate)

**Data Storage:**
- MicroSD: 16GB (expandable)
- Format: CSV
- Capacity: ~1000 hours of data

---

For additional information, refer to:
- Assembly Guide: `docs/assembly_guide.md`
- Flight Operations: `docs/flight_operations.md`
- Safety Guidelines: `docs/safety.md`
- Calibration Procedures: `docs/calibration.md`

**Technical Support:** See project repository
**Version:** 1.0.0
**Date:** 2024-01-15
