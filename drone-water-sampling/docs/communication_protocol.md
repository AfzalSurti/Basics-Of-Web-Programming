# Communication Protocol Specification

## Overview

This document defines the telemetry communication protocol used between the drone water sampling system and the ground control station via LoRa radio.

## LoRa Configuration

### Physical Layer Parameters

| Parameter | Value | Notes |
|-----------|-------|-------|
| Frequency | 915 MHz (US) / 868 MHz (EU) | ISM band |
| Bandwidth | 125 kHz | Optimal for range |
| Spreading Factor | 7 | Balance of speed/range |
| Coding Rate | 4/5 | Forward error correction |
| TX Power | 20 dBm | Maximum allowed |
| Sync Word | 0x12 | Private network |
| Preamble Length | 8 symbols | Standard |
| CRC | Enabled | Data integrity |

### Link Budget

```
TX Power:        +20 dBm
RX Sensitivity:  -136 dBm (SF7, BW125)
Link Budget:     156 dB
Estimated Range: 10+ km (line of sight)
```

## Packet Format

### General Structure

All packets follow this structure:

```
┌────────┬──────────┬─────────────┬────────┬─────┐
│ Header │ Packet # │   Payload   │  CRC   │ End │
└────────┴──────────┴─────────────┴────────┴─────┘
  1 byte    2 bytes    Variable     1 byte  1 byte
```

### Packet Types

| Type Code | Name | Direction | Description |
|-----------|------|-----------|-------------|
| 0x01 | HEARTBEAT | UAV → GCS | Periodic status |
| 0x02 | SENSOR_DATA | UAV → GCS | Water quality data |
| 0x03 | GPS_DATA | UAV → GCS | Location info |
| 0x04 | STATUS | UAV → GCS | System status |
| 0x05 | COMMAND_ACK | UAV → GCS | Command acknowledgment |
| 0x10 | START_SAMPLING | GCS → UAV | Begin sampling |
| 0x11 | STOP_SAMPLING | GCS → UAV | Stop sampling |
| 0x12 | GET_STATUS | GCS → UAV | Status request |
| 0x13 | CALIBRATE | GCS → UAV | Calibration command |
| 0xFF | ERROR | UAV → GCS | Error notification |

## Detailed Packet Formats

### 1. Complete Telemetry Packet (0x02)

**Most commonly used packet with all data**

Format:
```
WQM|packet_id|uptime|pH|turbidity|temp|lat|lon|alt|sats|sampling|gps|sd|error
```

Example:
```
WQM|123|1845|7.2|15.5|18.3|37.774900|-122.419400|0.5|10|1|1|1|0
```

Field Description:
```
WQM          - Header identifier
123          - Packet counter
1845         - System uptime (seconds)
7.2          - pH value
15.5         - Turbidity (NTU)
18.3         - Temperature (°C)
37.774900    - Latitude (degrees)
-122.419400  - Longitude (degrees)
0.5          - Altitude (meters)
10           - Satellite count
1            - Sampling active (1=yes, 0=no)
1            - GPS lock (1=yes, 0=no)
1            - SD card OK (1=yes, 0=no)
0            - Error code (0=none)
```

**Size:** ~80 bytes (variable)  
**Transmission Time:** ~150ms at SF7  
**Frequency:** Every 10 seconds

### 2. Heartbeat Packet (0x01)

**Simple alive message**

Binary Format:
```
┌────┬────┬────┬─────────┐
│0x01│ MSB│ LSB│ "ALIVE" │
└────┴────┴────┴─────────┘
```

Fields:
- `0x01`: Packet type
- `MSB/LSB`: 16-bit packet counter
- `"ALIVE"`: ASCII string

**Size:** 8 bytes  
**Frequency:** Every 30 seconds  
**Purpose:** Link health monitoring

### 3. Sensor Data Packet (0x02)

**Compact sensor-only packet**

ASCII Format:
```
SENS|packet_id|pH|turbidity|temperature
```

Example:
```
SENS|456|7.2|15.5|18.3
```

**Size:** ~25 bytes  
**Frequency:** Every 5 seconds during sampling

### 4. GPS Data Packet (0x03)

**Location information**

