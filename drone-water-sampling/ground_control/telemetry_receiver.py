#!/usr/bin/env python3
"""
LoRa Telemetry Receiver
Ground station software for receiving and processing telemetry from water sampling drone

Receives data packets via LoRa radio and displays/logs information
Compatible with RFM95W LoRa module

Author: Drone Water Sampling Team
Version: 1.0.0
Date: 2024-01-15
"""

import time
import datetime
import csv
import json
from dataclasses import dataclass, asdict
from typing import Optional

try:
    import serial
    import serial.tools.list_ports
except ImportError:
    print("Error: pyserial not installed")
    print("Install with: pip install pyserial")
    exit(1)

@dataclass
class TelemetryPacket:
    """Data structure for telemetry packets"""
    timestamp: str
    packet_id: int
    pH: float
    turbidity: float
    temperature: float
    latitude: float
    longitude: float
    altitude: float
    satellites: int
    battery: float
    sampling: bool
    gps_lock: bool
    rssi: int  # Signal strength

class LoRaTelemetryReceiver:
    """LoRa telemetry receiver and processor"""
    
    def __init__(self, port='/dev/ttyUSB0', baud_rate=115200):
        """
        Initialize telemetry receiver
        
        Args:
            port: Serial port for LoRa module
            baud_rate: Serial communication baud rate
        """
        self.port = port
        self.baud_rate = baud_rate
        self.serial_connection = None
        self.running = False
        self.packet_count = 0
        self.last_packet_time = None
        
        # Data storage
        self.telemetry_log = []
        self.log_file = f"telemetry_log_{datetime.datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
        
        print("=" * 60)
        print("LoRa Telemetry Receiver v1.0")
        print("=" * 60)
    
    def list_serial_ports(self):
        """List available serial ports"""
        print("\nAvailable serial ports:")
        ports = serial.tools.list_ports.comports()
        for i, port in enumerate(ports):
            print(f"  {i+1}. {port.device} - {port.description}")
        return [port.device for port in ports]
    
    def connect(self):
        """Connect to LoRa module"""
        try:
            print(f"\nConnecting to LoRa module on {self.port}...")
            self.serial_connection = serial.Serial(
                port=self.port,
                baudrate=self.baud_rate,
                timeout=1.0
            )
            time.sleep(2)  # Wait for connection to stabilize
            print("Connected successfully!")
            return True
            
        except serial.SerialException as e:
            print(f"Error connecting to serial port: {e}")
            print("\nTroubleshooting:")
            print("1. Check if device is connected")
            print("2. Verify correct port name")
            print("3. Check if port is in use by another program")
            print("4. Try: sudo usermod -a -G dialout $USER (Linux)")
            return False
    
    def initialize_csv_log(self):
        """Initialize CSV log file with headers"""
        headers = [
            'Timestamp', 'Packet_ID', 'pH', 'Turbidity', 'Temperature',
            'Latitude', 'Longitude', 'Altitude', 'Satellites',
            'Battery', 'Sampling', 'GPS_Lock', 'RSSI'
        ]
        
        with open(self.log_file, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(headers)
        
        print(f"\nLogging to: {self.log_file}")
    
    def parse_packet(self, raw_data):
        """
        Parse incoming telemetry packet
        
        Expected format:
        WQM|packet_id|uptime|pH|turbidity|temp|lat|lon|alt|sats|sampling|gps|sd|error
        """
        try:
            # Remove whitespace and split
            data = raw_data.strip().split('|')
            
            if len(data) < 14 or data[0] != 'WQM':
                return None
            
            packet = TelemetryPacket(
                timestamp=datetime.datetime.now().isoformat(),
                packet_id=int(data[1]),
                pH=float(data[3]),
                turbidity=float(data[4]),
                temperature=float(data[5]),
                latitude=float(data[6]),
                longitude=float(data[7]),
                altitude=float(data[8]),
                satellites=int(data[9]),
                battery=0.0,  # Not in this packet format
                sampling=data[10] == '1',
                gps_lock=data[11] == '1',
                rssi=-100  # Placeholder, would come from LoRa module
            )
            
            return packet
            
        except (IndexError, ValueError) as e:
            print(f"Error parsing packet: {e}")
            print(f"Raw data: {raw_data}")
            return None
    
    def display_packet(self, packet: TelemetryPacket):
        """Display packet information in terminal"""
        print("\n" + "=" * 60)
        print(f"TELEMETRY PACKET #{packet.packet_id}")
        print("=" * 60)
        print(f"Time:        {packet.timestamp}")
        print(f"RSSI:        {packet.rssi} dBm")
        print("\nWater Quality:")
        print(f"  pH:          {packet.pH:.2f}")
        print(f"  Turbidity:   {packet.turbidity:.2f} NTU")
        print(f"  Temperature: {packet.temperature:.2f} °C")
        print("\nLocation:")
        print(f"  Latitude:    {packet.latitude:.6f}°")
        print(f"  Longitude:   {packet.longitude:.6f}°")
        print(f"  Altitude:    {packet.altitude:.1f} m")
        print(f"  Satellites:  {packet.satellites}")
        print("\nStatus:")
        print(f"  GPS Lock:    {'✓ Yes' if packet.gps_lock else '✗ No'}")
        print(f"  Sampling:    {'✓ Active' if packet.sampling else '○ Inactive'}")
        print(f"  Battery:     {packet.battery:.0f}%")
        print("=" * 60)
    
    def log_packet(self, packet: TelemetryPacket):
        """Log packet to CSV file"""
        with open(self.log_file, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([
                packet.timestamp,
                packet.packet_id,
                packet.pH,
                packet.turbidity,
                packet.temperature,
                packet.latitude,
                packet.longitude,
                packet.altitude,
                packet.satellites,
                packet.battery,
                int(packet.sampling),
                int(packet.gps_lock),
                packet.rssi
            ])
    
    def send_command(self, command):
        """Send command to drone via LoRa"""
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.write(command.encode() + b'\n')
            print(f"Command sent: {command}")
        else:
            print("Error: Not connected to LoRa module")
    
    def receive_loop(self):
        """Main receive loop"""
        print("\nStarting receive loop...")
        print("Listening for telemetry packets...")
        print("Press Ctrl+C to stop\n")
        
        self.running = True
        consecutive_errors = 0
        max_consecutive_errors = 10
        
        while self.running:
            try:
                if self.serial_connection.in_waiting > 0:
                    # Read line from serial
                    line = self.serial_connection.readline().decode('utf-8', errors='ignore')
                    
                    if line.strip():
                        print(f"Received: {line.strip()}")
                        
                        # Parse packet
                        packet = self.parse_packet(line)
                        
                        if packet:
                            self.packet_count += 1
                            self.last_packet_time = datetime.datetime.now()
                            
                            # Display and log
                            self.display_packet(packet)
                            self.log_packet(packet)
                            self.telemetry_log.append(packet)
                            
                            consecutive_errors = 0
                        else:
                            print("Warning: Invalid packet format")
                            consecutive_errors += 1
                
                # Check for connection timeout
                if self.last_packet_time:
                    time_since_last = (datetime.datetime.now() - self.last_packet_time).seconds
                    if time_since_last > 60:
                        print(f"\nWarning: No packets received for {time_since_last} seconds")
                
                # Stop if too many errors
                if consecutive_errors >= max_consecutive_errors:
                    print(f"\nError: {max_consecutive_errors} consecutive parsing errors. Stopping.")
                    break
                
                time.sleep(0.1)  # Small delay to prevent CPU overload
                
            except KeyboardInterrupt:
                print("\n\nReceive loop interrupted by user")
                break
                
            except Exception as e:
                print(f"Error in receive loop: {e}")
                consecutive_errors += 1
    
    def generate_summary(self):
        """Generate summary statistics"""
        if not self.telemetry_log:
            print("\nNo telemetry data received")
            return
        
        print("\n" + "=" * 60)
        print("SESSION SUMMARY")
        print("=" * 60)
        print(f"Total packets received: {self.packet_count}")
        print(f"Log file: {self.log_file}")
        
        # Calculate averages
        if self.telemetry_log:
            avg_pH = sum(p.pH for p in self.telemetry_log) / len(self.telemetry_log)
            avg_turbidity = sum(p.turbidity for p in self.telemetry_log) / len(self.telemetry_log)
            avg_temp = sum(p.temperature for p in self.telemetry_log) / len(self.telemetry_log)
            
            print("\nAverage Sensor Readings:")
            print(f"  pH:          {avg_pH:.2f}")
            print(f"  Turbidity:   {avg_turbidity:.2f} NTU")
            print(f"  Temperature: {avg_temp:.2f} °C")
        
        print("=" * 60)
    
    def disconnect(self):
        """Close serial connection"""
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()
            print("\nDisconnected from LoRa module")
    
    def run(self):
        """Main run method"""
        # List available ports
        self.list_serial_ports()
        
        # Connect to LoRa module
        if not self.connect():
            return False
        
        # Initialize logging
        self.initialize_csv_log()
        
        try:
            # Start receive loop
            self.receive_loop()
            
        finally:
            # Generate summary and cleanup
            self.generate_summary()
            self.disconnect()
        
        return True

def main():
    """Main entry point"""
    import argparse
    
    parser = argparse.ArgumentParser(description='LoRa Telemetry Receiver')
    parser.add_argument('--port', '-p', default='/dev/ttyUSB0',
                       help='Serial port (default: /dev/ttyUSB0)')
    parser.add_argument('--baud', '-b', type=int, default=115200,
                       help='Baud rate (default: 115200)')
    
    args = parser.parse_args()
    
    # Create receiver
    receiver = LoRaTelemetryReceiver(port=args.port, baud_rate=args.baud)
    
    # Run
    success = receiver.run()
    
    return 0 if success else 1

if __name__ == "__main__":
    exit(main())
