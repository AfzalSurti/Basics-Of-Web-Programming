/*
 * Telemetry Library for LoRa Communication
 * Handles data transmission to ground control station
 * 
 * Protocol: Custom packet-based telemetry over LoRa
 * Frequency: 915 MHz (US) / 868 MHz (Europe)
 */

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include <LoRa.h>

// Packet types
#define PACKET_HEARTBEAT    0x01
#define PACKET_SENSOR_DATA  0x02
#define PACKET_GPS_DATA     0x03
#define PACKET_STATUS       0x04
#define PACKET_COMMAND_ACK  0x05
#define PACKET_ERROR        0xFF

// Command types (received from ground station)
#define CMD_START_SAMPLING  0x10
#define CMD_STOP_SAMPLING   0x11
#define CMD_GET_STATUS      0x12
#define CMD_CALIBRATE       0x13
#define CMD_RESET           0x14

class TelemetryManager {
  private:
    unsigned long lastHeartbeat;
    const unsigned long HEARTBEAT_INTERVAL = 30000;  // 30 seconds
    
    int packetCounter;
    
    // CRC calculation for data integrity
    uint8_t calculateCRC(uint8_t* data, int length) {
      uint8_t crc = 0;
      for (int i = 0; i < length; i++) {
        crc ^= data[i];
      }
      return crc;
    }
    
  public:
    TelemetryManager() {
      lastHeartbeat = 0;
      packetCounter = 0;
    }
    
    void begin() {
      Serial.println(F("Telemetry manager initialized"));
    }
    
    // Send heartbeat packet
    void sendHeartbeat() {
      LoRa.beginPacket();
      LoRa.write(PACKET_HEARTBEAT);
      LoRa.write(packetCounter >> 8);
      LoRa.write(packetCounter & 0xFF);
      LoRa.print("ALIVE");
      LoRa.endPacket();
      
      packetCounter++;
      lastHeartbeat = millis();
      
      Serial.println(F("Heartbeat sent"));
    }
    
    // Send sensor data packet
    void sendSensorData(float pH, float turbidity, float temperature) {
      LoRa.beginPacket();
      LoRa.write(PACKET_SENSOR_DATA);
      LoRa.write(packetCounter >> 8);
      LoRa.write(packetCounter & 0xFF);
      
      // Convert floats to bytes (simple encoding)
      LoRa.print(pH, 2);
      LoRa.print("|");
      LoRa.print(turbidity, 2);
      LoRa.print("|");
      LoRa.print(temperature, 2);
      
      LoRa.endPacket();
      
      packetCounter++;
      
      Serial.print(F("Sensor data sent: pH="));
      Serial.print(pH, 2);
      Serial.print(F(", Turb="));
      Serial.print(turbidity, 2);
      Serial.print(F(", Temp="));
      Serial.println(temperature, 2);
    }
    
    // Send GPS data packet
    void sendGPSData(double latitude, double longitude, float altitude, int satellites) {
      LoRa.beginPacket();
      LoRa.write(PACKET_GPS_DATA);
      LoRa.write(packetCounter >> 8);
      LoRa.write(packetCounter & 0xFF);
      
      LoRa.print(latitude, 6);
      LoRa.print("|");
      LoRa.print(longitude, 6);
      LoRa.print("|");
      LoRa.print(altitude, 1);
      LoRa.print("|");
      LoRa.print(satellites);
      
      LoRa.endPacket();
      
      packetCounter++;
      
      Serial.print(F("GPS data sent: "));
      Serial.print(latitude, 6);
      Serial.print(F(", "));
      Serial.println(longitude, 6);
    }
    
    // Send status packet
    void sendStatus(bool sampling, bool gpsLock, bool sdCard, int errorCode) {
      LoRa.beginPacket();
      LoRa.write(PACKET_STATUS);
      LoRa.write(packetCounter >> 8);
      LoRa.write(packetCounter & 0xFF);
      
      uint8_t statusByte = 0;
      if (sampling) statusByte |= 0x01;
      if (gpsLock) statusByte |= 0x02;
      if (sdCard) statusByte |= 0x04;
      
      LoRa.write(statusByte);
      LoRa.write(errorCode);
      
      LoRa.endPacket();
      
      packetCounter++;
      
      Serial.println(F("Status packet sent"));
    }
    
