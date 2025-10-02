# Assembly Guide - Water Sampling System

## Overview

This guide provides step-by-step instructions for assembling the complete drone water sampling system from components.

## Required Tools

- Screwdriver set (Phillips, flathead)
- Allen key set (metric)
- Wire strippers and cutters
- Soldering iron and solder
- Multimeter
- Torque wrench (small, 0-5 Nm)
- Cable ties
- Heat shrink tubing
- Isopropyl alcohol
- Lint-free cloths

## Assembly Time

- **Mechanical Assembly:** 2-3 hours
- **Electronics Assembly:** 3-4 hours (if PCB pre-assembled)
- **Testing and Calibration:** 2 hours
- **Total:** 7-9 hours for first build

## Part 1: Mechanical Assembly

### Step 1: Main Housing Assembly (30 minutes)

1. **Prepare housing halves**
   - Clean all surfaces with isopropyl alcohol
   - Inspect for cracks or defects
   - Apply gasket seal to mating surfaces

2. **Install mounting brackets**
   - Attach 4x aluminum brackets to top housing
   - Use M4x16 screws with lock washers
   - Torque to 3.0 Nm
   - Verify 30mm mounting spacing

3. **Install cable glands**
   - Insert PG7 and PG9 glands
   - Hand tighten, then 1/4 turn with wrench
   - Verify IP68 sealing

### Step 2: Pump Installation (30 minutes)

1. **Mount pump to housing**
   - Position pump in designated location
   - Secure with anti-vibration mounts
   - Connect motor power leads

2. **Install tubing**
   - Cut silicone tubing to length (as marked)
   - Connect pump inlet to bottle cap
   - Connect pump outlet to suction pipe
   - Secure all connections with hose clamps

3. **Test pump**
   - Apply 12V power
   - Verify rotation direction (forward)
   - Check for leaks
   - Test flow rate (should be ~100 mL/min)

### Step 3: Bottle and Pipe Assembly (20 minutes)

1. **Prepare sample bottle**
   - Install modified cap with inlet/outlet ports
   - Apply silicone gasket
   - Test seal (should hold vacuum)

2. **Install quick-release mechanism**
   - Attach base plate to housing
   - Test bottle insertion and removal
   - Verify 180° twist-lock operation

3. **Assemble suction pipe**
   - Extend telescopic sections
   - Lock at desired depth (0.3-1.0m)
   - Install filter screen at intake end
   - Attach protective cap for transport

## Part 2: Electronics Assembly

**Note:** If using pre-assembled PCB, skip to "PCB Installation"

### Step 4: PCB Assembly

See detailed instructions in `pcb/assembly_guide.md` for complete PCB assembly procedures including:
- Component soldering
- Module installation
- Testing and validation

### Step 5: PCB Installation (30 minutes)

1. **Prepare PCB**
   - Verify all components soldered
   - Test 5V and 3.3V rails
   - Check for shorts

2. **Mount PCB in housing**
   - Use standoffs for vibration isolation
   - Secure with M3 screws
   - Ensure no metal contact with housing

3. **Connect external sensors**
   - pH sensor via BNC connector (J1)
   - Turbidity sensor via 4-pin JST (J2)
   - Temperature sensor via 3-pin JST (J4)
   - Label all connections

4. **Connect pump motor**
   - Motor leads to J3 (2-pin connector)
   - Verify polarity (red=+, black=-)
   - Test direction before sealing

### Step 6: Antenna Installation (15 minutes)

1. **GPS antenna**
   - Mount on top of housing (clear sky view)
   - Connect U.FL cable to GPS module
   - Secure with adhesive

2. **LoRa antenna**
   - Mount vertically on side
   - Connect to RFM95W module
   - Test with antenna analyzer if available

### Step 7: Power Connections (20 minutes)

1. **Battery connector**
   - Connect 2S LiPo battery lead to BAT1
   - Verify polarity (DO NOT REVERSE!)
   - Test voltage with multimeter
   - Install resettable fuse (F1)

2. **Power distribution**
   - Verify 7.4V input
   - Check 5V regulator output
   - Check 3.3V regulator output
   - Measure current draw at idle

### Step 8: Final Assembly (30 minutes)

1. **Close housing**
   - Route all cables through glands
   - Tighten cable glands
   - Join housing halves
   - Install perimeter screws
   - Torque evenly in star pattern

2. **Install status indicators**
   - LEDs visible through window
   - Test each LED
   - Install buzzer