ASCII Format:
```
GPS|packet_id|latitude|longitude|altitude|satellites
```

Example:
```
GPS|789|37.774900|-122.419400|0.5|10
```

**Size:** ~40 bytes  
**Frequency:** Every 5 seconds

### 5. Status Packet (0x04)

**System health information**

Binary Format:
```
┌────┬────┬────┬──────┬──────┐
│0x04│ MSB│ LSB│Status│Error │
└────┴────┴────┴──────┴──────┘
```

Status Byte (bitfield):
```
Bit 0: Sampling active
Bit 1: GPS lock
Bit 2: SD card OK
Bit 3: LoRa OK
Bit 4: Pump running
Bit 5: Low battery
Bit 6-7: Reserved
```

Error Code:
```
0x00: No error
0x01: SD card error
0x02: LoRa error
0x04: GPS error
0x08: Sensor error
0x10: Pump error
0x20: Low battery
0x40: Temperature out of range
0x80: Critical error
```

**Size:** 6 bytes  
**Frequency:** Every 10 seconds

### 6. Command Packets (GCS → UAV)

#### Start Sampling (0x10)
```
┌────┬────┬────┬──────┬────────┐
│0x10│ MSB│ LSB│Volume│Duration│
└────┴────┴────┴──────┴────────┘
```
- Volume: Target volume in mL (uint16)
- Duration: Max duration in seconds (uint16)

#### Stop Sampling (0x11)
```
┌────┬────┬────┐
│0x11│ MSB│ LSB│
└────┴────┴────┘
```

#### Get Status (0x12)
```
┌────┬────┬────┐
│0x12│ MSB│ LSB│
└────┴────┴────┘
```

#### Calibrate (0x13)
```
┌────┬────┬────┬──────┐
│0x13│ MSB│ LSB│Sensor│
└────┴────┴────┴──────┘
```
Sensor codes:
- 0x01: pH
- 0x02: Turbidity
- 0x03: Temperature
- 0xFF: All sensors

### 7. Command Acknowledgment (0x05)

**Response to commands**

Binary Format:
```
┌────┬────┬────┬───────┬────────┐
│0x05│ MSB│ LSB│Command│Success │
└────┴────┴────┴───────┴────────┘
```

- Command: Echo of command byte received
- Success: 0x01=success, 0x00=failed

### 8. Error Packet (0xFF)

**Error notification**

ASCII Format:
```
ERROR|code|message
```

Example:
```
ERROR|0x08|pH sensor not responding
```

**Size:** Variable (max 100 bytes)

## Communication Patterns

### Normal Operation Sequence

```
GCS                         UAV
 │                           │
 │◄──────HEARTBEAT───────────│  Every 30s
 │                           │
 │◄─────SENSOR_DATA──────────│  Every 5s
 │                           │
 │◄──────GPS_DATA────────────│  Every 5s
 │                           │
 │──────START_SAMPLING──────►│  User initiated
 │                           │
 │◄──────COMMAND_ACK─────────│  Immediate
 │                           │
 │◄─────SENSOR_DATA──────────│  Continuous during sampling
 │◄─────SENSOR_DATA──────────│
 │◄─────SENSOR_DATA──────────│
 │         ...               │
 │                           │
 │◄──────STATUS──────────────│  Sampling complete
 │                           │
```

### Link Health Monitoring

```
┌─────────────────────────────┐
│   Heartbeat every 30s       │
│                             │
│   If no heartbeat for 60s:  │
│   ├─ Display "Link Lost"    │
│   ├─ Log warning            │
│   └─ Alert operator         │
│                             │
│   If no heartbeat for 120s: │
│   ├─ Assume link failed     │
│   ├─ UAV enters failsafe    │
│   └─ Automatic RTL          │
└─────────────────────────────┘
```

### Error Handling

```
┌─────────────────────────────┐
│  Packet Checksum Failed     │
│  ├─ Discard packet          │
│  ├─ Increment error counter │
│  └─ Request retransmission  │
│     if critical data        │
└─────────────────────────────┘

┌─────────────────────────────┐
│  Command Not Acknowledged   │
│  ├─ Wait 5 seconds          │
│  ├─ Retry transmission      │
│  ├─ Max 3 retries           │
│  └─ Alert operator if fail  │
└─────────────────────────────┘
```

