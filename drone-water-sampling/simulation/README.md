# Water Sampling Mission Simulation

## Overview

This directory contains simulation scripts and configuration files for testing the drone water sampling system in a virtual environment before deploying to real hardware.

## Files

- `mission_script.py` - Main mission execution script
- `mission_config.yaml` - Mission configuration parameters
- `gazebo_world.sdf` - Gazebo simulation world with water body
- `results/` - Sample telemetry logs and simulation outputs

## Prerequisites

### Software Requirements

1. **Python 3.7+** with pip

2. **MAVProxy** (MAVLink ground control station)
   ```bash
   pip install MAVProxy
   ```

3. **DroneKit-Python** (Drone programming library)
   ```bash
   pip install dronekit
   pip install dronekit-sitl
   ```

4. **ArduPilot SITL** (Software-in-the-Loop simulator)
   ```bash
   pip install pymavlink
   ```

5. **Gazebo** (Optional - for 3D visualization)
   ```bash
   # Ubuntu/Debian
   sudo apt-get install gazebo11 libgazebo11-dev
   
   # macOS
   brew install gazebo
   ```

6. **Python Dependencies**
   ```bash
   pip install PyYAML numpy matplotlib
   ```

## Quick Start

### 1. Start SITL Simulator

Open a terminal and start the ArduCopter SITL:

```bash
dronekit-sitl copter --home=37.7749,-122.4194,0,0
```

This will simulate a quadcopter at the specified home location (San Francisco Bay Area).

### 2. Start MAVProxy

In a new terminal, connect MAVProxy to SITL:

```bash
mavproxy.py --master tcp:127.0.0.1:5760 --out udp:127.0.0.1:14550
```

You should see the MAVProxy console with flight data.

### 3. Run Mission Script

In a third terminal, execute the mission script:

```bash
cd simulation
python3 mission_script.py
```

The script will:
- Connect to the simulated vehicle
- Perform pre-flight checks
- Execute the complete water sampling mission
- Display progress and sensor readings
- Return to launch and land

## Configuration

### Editing Mission Parameters

Edit `mission_config.yaml` to customize:

```yaml
# Change target location
target_location:
  latitude: 37.7749
  longitude: -122.4194
  altitude: 0.5

# Adjust flight parameters
flight:
  takeoff_altitude: 10.0
  cruise_altitude: 20.0
  sampling_altitude: 0.5

# Configure sampling
sampling:
  target_volume: 500  # mL
  duration: 300  # seconds
```

### Loading Configuration in Script

The mission script can load configuration from YAML:

```python
mission = WaterSamplingMission()
mission.load_config('mission_config.yaml')
mission.execute_mission()
```

## Simulation Modes

### Mode 1: SITL Only (No Graphics)

Fastest simulation, text-based output only:

```bash
# Terminal 1
dronekit-sitl copter

# Terminal 2
python3 mission_script.py
```

### Mode 2: SITL + MAVProxy

With MAVProxy for monitoring and manual control:

```bash
# Terminal 1
dronekit-sitl copter

# Terminal 2
mavproxy.py --master tcp:127.0.0.1:5760

# Terminal 3
python3 mission_script.py
```

### Mode 3: SITL + Gazebo (Full 3D Visualization)

Complete 3D simulation with physics:

```bash
# Terminal 1
gazebo --verbose worlds/lake_sampling.world

# Terminal 2 (in Gazebo-ArduPilot plugin directory)
./start_sitl.sh

# Terminal 3
python3 mission_script.py
```

## Mission Execution Flow

```
1. Initialize
   ├─ Connect to vehicle
   ├─ Wait for GPS fix
   └─ Pre-flight checks

2. Takeoff
   ├─ Arm motors
   ├─ Takeoff to 10m
   └─ Climb to cruise altitude (20m)

3. Navigate
   ├─ Fly to target GPS coordinates
   └─ Monitor battery and position

4. Sampling Approach
   ├─ Arrive at target location
   ├─ Descend to 0.5m above water
   └─ Stabilize hover

5. Water Sampling
   ├─ Lower suction pipe (0.5m)
   ├─ Activate peristaltic pump
   ├─ Collect 500mL sample (5 minutes)
   ├─ Read sensors (pH, turbidity, temp)
   ├─ Transmit data via LoRa
   └─ Retract suction pipe

6. Return Home
   ├─ Climb to cruise altitude
   ├─ Navigate back to launch point
   ├─ Descend
   └─ Land and disarm
```

## Testing Scenarios

### Scenario 1: Normal Operation
```bash
python3 mission_script.py
```
Expected: Complete mission with successful sampling and return.

### Scenario 2: Low Battery
Modify battery level in SITL or config to test RTL trigger:
```yaml
safety:
  min_battery_level: 80.0  # Force low battery scenario
```

### Scenario 3: GPS Loss
Test GPS failsafe behavior:
```python
# In MAVProxy console
param set GPS_TYPE 0  # Disable GPS
```