3. **Attach rubber feet**
   - 4x vibration dampening feet
   - Secure to bottom corners

## Part 3: System Integration

### Step 9: UAV Mounting (20 minutes)

1. **Prepare UAV**
   - Clean mounting surface
   - Verify load capacity (>2.5 kg)
   - Check center of gravity

2. **Attach sampling system**
   - Align with mounting holes
   - Use 4x M4 screws and lock washers
   - Ensure level mounting
   - Check for secure attachment

3. **Connect to flight controller** (if integrated)
   - Optional: UART connection for monitoring
   - Optional: PWM signal for pump control

### Step 10: Testing (1 hour)

1. **Power-on test**
   - Connect battery
   - Observe LED sequence
   - Check serial output
   - Verify GPS acquiring satellites

2. **Sensor test**
   - pH: Should read ~7 in air or water
   - Turbidity: Should respond to light changes
   - Temperature: Room temperature ±2°C
   - GPS: Acquiring satellites

3. **Pump test**
   - Fill bottle with water
   - Run pump for 30 seconds
   - Verify no leaks
   - Check suction
   - Measure flow rate

4. **Telemetry test**
   - Power on ground station
   - Verify LoRa link
   - Check data reception
   - Test RSSI levels

5. **Functional test**
   - Simulate complete sampling cycle
   - Verify data logging to SD card
   - Test all modes
   - Check emergency stop

### Step 11: Calibration (1 hour)

Follow procedures in `docs/calibration.md` for:
- pH sensor calibration
- Turbidity sensor calibration
- Temperature verification
- System checks

## Part 4: Final Checks

### Pre-Flight Inspection Checklist

- [ ] All screws tight
- [ ] No loose wires
- [ ] Housing sealed
- [ ] Gaskets intact
- [ ] Bottle secured
- [ ] Pipe extends properly
- [ ] Sensors clean
- [ ] Battery charged
- [ ] Antennas secure
- [ ] LEDs functional
- [ ] Pump operational
- [ ] Data logging working
- [ ] Telemetry active
- [ ] Weight verified (should be ~2.0 kg)

## Troubleshooting Assembly Issues

### Pump Not Working
- Check motor connections
- Verify 12V supply
- Test motor driver (L298N)
- Check for blockages

### No Sensor Readings
- Verify sensor power (5V or 3.3V)
- Check connector wiring
- Test with multimeter
- Verify ADC functionality

### GPS Not Locking
- Check antenna connection
- Verify clear sky view
- Wait 5 minutes (cold start)
- Check GPS module power

### LoRa Not Transmitting
- Verify antenna connected
- Check SPI connections
- Test with loopback
- Verify frequency setting

### Leaks in System
- Check all O-rings
- Tighten hose clamps
- Verify gasket sealing
- Test under pressure

## Maintenance Notes

### After Each Flight
- Clean sensors
- Check for leaks
- Inspect connections
- Download data

### Weekly
- Tighten all screws
- Check battery health
- Clean housing exterior
- Lubricate pump

### Monthly
- Full system inspection
- Recalibrate sensors
- Replace wear items
- Update firmware

## Safety Warnings

⚠️ **Electrical Safety**
- Always disconnect battery before working on electronics
- Check polarity before connecting power
- Use proper ESD protection

⚠️ **Mechanical Safety**
- Torque screws to specification
- Do not overtighten plastic components
- Check all connections are secure

⚠️ **Chemical Safety**
- Handle buffer solutions with care
- Wear gloves when handling samples
- Dispose of waste properly

## Assembly Quality Check

After assembly, system should meet these criteria:

- **Weight:** 2.0 ± 0.2 kg (with empty bottle)
- **Dimensions:** 250 × 150 × 200 mm
- **Waterproof:** IP67 minimum (except connectors)
- **Power consumption:** <1A idle, <2A during sampling
- **Operating time:** 30-45 minutes continuous
- **Data rate:** 1 sample per 5 seconds minimum

## Documentation

After assembly, document:
- Serial numbers of all components
- Calibration dates and values
- Assembly date and technician
- Initial test results
- Any modifications or issues

Store documentation with unit.

## Support

For assembly questions:
1. Review this guide thoroughly
2. Check PCB assembly guide for electronics
3. Refer to CAD drawings for mechanical details
4. See troubleshooting section
5. Contact technical support (see repository)

---

**Document Version:** 1.0.0  
**Date:** 2024-01-15  
**Estimated Assembly Time:** 7-9 hours (first build)
