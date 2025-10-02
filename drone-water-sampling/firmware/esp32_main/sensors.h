/*
 * Sensor Library for Water Quality Monitoring
 * Handles pH, turbidity, and temperature sensors
 * 
 * Hardware:
 * - pH sensor (analog via BNC)
 * - Turbidity sensor (analog)
 * - DS18B20 temperature sensor (1-Wire)
 * - MCP3008 ADC for analog sensors
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <SPI.h>

// ADC Constants
#define ADC_VREF 3.3
#define ADC_RESOLUTION 1024

// Sensor Channels on MCP3008
#define PH_CHANNEL 0
#define TURBIDITY_CHANNEL 1

// Calibration Constants
#define PH_CALIBRATION_SLOPE 3.5
#define PH_CALIBRATION_OFFSET 7.0
#define TURBIDITY_CALIBRATION_FACTOR 5.0

class WaterSensors {
  private:
    int csPin;
    
    // ADC reading function
    int readADC(int channel) {
      // MCP3008 SPI protocol
      byte commandByte1 = 0b00000001;  // Start bit
      byte commandByte2 = (channel + 8) << 4;  // Single-ended mode, channel select
      
      digitalWrite(csPin, LOW);
      
      SPI.transfer(commandByte1);
      byte highByte = SPI.transfer(commandByte2);
      byte lowByte = SPI.transfer(0x00);
      
      digitalWrite(csPin, HIGH);
      
      // Combine bytes (10-bit result)
      int result = ((highByte & 0x03) << 8) | lowByte;
      
      return result;
    }
    
    // Voltage calculation
    float adcToVoltage(int adcValue) {
      return (adcValue * ADC_VREF) / ADC_RESOLUTION;
    }
    
  public:
    WaterSensors() {
      csPin = 32;  // Default ADC CS pin
    }
    
    void begin() {
      pinMode(csPin, OUTPUT);
      digitalWrite(csPin, HIGH);
      
      SPI.begin();
      SPI.setClockDivider(SPI_CLOCK_DIV16);  // ~1 MHz
      
      // Allow sensors to stabilize
      delay(1000);
    }
    
    // Read pH sensor
    float readpH() {
      // Take multiple readings for stability
      long sum = 0;
      const int samples = 10;
      
      for (int i = 0; i < samples; i++) {
        sum += readADC(PH_CHANNEL);
        delay(10);
      }
      
      int avgReading = sum / samples;
      float voltage = adcToVoltage(avgReading);
      
      // Convert voltage to pH using calibration
      // pH = (voltage - offset) / slope
      float pH = PH_CALIBRATION_OFFSET - ((voltage - 2.5) * PH_CALIBRATION_SLOPE);
      
      // Constrain to valid pH range
      if (pH < 0) pH = 0;
      if (pH > 14) pH = 14;
      
      return pH;
    }
    
    // Read turbidity sensor
    float readTurbidity() {
      // Take multiple readings
      long sum = 0;
      const int samples = 10;
      
      for (int i = 0; i < samples; i++) {
        sum += readADC(TURBIDITY_CHANNEL);
        delay(10);
      }
      
      int avgReading = sum / samples;
      float voltage = adcToVoltage(avgReading);
      
      // Convert voltage to NTU (Nephelometric Turbidity Units)
      // Lower voltage = higher turbidity (inverse relationship)
      // NTU = calibration_factor / voltage * 1000
      float turbidity = 0;
      
      if (voltage < 2.5) {
        turbidity = 3000;  // Max turbidity
      } else if (voltage > 4.0) {
        turbidity = 0;  // Clear water
      } else {
        // Linear interpolation for voltage between 2.5V and 4.0V
        turbidity = (4.0 - voltage) / 1.5 * 3000;
      }
      
      return turbidity;
    }
    
    // Alternative method using specific calibration points
    float readTurbidityCalibrated(float calibration_0NTU, float calibration_3000NTU) {
      int avgReading = 0;
      const int samples = 10;
      
      for (int i = 0; i < samples; i++) {
        avgReading += readADC(TURBIDITY_CHANNEL);
        delay(10);
      }
      
      avgReading /= samples;
      float voltage = adcToVoltage(avgReading);
      
      // Linear interpolation between calibration points
      float turbidity = ((calibration_3000NTU - voltage) / 
                        (calibration_3000NTU - calibration_0NTU)) * 3000;
      
      if (turbidity < 0) turbidity = 0;
      if (turbidity > 3000) turbidity = 3000;
      
      return turbidity;
    }
    
    // Calibration helper - get raw voltage
    float getRawVoltage(int channel) {
      int reading = readADC(channel);
      return adcToVoltage(reading);
    }
    
    // pH calibration helper
    void calibratepH(float pH4Voltage, float pH7Voltage, float pH10Voltage) {
      // Three-point pH calibration
      // Calculate slope from pH 4 to pH 10
      float slope = (10.0 - 4.0) / (pH10Voltage - pH4Voltage);
      
      // Calculate offset using pH 7 point
      float offset = 7.0 - (pH7Voltage * slope);
      
      Serial.println(F("pH Calibration Results:"));
      Serial.print(F("  Slope: ")); Serial.println(slope, 4);
      Serial.print(F("  Offset: ")); Serial.println(offset, 4);
      Serial.println(F("Update PH_CALIBRATION_SLOPE and PH_CALIBRATION_OFFSET in code."));
    }
    
    // Temperature compensation for pH
    float compensatepHForTemp(float pH, float tempC) {
      // pH changes with temperature
      // Typical: -0.03 pH units per degree C from 25°C
      float tempOffset = (tempC - 25.0) * (-0.03);
      return pH + tempOffset;
    }
    
    // Check if sensor readings are valid
    bool isValidpH(float pH) {
      return (pH >= 0 && pH <= 14);
    }
    
    bool isValidTurbidity(float turbidity) {
      return (turbidity >= 0 && turbidity <= 3000);
    }
    
    // Self-test function
    bool selfTest() {
      Serial.println(F("\n=== Sensor Self-Test ==="));
      
      // Test pH sensor
      Serial.print(F("Testing pH sensor... "));
      float testpH = readpH();
      if (isValidpH(testpH)) {
        Serial.print(F("OK (pH: ")); Serial.print(testpH, 2); Serial.println(F(")"));
      } else {
        Serial.println(F("FAILED"));
        return false;
      }
      
      // Test turbidity sensor
      Serial.print(F("Testing turbidity sensor... "));
      float testTurb = readTurbidity();
      if (isValidTurbidity(testTurb)) {
        Serial.print(F("OK (Turbidity: ")); Serial.print(testTurb, 2); Serial.println(F(" NTU)"));
      } else {
        Serial.println(F("FAILED"));
        return false;
      }
      
      Serial.println(F("=== Self-Test Complete ===\n"));
      return true;
    }
};

#endif // SENSORS_H