### Scenario 4: Wind Conditions
Add wind in Gazebo or SITL:
```bash
# In MAVProxy
param set SIM_WIND_SPD 12.0  # 12 m/s wind
```

### Scenario 5: Manual Override
Take control during mission:
```bash
# In MAVProxy console
mode STABILIZE  # Switch to manual control
```

## Monitoring and Debugging

### Real-time Telemetry

Monitor mission progress:
```python
# In mission_script.py, add logging
print(f"Lat: {vehicle.location.global_frame.lat}")
print(f"Lon: {vehicle.location.global_frame.lon}")
print(f"Alt: {vehicle.location.global_relative_frame.alt}")
print(f"Battery: {vehicle.battery.level}%")
```

### Log Files

Mission logs are saved to:
- `results/mission_log_[timestamp].txt` - Text log
- `results/telemetry_[timestamp].csv` - Sensor data
- `results/flight_path_[timestamp].kml` - GPS track (Google Earth)

### Visualization

Plot flight path:
```python
import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('results/telemetry_*.csv')
plt.plot(data['longitude'], data['latitude'])
plt.xlabel('Longitude')
plt.ylabel('Latitude')
plt.title('Flight Path')
plt.show()
```

## Troubleshooting

### Connection Issues

**Problem**: "Failed to connect to vehicle"
```
Solution:
1. Verify SITL is running (check port 5760)
2. Check firewall settings
3. Try: telnet 127.0.0.1 5760
```

**Problem**: "GPS fix not acquired"
```
Solution:
1. Wait longer (SITL GPS takes 30-60 seconds)
2. Check GPS parameters in MAVProxy:
   param show GPS*
3. Reset GPS: param set GPS_TYPE 1
```

### Arming Issues

**Problem**: "Vehicle not armable"
```
Solution:
1. Check pre-arm errors in MAVProxy
2. Common issues:
   - GPS not locked
   - Compass not calibrated
   - Mode not GUIDED
   - Safety switch enabled
3. Force arm (simulation only):
   arm throttle force
```

### Mission Execution Issues

**Problem**: "Mission aborted"
```
Solution:
1. Check logs for error messages
2. Verify battery level
3. Check geofence settings
4. Review mission config parameters
```

## Advanced Usage

### Custom Mission Sequences

Create custom sampling patterns:

```python
# Multiple waypoint sampling
waypoints = [
    LocationGlobalRelative(37.7749, -122.4194, 0.5),
    LocationGlobalRelative(37.7750, -122.4190, 0.5),
    LocationGlobalRelative(37.7755, -122.4195, 0.5),
]

for wp in waypoints:
    mission.fly_to_location(wp)
    mission.perform_sampling()
```

### Integration with External Sensors

Simulate sensor readings from files:

```python
# Load sensor data from CSV
sensor_data = pd.read_csv('sensor_calibration.csv')
mission.use_simulated_sensors(sensor_data)
```

### Automated Testing

Run multiple missions with different parameters:

```bash
#!/bin/bash
for altitude in 0.5 1.0 1.5; do
    python3 mission_script.py --altitude $altitude
done
```

## Safety Guidelines

⚠️ **Important Simulation Safety**:

1. Always test in simulation before real flight
2. Verify GPS coordinates (don't fly to random locations)
3. Check battery and altitude limits
4. Test failsafe behaviors
5. Practice manual override
6. Review logs after each simulation
7. Validate sensor readings
8. Test in various weather conditions (simulated)

## Sample Outputs

Example telemetry log (`results/telemetry_example.csv`):
```csv
timestamp,latitude,longitude,altitude,pH,turbidity,temperature,battery
1705334400,37.7749,-122.4194,20.0,7.2,15.5,18.3,95
1705334410,37.7750,-122.4192,18.5,7.2,15.5,18.3,94
1705334420,37.7750,-122.4190,15.0,7.2,15.5,18.3,93
...
```

## Performance Metrics

Expected simulation performance:
- Mission duration: ~15-20 minutes
- Data points collected: ~180-240 (1 per 5 seconds)
- Battery consumption: ~70-75%
- GPS accuracy: ±2 meters
- Altitude hold accuracy: ±0.2 meters

## Next Steps

After successful simulation:

1. Review all mission logs
2. Verify sensor readings are reasonable
3. Test failsafe scenarios
4. Proceed to hardware testing (see `docs/flight_operations.md`)
5. Conduct ground tests with actual hardware
6. Perform tethered test flights
7. Execute first autonomous mission with supervision

## Support

For issues with simulation:
- Check DroneKit documentation: https://dronekit-python.readthedocs.io
- ArduPilot SITL guide: https://ardupilot.org/dev/docs/sitl-simulator-software-in-the-loop.html
- Gazebo tutorials: http://gazebosim.org/tutorials

## License

This simulation code is part of the Drone Water Sampling System project and is provided for educational and testing purposes.
