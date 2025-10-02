/*
 * ESP32 Water Quality Monitoring System
 * Main Firmware for Drone-Based Water Sampling
 * 
 * Features:
 * - pH, turbidity, and temperature monitoring
 * - Peristaltic pump control
 * - GPS location tracking
 * - LoRa telemetry transmission
 * - Data logging to microSD card
 * - Status indicators (LEDs, buzzer)
 * 
 * Hardware: ESP32-WROOM-32
 * Author: Drone Water Sampling Team
 * Version: 1.0.0
 * Date: 2024-01-15
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <LoRa.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sensors.h"
#include "pump_control.h"
#include "telemetry.h"

// ==================== PIN DEFINITIONS ====================
// LoRa Module
#define LORA_SS_PIN     4
#define LORA_RST_PIN    2
#define LORA_DIO0_PIN   2
#define LORA_SCK_PIN    5
#define LORA_MISO_PIN   18
#define LORA_MOSI_PIN   19

// GPS Module (UART2)
#define GPS_RX_PIN      16
#define GPS_TX_PIN      17

// SD Card
#define SD_CS_PIN       13
#define SD_SCK_PIN      14
#define SD_MISO_PIN     12
#define SD_MOSI_PIN     23

// 1-Wire (Temperature)
#define ONE_WIRE_PIN    15

// ADC (MCP3008) - SPI
#define ADC_CS_PIN      32
#define ADC_SCK_PIN     33

// Motor Driver (L298N)
#define MOTOR_IN1_PIN   25
#define MOTOR_IN2_PIN   26
#define MOTOR_PWM_PIN   27

// Status LEDs
#define LED_POWER       34
#define LED_GPS         35
#define LED_LORA        36
#define LED_SAMPLING    37
#define LED_ERROR       38

// Buzzer
#define BUZZER_PIN      39

// Buttons
#define BTN_RESET       21
#define BTN_SAMPLE      22

// ==================== GLOBAL OBJECTS ====================
TinyGPSPlus gps;
HardwareSerial GPS_Serial(2);

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature tempSensor(&oneWire);

WaterSensors sensors;
PumpController pump;
TelemetryManager telemetry;

// ==================== GLOBAL VARIABLES ====================
struct SensorData {
  float pH;
  float turbidity;
  float temperature;
  double latitude;
  double longitude;
  float altitude;
  int satellites;
  unsigned long timestamp;
} currentData;

struct SystemState {
  bool sdCardReady;
  bool gpsFixed;
  bool loraReady;
  bool sampling;
  int errorCode;
} systemState;

File dataFile;
unsigned long lastSampleTime = 0;
unsigned long lastTelemetryTime = 0;
const unsigned long SAMPLE_INTERVAL = 5000;  // 5 seconds
const unsigned long TELEMETRY_INTERVAL = 10000;  // 10 seconds

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println(F("================================="));
  Serial.println(F("ESP32 Water Quality Monitor v1.0"));
  Serial.println(F("================================="));
  
  // Initialize pins
  initializePins();
  
  // Initialize peripherals
  initializeSD();
  initializeGPS();
  initializeLoRa();
  initializeSensors();
  initializePump();
  
  // System ready
  systemState.errorCode = 0;
  playStartupTone();
  
  Serial.println(F("System initialized successfully!"));
  Serial.println(F("Ready for water sampling operations."));
}

// ==================== MAIN LOOP ====================
void loop() {
  // Update GPS data
  updateGPS();
  
  // Check for sampling command
  if (digitalRead(BTN_SAMPLE) == LOW || systemState.sampling) {
    performSampling();
  }
  
  // Read sensors at interval
  if (millis() - lastSampleTime > SAMPLE_INTERVAL) {
    readAllSensors();
    logDataToSD();
    lastSampleTime = millis();
  }
  
  // Send telemetry at interval
  if (millis() - lastTelemetryTime > TELEMETRY_INTERVAL) {
    sendTelemetry();
    lastTelemetryTime = millis();
  }
  
  // Update status LEDs
  updateStatusLEDs();
  
  // Small delay to prevent watchdog issues
  delay(10);
}

// ==================== INITIALIZATION FUNCTIONS ====================

void initializePins() {
  Serial.println(F("Initializing pins..."));
  
  // LED pins
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_GPS, OUTPUT);
  pinMode(LED_LORA, OUTPUT);
  pinMode(LED_SAMPLING, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  
  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Buttons
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BTN_SAMPLE, INPUT_PULLUP);
  
  // Initial LED state
  digitalWrite(LED_POWER, HIGH);
  digitalWrite(LED_GPS, LOW);
  digitalWrite(LED_LORA, LOW);
  digitalWrite(LED_SAMPLING, LOW);
  digitalWrite(LED_ERROR, LOW);
  
  Serial.println(F("  Pins initialized"));
}

void initializeSD() {
  Serial.print(F("Initializing SD card..."));
  
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println(F(" FAILED!"));
    systemState.sdCardReady = false;
    systemState.errorCode |= 0x01;
    return;
  }
  
  systemState.sdCardReady = true;
  Serial.println(F(" OK"));
  
  // Create data file
  String filename = "/water_data_" + String(millis()) + ".csv";
  dataFile = SD.open(filename, FILE_WRITE);
  
  if (dataFile) {
    // Write CSV header
    dataFile.println("Timestamp,pH,Turbidity,Temperature,Latitude,Longitude,Altitude,Satellites");
    dataFile.close();
    Serial.println(F("  Data file created: ") + filename);
  }
}

void initializeGPS() {
  Serial.print(F("Initializing GPS..."));
  
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  delay(500);
  
  Serial.println(F(" OK"));
  Serial.println(F("  Waiting for GPS fix (this may take 2-5 minutes)..."));
  
  systemState.gpsFixed = false;
}

void initializeLoRa() {
  Serial.print(F("Initializing LoRa..."));
  
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
  
  if (!LoRa.begin(915E6)) {  // 915 MHz for US, use 868E6 for Europe
    Serial.println(F(" FAILED!"));
    systemState.loraReady = false;
    systemState.errorCode |= 0x02;
    return;
  }
  
  // Configure LoRa parameters
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(0x12);
  LoRa.enableCrc();
  
  systemState.loraReady = true;
  Serial.println(F(" OK"));
  Serial.println(F("  Frequency: 915 MHz, SF: 7, BW: 125 kHz"));
}

void initializeSensors() {
  Serial.println(F("Initializing sensors..."));
  
  // Initialize temperature sensor
  tempSensor.begin();
  Serial.println(F("  Temperature sensor: OK"));
  
  // Initialize pH and turbidity sensors (via ADC)
  sensors.begin();
  Serial.println(F("  pH sensor: OK"));
  Serial.println(F("  Turbidity sensor: OK"));
}

void initializePump() {
  Serial.print(F("Initializing pump controller..."));
  
  pump.begin(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_PWM_PIN);
  pump.stop();
  
  Serial.println(F(" OK"));
}

// ==================== SENSOR READING FUNCTIONS ====================

void readAllSensors() {
  // Read pH
  currentData.pH = sensors.readpH();
  
  // Read turbidity
  currentData.turbidity = sensors.readTurbidity();
  
  // Read temperature
  tempSensor.requestTemperatures();
  currentData.temperature = tempSensor.getTempCByIndex(0);
  
  // Get GPS data
  if (gps.location.isValid()) {
    currentData.latitude = gps.location.lat();
    currentData.longitude = gps.location.lng();
    currentData.altitude = gps.altitude.meters();
    currentData.satellites = gps.satellites.value();
  }
  
  currentData.timestamp = millis();
  
  // Print to serial
  Serial.println(F("\n--- Sensor Readings ---"));
  Serial.print(F("pH: ")); Serial.println(currentData.pH, 2);
  Serial.print(F("Turbidity: ")); Serial.print(currentData.turbidity, 2); Serial.println(F(" NTU"));
  Serial.print(F("Temperature: ")); Serial.print(currentData.temperature, 2); Serial.println(F(" °C"));
  
  if (systemState.gpsFixed) {
    Serial.print(F("Location: ")); 
    Serial.print(currentData.latitude, 6); Serial.print(F(", "));
    Serial.println(currentData.longitude, 6);
    Serial.print(F("Altitude: ")); Serial.print(currentData.altitude, 1); Serial.println(F(" m"));
    Serial.print(F("Satellites: ")); Serial.println(currentData.satellites);
  } else {
    Serial.println(F("GPS: No fix"));
  }
  Serial.println(F("-----------------------"));
}

// ==================== GPS FUNCTIONS ====================

void updateGPS() {
  while (GPS_Serial.available() > 0) {
    char c = GPS_Serial.read();
    gps.encode(c);
    
    if (gps.location.isUpdated()) {
      if (!systemState.gpsFixed) {
        systemState.gpsFixed = true;
        Serial.println(F("\nGPS Fix Acquired!"));
        tone(BUZZER_PIN, 2000, 200);
      }
    }
  }
}

// ==================== SAMPLING FUNCTIONS ====================

void performSampling() {
  Serial.println(F("\n=== Starting Water Sampling ==="));
  systemState.sampling = true;
  digitalWrite(LED_SAMPLING, HIGH);
  
  // Step 1: Start pump
  Serial.println(F("Step 1: Starting pump..."));
  pump.start(100);  // 100% speed
  tone(BUZZER_PIN, 1500, 100);
  delay(100);
  tone(BUZZER_PIN, 1500, 100);
  
  // Step 2: Run pump for calculated time
  // 500 mL at 100 mL/min = 5 minutes = 300 seconds
  Serial.println(F("Step 2: Collecting 500 mL sample (5 minutes)..."));
  unsigned long samplingDuration = 300000;  // 5 minutes in milliseconds
  unsigned long startTime = millis();
  
  while (millis() - startTime < samplingDuration) {
    // Continue reading sensors during sampling
    if (millis() - lastSampleTime > SAMPLE_INTERVAL) {
      readAllSensors();
      logDataToSD();
      lastSampleTime = millis();
    }
    
    // Update GPS
    updateGPS();
    
    // Blink LED during sampling
    digitalWrite(LED_SAMPLING, (millis() / 500) % 2);
    
    // Display progress
    if ((millis() - startTime) % 30000 == 0) {  // Every 30 seconds
      Serial.print(F("Progress: "));
      Serial.print((millis() - startTime) / 1000);
      Serial.println(F(" seconds"));
    }
    
    delay(100);
  }
  
  // Step 3: Stop pump
  Serial.println(F("Step 3: Stopping pump..."));
  pump.stop();
  delay(1000);
  
  // Step 4: Final sensor reading
  Serial.println(F("Step 4: Final measurements..."));
  readAllSensors();
  logDataToSD();
  
  // Step 5: Send telemetry
  Serial.println(F("Step 5: Transmitting data..."));
  sendTelemetry();
  
  systemState.sampling = false;
  digitalWrite(LED_SAMPLING, LOW);
  
  // Success tone
  tone(BUZZER_PIN, 2000, 200);
  delay(300);
  tone(BUZZER_PIN, 2500, 200);
  delay(300);
  tone(BUZZER_PIN, 3000, 200);
  
  Serial.println(F("=== Sampling Complete ===\n"));
}

// ==================== DATA LOGGING ====================

void logDataToSD() {
  if (!systemState.sdCardReady) return;
  
  String filename = "/water_data_" + String(millis() / 300000) + ".csv";
  dataFile = SD.open(filename, FILE_APPEND);
  
  if (dataFile) {
    // Write CSV line
    dataFile.print(currentData.timestamp); dataFile.print(",");
    dataFile.print(currentData.pH, 2); dataFile.print(",");
    dataFile.print(currentData.turbidity, 2); dataFile.print(",");
    dataFile.print(currentData.temperature, 2); dataFile.print(",");
    dataFile.print(currentData.latitude, 6); dataFile.print(",");
    dataFile.print(currentData.longitude, 6); dataFile.print(",");
    dataFile.print(currentData.altitude, 1); dataFile.print(",");
    dataFile.println(currentData.satellites);
    
    dataFile.close();
  } else {
    Serial.println(F("Error writing to SD card"));
    systemState.errorCode |= 0x04;
  }
}

// ==================== TELEMETRY ====================

void sendTelemetry() {
  if (!systemState.loraReady) return;
  
  digitalWrite(LED_LORA, HIGH);
  
  // Build telemetry packet
  String packet = "WQ|";
  packet += String(currentData.timestamp) + "|";
  packet += String(currentData.pH, 2) + "|";
  packet += String(currentData.turbidity, 2) + "|";
  packet += String(currentData.temperature, 2) + "|";
  packet += String(currentData.latitude, 6) + "|";
  packet += String(currentData.longitude, 6) + "|";
  packet += String(currentData.altitude, 1) + "|";
  packet += String(currentData.satellites);
  
  // Send via LoRa
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
  
  Serial.println(F("Telemetry sent: ") + packet);
  
  digitalWrite(LED_LORA, LOW);
}

// ==================== STATUS UPDATES ====================

void updateStatusLEDs() {
  // Power LED - always on
  digitalWrite(LED_POWER, HIGH);
  
  // GPS LED - solid when fixed, blink when searching
  if (systemState.gpsFixed) {
    digitalWrite(LED_GPS, HIGH);
  } else {
    digitalWrite(LED_GPS, (millis() / 1000) % 2);
  }
  
  // LoRa LED - brief flash after transmission
  // (handled in sendTelemetry function)
  
  // Error LED - on if any errors
  digitalWrite(LED_ERROR, systemState.errorCode != 0);
}

void playStartupTone() {
  tone(BUZZER_PIN, 1000, 100);
  delay(150);
  tone(BUZZER_PIN, 1500, 100);
  delay(150);
  tone(BUZZER_PIN, 2000, 200);
}
