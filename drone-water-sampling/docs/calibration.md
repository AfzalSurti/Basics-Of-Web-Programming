# Sensor Calibration Procedures

## Overview

Proper sensor calibration is critical for accurate water quality measurements. This guide provides step-by-step procedures for calibrating all sensors in the water sampling system.

## Calibration Schedule

| Sensor | Initial | Routine | After Event |
|--------|---------|---------|-------------|
| pH | Yes | Every 10 flights or monthly | After sensor replacement, extreme temperature exposure |
| Turbidity | Yes | Every 20 flights or quarterly | After lens cleaning, sensor replacement |
| Temperature | No (factory calibrated) | Optional annual verification | After sensor replacement |
| GPS | No (auto-calibrating) | N/A | N/A |

## Required Equipment

### General Tools
- Distilled water (ASTM Type II minimum)
- Clean beakers (100mL, 250mL)
- Graduated cylinders
- Stirring rod (glass or plastic)
- Thermometer (±0.1°C accuracy)
- Timer
- Lab notebook for recording results

### pH Calibration
- pH buffer solutions:
  - pH 4.0 (red)
  - pH 7.0 (yellow)
  - pH 10.0 (blue)
- pH electrode storage solution
- Lint-free wipes

### Turbidity Calibration
- Formazin turbidity standards:
  - 0 NTU (distilled water)
  - 100 NTU
  - 500 NTU
  - 1000 NTU
- Optical cell cleaning solution
- Lens tissue

### Temperature Calibration (Optional)
- Reference thermometer (NIST traceable)
- Ice bath setup
- Hot water bath

## pH Sensor Calibration

### Preparation (15 minutes)

1. **Equipment Setup**
   ```
   - Clean work area
   - Room temperature: 20-25°C
   - Arrange buffers at room temp (2 hours prior)
   - Prepare three clean beakers
   - Have distilled water ready for rinsing
   ```

2. **Sensor Preparation**
   ```
   - Remove pH probe from storage solution
   - Rinse with distilled water
   - Gently blot dry with lint-free wipe
   - Do NOT rub the glass bulb
   ```

### Three-Point Calibration (30 minutes)

#### Step 1: pH 7.0 (Neutral Point)

1. Pour 50mL of pH 7.0 buffer into clean beaker
2. Immerse pH probe completely covering the glass bulb
3. Gently stir for 10 seconds, then let stabilize
4. Wait for reading to stabilize (30-60 seconds)
5. Record voltage reading: ______ mV
6. Expected: ~0 mV (near zero potential at pH 7)

#### Step 2: pH 4.0 (Acid Point)

1. Remove probe and rinse thoroughly with distilled water
2. Blot dry gently
3. Pour 50mL of pH 4.0 buffer into clean beaker
4. Immerse probe as before
5. Wait for stable reading (30-60 seconds)
6. Record voltage reading: ______ mV
7. Expected: ~180 mV (positive voltage)

#### Step 3: pH 10.0 (Base Point)

1. Rinse probe thoroughly
2. Blot dry
3. Pour 50mL of pH 10.0 buffer into clean beaker
4. Immerse probe
5. Wait for stable reading
6. Record voltage reading: ______ mV
7. Expected: ~-180 mV (negative voltage)

### Calculate Calibration Coefficients

Using your recorded voltages:

```
pH 4.0 voltage  (V1): ______ mV
pH 7.0 voltage  (V2): ______ mV
pH 10.0 voltage (V3): ______ mV

Slope = (pH10 - pH4) / (V3 - V1)
      = (10 - 4) / (V3 - V1)
      = 6 / (V3 - V1)

Offset = pH7 - (V2 * Slope)
       = 7 - (V2 * Slope)

Example Calculation:
If V1 = +180 mV, V2 = 0 mV, V3 = -180 mV
Slope = 6 / (-180 - 180) = 6 / -360 = -0.0167
Offset = 7 - (0 * -0.0167) = 7.0

Theoretical (Nernst): ~59.16 mV per pH unit at 25°C
```

### Update Firmware

Edit `firmware/esp32_main/sensors.h`:

```cpp
#define PH_CALIBRATION_SLOPE  -0.0167  // Your calculated value
#define PH_CALIBRATION_OFFSET  7.0     // Your calculated value
```

Upload updated firmware to ESP32.

### Verification

1. Test in pH 7.0 buffer: Should read 7.0 ± 0.1
2. Test in pH 4.0 buffer: Should read 4.0 ± 0.1
3. Test in pH 10.0 buffer: Should read 10.0 ± 0.1

If readings are outside tolerance, repeat calibration.

### Post-Calibration Storage

1. Rinse probe with distilled water
2. Store in electrode storage solution
3. Never store dry or in distilled water
4. Replace storage solution monthly

## Turbidity Sensor Calibration

### Preparation (10 minutes)

1. **Clean Optical Surfaces**
   ```
   - Remove sensor from housing
   - Gently clean lens with lens tissue
   - Use optical cleaning solution if needed
   - Ensure no fingerprints or dust
   - Let air dry completely
   ```

2. **Prepare Standards**
   ```
   - Shake formazin standards gently
   - Allow bubbles to dissipate (5 minutes)
   - Ensure standards are at room temperature
   - Check expiration dates
   ```

### Two-Point Calibration (20 minutes)

#### Zero Point (0 NTU)

1. Fill optical cell with distilled water
2. Wipe outside of cell with lint-free cloth
3. Insert into sensor housing
4. Wait 30 seconds for reading to stabilize
5. Record voltage: ______ V
6. Expected: 2.5-3.0V (clear water, high voltage)

