#!/usr/bin/env python3
"""
Water Sampling Mission Script
Automated drone mission for water sampling operations

Uses DroneKit-Python for MAVLink communication with flight controller
Supports ArduCopter/Pixhawk autopilots

Mission Sequence:
1. Takeoff to altitude
2. Navigate to GPS waypoint (water body location)
3. Descend to sampling altitude (0.5m above water)
4. Lower suction pipe
5. Activate pump and collect sample
6. Read sensors (pH, turbidity, temperature)
7. Transmit data via LoRa
8. Raise suction pipe
9. Return to home
10. Land

Author: Drone Water Sampling Team
Version: 1.0.0
Date: 2024-01-15
"""

import time
import math
from dronekit import connect, VehicleMode, LocationGlobalRelative
from pymavlink import mavutil

class WaterSamplingMission:
    def __init__(self, connection_string='tcp:127.0.0.1:5760'):
        """
        Initialize water sampling mission
        
        Args:
            connection_string: MAVLink connection string
                              Examples:
                              - SITL: 'tcp:127.0.0.1:5760'
                              - Serial: '/dev/ttyUSB0'
                              - UDP: 'udp:127.0.0.1:14550'
        """
        print("=" * 50)
        print("Water Sampling Mission Script v1.0")
        print("=" * 50)
        
        self.vehicle = None
        self.connection_string = connection_string
        
        # Mission parameters (can be loaded from config file)
        self.takeoff_altitude = 10.0  # meters
        self.cruise_altitude = 20.0   # meters
        self.sampling_altitude = 0.5  # meters above water
        self.pipe_extension_depth = 0.5  # meters below water surface
        self.sampling_duration = 300  # seconds (5 minutes for 500mL at 100mL/min)
        
        # Target location (example: lake/reservoir)
        self.target_location = LocationGlobalRelative(37.7749, -122.4194, 0.5)
        
        # Safety parameters
        self.min_battery = 30.0  # percent
        self.max_wind_speed = 10.0  # m/s
        self.geofence_radius = 500.0  # meters from home
        
    def connect_vehicle(self):
        """Connect to the vehicle"""
        print(f"\nConnecting to vehicle on: {self.connection_string}")
        
        try:
            self.vehicle = connect(self.connection_string, wait_ready=True, timeout=60)
            print("Connected successfully!")
            
            # Display vehicle information
            print("\n--- Vehicle Information ---")
            print(f"Vehicle Type: {self.vehicle.version}")
            print(f"Autopilot Firmware: {self.vehicle.version}")
            print(f"Autopilot Version: {self.vehicle.version.release_type()}")
            print(f"Battery: {self.vehicle.battery.voltage}V, {self.vehicle.battery.level}%")
            print(f"GPS: {self.vehicle.gps_0.fix_type}, {self.vehicle.gps_0.satellites_visible} sats")
            print(f"Mode: {self.vehicle.mode.name}")
            print(f"Armable: {self.vehicle.is_armable}")
            print(f"Armed: {self.vehicle.armed}")
            
            return True
            
        except Exception as e:
            print(f"Failed to connect: {str(e)}")
            return False
    
    def wait_for_gps_fix(self, timeout=60):
        """Wait for GPS to get a good fix"""
        print("\nWaiting for GPS fix...")
        start_time = time.time()
        
        while time.time() - start_time < timeout:
            gps = self.vehicle.gps_0
            if gps.fix_type >= 3 and gps.satellites_visible >= 6:
                print(f"GPS fix acquired: Type {gps.fix_type}, {gps.satellites_visible} satellites")
                return True
            
            print(f"  GPS status: Fix type {gps.fix_type}, {gps.satellites_visible} satellites visible")
            time.sleep(2)
        
        print("WARNING: GPS fix timeout!")
        return False
    
    def pre_flight_checks(self):
        """Perform pre-flight safety checks"""
        print("\n--- Pre-Flight Checks ---")
        
        checks_passed = True
        
        # Check battery level
        battery_level = self.vehicle.battery.level
        if battery_level is not None and battery_level < self.min_battery:
            print(f"✗ Battery too low: {battery_level}% (minimum {self.min_battery}%)")
            checks_passed = False
        else:
            print(f"✓ Battery OK: {battery_level}%")
        
        # Check GPS
        if self.vehicle.gps_0.fix_type < 3:
            print("✗ GPS fix not adequate")
            checks_passed = False
        else:
            print("✓ GPS OK")
        
        # Check home location
        if self.vehicle.home_location is None:
            print("✗ Home location not set")
            checks_passed = False
        else:
            print(f"✓ Home location set: {self.vehicle.home_location}")
        
        # Check armable
        if not self.vehicle.is_armable:
            print("✗ Vehicle not armable")
            checks_passed = False
        else:
            print("✓ Vehicle is armable")
        
        # Check mode
        if self.vehicle.mode.name != 'GUIDED':
            print(f"  Current mode: {self.vehicle.mode.name}")
            print("  Switching to GUIDED mode...")
            self.vehicle.mode = VehicleMode("GUIDED")
            time.sleep(2)
            if self.vehicle.mode.name == 'GUIDED':
                print("✓ Mode set to GUIDED")
            else:
                print("✗ Failed to set GUIDED mode")
                checks_passed = False
        else:
            print("✓ Mode is GUIDED")
        
        return checks_passed
    
    def arm_and_takeoff(self, target_altitude):
        """Arm vehicle and takeoff to target altitude"""
        print(f"\n--- Arming and Taking Off to {target_altitude}m ---")
        
        # Arm vehicle
        print("Arming motors...")
        self.vehicle.armed = True
        
        # Wait for arming
        timeout = 30
        start_time = time.time()
        while not self.vehicle.armed:
            if time.time() - start_time > timeout:
                print("ERROR: Failed to arm vehicle")
                return False
            print("  Waiting for arming...")
            time.sleep(1)
        
        print("Motors armed!")
        
        # Takeoff
        print(f"Taking off to {target_altitude}m...")
        self.vehicle.simple_takeoff(target_altitude)
        
        # Wait for altitude
        while True:
            current_altitude = self.vehicle.location.global_relative_frame.alt
            print(f"  Altitude: {current_altitude:.1f}m")
            
            if current_altitude >= target_altitude * 0.95:
                print(f"Reached target altitude: {current_altitude:.1f}m")
                break
            
            time.sleep(1)
        
        return True
    
    def fly_to_location(self, location, altitude=None):
        """Fly to a specific GPS location"""
        if altitude is not None:
            location.alt = altitude
        
        print(f"\n--- Flying to Location ---")
        print(f"Target: Lat {location.lat:.6f}, Lon {location.lon:.6f}, Alt {location.alt:.1f}m")
        
        self.vehicle.simple_goto(location)
        
        # Monitor progress
        while True:
            current_location = self.vehicle.location.global_relative_frame
            distance = self.get_distance_metres(current_location, location)
            
            print(f"  Distance to target: {distance:.1f}m, Altitude: {current_location.alt:.1f}m")
            
            if distance < 2.0:  # Within 2 meters
                print("Reached target location")
                break
            
            time.sleep(2)
    
    def get_distance_metres(self, location1, location2):
        """Calculate distance between two GPS coordinates"""
        dlat = location2.lat - location1.lat
        dlon = location2.lon - location1.lon
        
        return math.sqrt((dlat*dlat) + (dlon*dlon)) * 1.113195e5
    
    def descend_to_altitude(self, target_altitude):
        """Descend to specific altitude"""
        print(f"\n--- Descending to {target_altitude}m ---")
        
        current_location = self.vehicle.location.global_relative_frame
        target_location = LocationGlobalRelative(
            current_location.lat,
            current_location.lon,
            target_altitude
        )
        
        self.vehicle.simple_goto(target_location)
        
        while True:
            current_altitude = self.vehicle.location.global_relative_frame.alt
            print(f"  Altitude: {current_altitude:.1f}m")
            
            if abs(current_altitude - target_altitude) < 0.2:
                print(f"Reached target altitude: {current_altitude:.1f}m")
                break
            
            time.sleep(1)
    
    def perform_sampling(self):
        """Perform water sampling operation"""
        print("\n" + "=" * 50)
        print("STARTING WATER SAMPLING OPERATION")
        print("=" * 50)
        
        # Step 1: Stabilize hover
        print("\nStep 1: Stabilizing hover...")
        time.sleep(3)
        print("Position stabilized")
        
        # Step 2: Lower suction pipe
        print(f"\nStep 2: Lowering suction pipe to {self.pipe_extension_depth}m depth...")
        # In real implementation, this would control a servo/actuator
        time.sleep(5)
        print("Suction pipe deployed")
        
        # Step 3: Activate pump and collect sample
        print(f"\nStep 3: Activating pump (collecting 500mL sample, {self.sampling_duration}s)...")
        print("Pump started...")
        
        # Simulate sampling with progress updates
        start_time = time.time()
        last_update = 0
        
        while time.time() - start_time < self.sampling_duration:
            elapsed = time.time() - start_time
            progress = (elapsed / self.sampling_duration) * 100
            volume_collected = (elapsed / self.sampling_duration) * 500  # mL
            
            if elapsed - last_update >= 30:  # Update every 30 seconds
                print(f"  Progress: {progress:.1f}% ({volume_collected:.1f} mL collected)")
                print(f"  Battery: {self.vehicle.battery.level}%")
                print(f"  Altitude: {self.vehicle.location.global_relative_frame.alt:.1f}m")
                last_update = elapsed
            
            time.sleep(1)
        
        print("Sample collection complete: 500 mL")
        print("Pump stopped")
        
        # Step 4: Read sensors
        print("\nStep 4: Reading water quality sensors...")
        time.sleep(2)
        
        # Simulated sensor readings
        sensor_data = {
            'pH': 7.2,
            'turbidity': 15.5,  # NTU
            'temperature': 18.3,  # Celsius
            'latitude': self.vehicle.location.global_frame.lat,
            'longitude': self.vehicle.location.global_frame.lon,
            'altitude': self.vehicle.location.global_relative_frame.alt,
            'timestamp': time.time()
        }
        
        print("--- Sensor Readings ---")
        print(f"pH: {sensor_data['pH']}")
        print(f"Turbidity: {sensor_data['turbidity']} NTU")
        print(f"Temperature: {sensor_data['temperature']} °C")
        print(f"Location: {sensor_data['latitude']:.6f}, {sensor_data['longitude']:.6f}")
        print(f"Altitude: {sensor_data['altitude']:.1f}m")
        print("-" * 23)
        
        # Step 5: Transmit data via LoRa
        print("\nStep 5: Transmitting data via LoRa...")
        time.sleep(2)
        print("Data transmission complete")
        
        # Step 6: Raise suction pipe
        print("\nStep 6: Raising suction pipe...")
        time.sleep(5)
        print("Suction pipe retracted")
        
        print("\n" + "=" * 50)
        print("SAMPLING OPERATION COMPLETE")
        print("=" * 50)
        
        return sensor_data
    
    def return_to_launch(self):
        """Return to launch point and land"""
        print("\n--- Returning to Launch ---")
        
        self.vehicle.mode = VehicleMode("RTL")
        
        # Monitor return
        while self.vehicle.armed:
            altitude = self.vehicle.location.global_relative_frame.alt
            mode = self.vehicle.mode.name
            print(f"  Mode: {mode}, Altitude: {altitude:.1f}m")
            
            if altitude < 0.5:
                print("Landing detected")
                break
            
            time.sleep(2)
        
        print("Vehicle has landed and disarmed")
    
    def execute_mission(self):
        """Execute complete water sampling mission"""
        print("\n" + "=" * 50)
        print("EXECUTING WATER SAMPLING MISSION")
        print("=" * 50)
        
        try:
            # Connect to vehicle
            if not self.connect_vehicle():
                return False
            
            # Wait for GPS
            if not self.wait_for_gps_fix():
                print("ERROR: GPS fix not acquired")
                return False
            
            # Pre-flight checks
            if not self.pre_flight_checks():
                print("ERROR: Pre-flight checks failed")
                return False
            
            # Arm and takeoff
            if not self.arm_and_takeoff(self.takeoff_altitude):
                print("ERROR: Takeoff failed")
                return False
            
            # Climb to cruise altitude
            print(f"\nClimbing to cruise altitude: {self.cruise_altitude}m")
            self.descend_to_altitude(self.cruise_altitude)
            
            # Fly to sampling location
            self.fly_to_location(self.target_location, self.cruise_altitude)
            
            # Descend to sampling altitude
            self.descend_to_altitude(self.sampling_altitude)
            
            # Perform sampling
            sensor_data = self.perform_sampling()
            
            # Return to launch
            self.return_to_launch()
            
            print("\n" + "=" * 50)
            print("MISSION COMPLETED SUCCESSFULLY")
            print("=" * 50)
            
            return True
            
        except KeyboardInterrupt:
            print("\n\nMission interrupted by user")
            if self.vehicle:
                print("Initiating emergency RTL...")
                self.vehicle.mode = VehicleMode("RTL")
            return False
            
        except Exception as e:
            print(f"\nERROR: Mission failed - {str(e)}")
            if self.vehicle:
                print("Initiating emergency RTL...")
                self.vehicle.mode = VehicleMode("RTL")
            return False
            
        finally:
            if self.vehicle:
                print("\nClosing vehicle connection...")
                self.vehicle.close()
                print("Connection closed")

def main():
    """Main entry point"""
    # Create mission instance
    mission = WaterSamplingMission(connection_string='tcp:127.0.0.1:5760')
    
    # Optional: Load mission parameters from config file
    # mission.load_config('mission_config.yaml')
    
    # Execute mission
    success = mission.execute_mission()
    
    if success:
        print("\nMission execution successful!")
        return 0
    else:
        print("\nMission execution failed!")
        return 1

if __name__ == "__main__":
    exit(main())