## Data Rates and Timing

### Maximum Data Rates

| SF | BW (kHz) | Data Rate | Airtime (100 bytes) |
|----|----------|-----------|---------------------|
| 7  | 125      | 5470 bps  | ~150 ms            |
| 8  | 125      | 3125 bps  | ~260 ms            |
| 9  | 125      | 1760 bps  | ~450 ms            |
| 10 | 125      | 980 bps   | ~800 ms            |

**Current Configuration:** SF7, 125kHz → ~150ms per 100-byte packet

### Transmission Schedule

```
Time    Packet Type        Size    Duration
─────────────────────────────────────────────
T+0s    Complete Telemetry  80B     120ms
T+5s    Sensor Data         25B      40ms
T+10s   Complete Telemetry  80B     120ms
T+15s   Sensor Data         25B      40ms
T+20s   Complete Telemetry  80B     120ms
T+25s   Sensor Data         25B      40ms
T+30s   Heartbeat            8B      15ms
T+30s   Complete Telemetry  80B     120ms
...
```

**Average bandwidth usage:** ~1.5% of channel capacity  
**Collision probability:** Very low with proper timing

## Security Considerations

### Current Implementation

- **Private Network:** Custom sync word (0x12)
- **Basic Validation:** CRC checksum
- **Packet Filtering:** Header validation

### Limitations

⚠️ **Not encrypted** - LoRa packets can be intercepted  
⚠️ **No authentication** - Commands can be spoofed  
⚠️ **Open protocol** - Anyone can decode packets

### Recommended Enhancements

For production/sensitive applications:

1. **Encryption:** AES-128 packet encryption
2. **Authentication:** HMAC command verification
3. **Rolling Codes:** Prevent replay attacks
4. **Frequency Hopping:** Spread spectrum for security

## Integration Examples

### Python (Ground Station)

```python
import serial
import time

# Open LoRa serial connection
lora = serial.Serial('/dev/ttyUSB0', 115200)

while True:
    if lora.in_waiting:
        packet = lora.readline().decode('utf-8').strip()
        
        if packet.startswith('WQM|'):
            # Parse complete telemetry
            fields = packet.split('|')
            pH = float(fields[3])
            turbidity = float(fields[4])
            temperature = float(fields[5])
            
            print(f"pH: {pH}, Turb: {turbidity}, Temp: {temperature}")
    
    time.sleep(0.1)
```

### Arduino (UAV)

```cpp
#include <LoRa.h>

void sendTelemetry() {
    String packet = "WQM|";
    packet += String(packetCounter) + "|";
    packet += String(millis()/1000) + "|";
    packet += String(pH, 2) + "|";
    packet += String(turbidity, 2) + "|";
    packet += String(temperature, 2) + "|";
    // ... add remaining fields
    
    LoRa.beginPacket();
    LoRa.print(packet);
    LoRa.endPacket();
}
```

## Troubleshooting

### Weak Signal (RSSI < -120 dBm)

- Reduce distance between UAV and GCS
- Check antenna orientation
- Verify antenna connections
- Check for obstructions

### Packet Loss

- Increase spreading factor (SF8 or SF9)
- Reduce transmission frequency
- Check for interference (other LoRa devices)
- Verify ground plane on antennas

### Garbled Data

- Check baud rate settings
- Verify UART connections
- Look for electromagnetic interference
- Check for loose connections

## Compliance

### Regulatory

- **FCC Part 15** (USA): Unlicensed operation at 915 MHz
- **ETSI EN 300 220** (Europe): 868 MHz ISM band
- **Maximum duty cycle:** 1% (EU), continuous (US)
- **Maximum EIRP:** +30 dBm (US), +14 dBm (EU)

### Best Practices

- Limit transmission duration
- Use lowest power necessary
- Implement listen-before-talk (LBT)
- Follow good neighbor policy
- Monitor channel usage

---

**Document Version:** 1.0.0  
**Date:** 2024-01-15  
**Revision History:** Initial release  
**Contact:** See project repository for updates