    // Send complete telemetry packet (all data)
    void sendCompleteTelemetry(float pH, float turbidity, float temperature,
                               double latitude, double longitude, float altitude,
                               int satellites, bool sampling, bool gpsLock, 
                               bool sdCard, int errorCode) {
      
      String packet = "WQM|";  // Water Quality Monitor
      packet += String(packetCounter) + "|";
      packet += String(millis() / 1000) + "|";  // Uptime in seconds
      packet += String(pH, 2) + "|";
      packet += String(turbidity, 2) + "|";
      packet += String(temperature, 2) + "|";
      packet += String(latitude, 6) + "|";
      packet += String(longitude, 6) + "|";
      packet += String(altitude, 1) + "|";
      packet += String(satellites) + "|";
      packet += String(sampling ? "1" : "0") + "|";
      packet += String(gpsLock ? "1" : "0") + "|";
      packet += String(sdCard ? "1" : "0") + "|";
      packet += String(errorCode);
      
      LoRa.beginPacket();
      LoRa.print(packet);
      LoRa.endPacket();
      
      packetCounter++;
      
      Serial.println(F("Complete telemetry: ") + packet);
    }
    
    // Send error packet
    void sendError(int errorCode, String errorMessage) {
      LoRa.beginPacket();
      LoRa.write(PACKET_ERROR);
      LoRa.write(packetCounter >> 8);
      LoRa.write(packetCounter & 0xFF);
      LoRa.write(errorCode);
      LoRa.print(errorMessage);
      LoRa.endPacket();
      
      packetCounter++;
      
      Serial.print(F("Error packet sent: "));
      Serial.print(errorCode);
      Serial.print(F(" - "));
      Serial.println(errorMessage);
    }
    
    // Check for incoming commands
    bool checkForCommands() {
      int packetSize = LoRa.parsePacket();
      
      if (packetSize) {
        Serial.print(F("Received packet: "));
        
        // Read packet type
        uint8_t packetType = LoRa.read();
        
        // Read packet content
        String content = "";
        while (LoRa.available()) {
          content += (char)LoRa.read();
        }
        
        Serial.print(F("Type=0x"));
        Serial.print(packetType, HEX);
        Serial.print(F(", Data="));
        Serial.println(content);
        
        // Get RSSI (signal strength)
        int rssi = LoRa.packetRssi();
        Serial.print(F("RSSI: "));
        Serial.println(rssi);
        
        return true;
      }
      
      return false;
    }
    
    // Parse received command
    uint8_t parseCommand() {
      if (LoRa.available() < 1) {
        return 0;
      }
      
      uint8_t command = LoRa.read();
      
      Serial.print(F("Command received: 0x"));
      Serial.println(command, HEX);
      
      return command;
    }
    
    // Send command acknowledgment
    void sendCommandAck(uint8_t command, bool success) {
      LoRa.beginPacket();
      LoRa.write(PACKET_COMMAND_ACK);
      LoRa.write(packetCounter >> 8);
      LoRa.write(packetCounter & 0xFF);
      LoRa.write(command);
      LoRa.write(success ? 0x01 : 0x00);
      LoRa.endPacket();
      
      packetCounter++;
      
      Serial.print(F("Command ACK sent: 0x"));
      Serial.print(command, HEX);
      Serial.print(F(" - "));
      Serial.println(success ? "SUCCESS" : "FAILED");
    }
    
    // Auto-send heartbeat if interval elapsed
    void updateHeartbeat() {
      if (millis() - lastHeartbeat > HEARTBEAT_INTERVAL) {
        sendHeartbeat();
      }
    }
    
    // Get signal strength (RSSI)
    int getLastRSSI() {
      return LoRa.packetRssi();
    }
    
    // Get packet counter
    int getPacketCount() {
      return packetCounter;
    }
    
    // Reset packet counter
    void resetPacketCounter() {
      packetCounter = 0;
      Serial.println(F("Packet counter reset"));
    }
    
    // Get link budget information
    void printLinkInfo() {
      Serial.println(F("\n=== LoRa Link Information ==="));
      Serial.print(F("Frequency: "));
      Serial.print(LoRa.getFrequency() / 1E6);
      Serial.println(F(" MHz"));
      
      Serial.print(F("Spreading Factor: "));
      Serial.println(LoRa.getSpreadingFactor());
      
      Serial.print(F("Signal Bandwidth: "));
      Serial.print(LoRa.getSignalBandwidth() / 1E3);
      Serial.println(F(" kHz"));
      
      Serial.print(F("Packets sent: "));
      Serial.println(packetCounter);
      
      Serial.println(F("============================\n"));
    }
    
    // Test transmission
    bool testTransmission() {
      Serial.println(F("\n=== Testing LoRa Transmission ==="));
      
      // Send test packet
      LoRa.beginPacket();
      LoRa.print("TEST_TRANSMISSION");
      LoRa.endPacket();
      
      Serial.println(F("Test packet sent"));
      
      // Wait for acknowledgment (timeout 5 seconds)
      unsigned long startTime = millis();
      while (millis() - startTime < 5000) {
        if (LoRa.parsePacket()) {
          Serial.println(F("Response received - Link OK"));
          return true;
        }
        delay(100);
      }
      
      Serial.println(F("No response - Link may be down"));
      return false;
    }
};

#endif // TELEMETRY_H