#### High Point (1000 NTU)

1. Shake 1000 NTU standard gently
2. Fill clean optical cell
3. Wipe outside
4. Insert into sensor
5. Wait 30 seconds
6. Record voltage: ______ V
7. Expected: 0.5-1.0V (turbid water, low voltage)

### Calculate Calibration

```
V_0NTU   = ______ V  (from step above)
V_1000NTU = ______ V  (from step above)

Linear relationship:
NTU = ((V_0NTU - V_measured) / (V_0NTU - V_1000NTU)) * 1000

Example:
If V_0NTU = 2.8V and V_1000NTU = 0.6V
For a measurement of 2.0V:
NTU = ((2.8 - 2.0) / (2.8 - 0.6)) * 1000
    = (0.8 / 2.2) * 1000
    = 364 NTU
```

### Update Firmware

Edit `firmware/esp32_main/sensors.h`:

```cpp
// Update these values in readTurbidityCalibrated() function
float calibration_0NTU = 2.8;    // Your measured value
float calibration_1000NTU = 0.6; // Your measured value
```

### Verification

Test with additional standards:

1. 100 NTU: Should read 100 ± 5 NTU
2. 500 NTU: Should read 500 ± 10 NTU

### Maintenance

- Clean optical surfaces before each use
- Replace optical cell if scratched
- Store sensor in dark, dry location
- Recalibrate if sensor is disassembled

## Temperature Sensor Calibration

### Factory Calibration

DS18B20 sensors are factory calibrated to ±0.5°C accuracy. Additional calibration is optional but recommended for critical applications.

### Optional Verification (30 minutes)

#### Ice Point (0°C)

1. Prepare ice bath:
   - Crushed ice + distilled water
   - Mix well to achieve 0°C
   - Let stabilize for 5 minutes

2. Immerse DS18B20 sensor
3. Wait 2 minutes for thermal equilibration
4. Record reading: ______ °C
5. Expected: 0.0 ± 0.5°C

#### Room Temperature (~20°C)

1. Place sensor and reference thermometer in still air
2. Wait 5 minutes
3. Record DS18B20: ______ °C
4. Record reference: ______ °C
5. Calculate offset: ______ °C

#### Hot Water (~40°C)

1. Prepare water bath at 40°C
2. Immerse both sensors
3. Wait 2 minutes
4. Record DS18B20: ______ °C
5. Record reference: ______ °C

### Apply Offset Correction

If systematic error detected:

```cpp
// In sensors.h or main firmware
float temperature = tempSensor.getTempCByIndex(0);
temperature += TEMP_OFFSET;  // Add measured offset
```

## GPS Calibration

GPS modules are self-calibrating and do not require manual calibration. However, performance can be verified:

### GPS Performance Check

1. Place module with clear sky view
2. Wait for full satellite lock (10+ satellites)
3. Let run for 5 minutes to stabilize
4. Compare reported position to known coordinates
5. Calculate position error

**Acceptable Performance:**
- Position error: <5 meters (2D RMS)
- Satellite count: >6 (preferably >8)
- HDOP (dilution of precision): <2.0

### If GPS Performance Poor

- Check antenna connection
- Verify antenna has clear sky view
- Update GPS firmware if available
- Consider external active antenna
- Check for interference sources

## Calibration Records

### Template Log Entry

```
Date: ________________
Operator: ________________
System Serial Number: ________________

pH Sensor:
  Buffer pH 4.0:  ______ mV
  Buffer pH 7.0:  ______ mV
  Buffer pH 10.0: ______ mV
  Calculated Slope: ______
  Calculated Offset: ______
  Verification pH 7.0: ______ (expected: 7.0)
  
Turbidity Sensor:
  0 NTU voltage: ______ V
  1000 NTU voltage: ______ V
  Verification 100 NTU: ______ (expected: 100)
  Verification 500 NTU: ______ (expected: 500)
  
Temperature Sensor:
  Ice point: ______ °C (expected: 0.0)
  Reference offset: ______ °C
  
GPS:
  Satellites: ______
  HDOP: ______
  Position error: ______ m
  
Notes:
_________________________________
_________________________________
_________________________________

Calibration Status: □ PASS  □ FAIL
Next Calibration Due: ________________
```

## Troubleshooting

### pH Sensor Issues

**Problem:** Erratic readings, slow response
- **Cause:** Dried out electrode, contamination
- **Solution:** Soak in storage solution overnight, clean carefully

**Problem:** Readings always near pH 7
- **Cause:** Broken glass bulb, internal damage
- **Solution:** Replace sensor

### Turbidity Sensor Issues

**Problem:** Reading always high (>500 NTU in clean water)
- **Cause:** Dirty optical surfaces
- **Solution:** Clean lens thoroughly, check for scratches

**Problem:** No change in readings
- **Cause:** Wiring issue, sensor failure
- **Solution:** Check connections, test with multimeter

### Temperature Sensor Issues

**Problem:** Reading -127°C
- **Cause:** Sensor not detected, wiring issue
- **Solution:** Check 1-Wire connection, verify power

**Problem:** Reading far from actual (~10°C off)
- **Cause:** Faulty sensor, poor thermal contact
- **Solution:** Verify with another sensor, replace if needed

## Safety Notes

- Handle buffer solutions with care (pH 4 and 10 are corrosive)
- Wear gloves when handling formazin standards
- Dispose of calibration solutions per local regulations
- Never ingest calibration standards
- Keep calibration materials away from food and drink

---

**Document Version:** 1.0.0  
**Last Updated:** 2024-01-15  
**Next Review:** 2025-01-15
