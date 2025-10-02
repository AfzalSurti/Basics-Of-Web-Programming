/*
 * Pump Control Library
 * Controls peristaltic pump for water sampling
 * 
 * Hardware: L298N motor driver connected to ESP32
 */

#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H

#include <Arduino.h>

class PumpController {
  private:
    int pin_IN1;
    int pin_IN2;
    int pin_PWM;
    
    bool isRunning;
    int currentSpeed;
    
    // PWM settings
    const int PWM_CHANNEL = 0;
    const int PWM_FREQ = 1000;  // 1 kHz
    const int PWM_RESOLUTION = 8;  // 8-bit resolution (0-255)
    
  public:
    PumpController() {
      isRunning = false;
      currentSpeed = 0;
    }
    
    void begin(int in1, int in2, int pwm) {
      pin_IN1 = in1;
      pin_IN2 = in2;
      pin_PWM = pwm;
      
      // Configure pins
      pinMode(pin_IN1, OUTPUT);
      pinMode(pin_IN2, OUTPUT);
      pinMode(pin_PWM, OUTPUT);
      
      // Configure PWM
      ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
      ledcAttachPin(pin_PWM, PWM_CHANNEL);
      
      // Initial state: stopped
      stop();
      
      Serial.println(F("Pump controller initialized"));
    }
    
    // Start pump in forward direction
    void start(int speedPercent = 100) {
      if (speedPercent < 0) speedPercent = 0;
      if (speedPercent > 100) speedPercent = 100;
      
      currentSpeed = speedPercent;
      
      // Set direction: Forward (IN1=HIGH, IN2=LOW)
      digitalWrite(pin_IN1, HIGH);
      digitalWrite(pin_IN2, LOW);
      
      // Set speed via PWM
      int pwmValue = map(speedPercent, 0, 100, 0, 255);
      ledcWrite(PWM_CHANNEL, pwmValue);
      
      isRunning = true;
      
      Serial.print(F("Pump started at "));
      Serial.print(speedPercent);
      Serial.println(F("% speed"));
    }
    
    // Start pump in reverse direction (for cleaning/purging)
    void startReverse(int speedPercent = 100) {
      if (speedPercent < 0) speedPercent = 0;
      if (speedPercent > 100) speedPercent = 100;
      
      currentSpeed = speedPercent;
      
      // Set direction: Reverse (IN1=LOW, IN2=HIGH)
      digitalWrite(pin_IN1, LOW);
      digitalWrite(pin_IN2, HIGH);
      
      // Set speed via PWM
      int pwmValue = map(speedPercent, 0, 100, 0, 255);
      ledcWrite(PWM_CHANNEL, pwmValue);
      
      isRunning = true;
      
      Serial.print(F("Pump started in REVERSE at "));
      Serial.print(speedPercent);
      Serial.println(F("% speed"));
    }
    
    // Stop pump
    void stop() {
      digitalWrite(pin_IN1, LOW);
      digitalWrite(pin_IN2, LOW);
      ledcWrite(PWM_CHANNEL, 0);
      
      isRunning = false;
      currentSpeed = 0;
      
      Serial.println(F("Pump stopped"));
    }
    
    // Change pump speed without stopping
    void setSpeed(int speedPercent) {
      if (!isRunning) {
        Serial.println(F("Pump not running. Use start() first."));
        return;
      }
      
      if (speedPercent < 0) speedPercent = 0;
      if (speedPercent > 100) speedPercent = 100;
      
      currentSpeed = speedPercent;
      int pwmValue = map(speedPercent, 0, 100, 0, 255);
      ledcWrite(PWM_CHANNEL, pwmValue);
      
      Serial.print(F("Pump speed set to "));
      Serial.print(speedPercent);
      Serial.println(F("%"));
    }
    
    // Gradually ramp up pump speed
    void rampUp(int targetSpeed, int stepDelay = 100) {
      Serial.println(F("Ramping up pump speed..."));
      
      if (!isRunning) {
        start(0);  // Start at 0 speed
      }
      
      for (int speed = currentSpeed; speed <= targetSpeed; speed += 5) {
        setSpeed(speed);
        delay(stepDelay);
      }
      
      setSpeed(targetSpeed);
      Serial.println(F("Ramp up complete"));
    }
    
    // Gradually ramp down pump speed
    void rampDown(int targetSpeed = 0, int stepDelay = 100) {
      Serial.println(F("Ramping down pump speed..."));
      
      if (!isRunning) {
        Serial.println(F("Pump not running"));
        return;
      }
      
      for (int speed = currentSpeed; speed >= targetSpeed; speed -= 5) {
        setSpeed(speed);
        delay(stepDelay);
      }
      
      if (targetSpeed == 0) {
        stop();
      } else {
        setSpeed(targetSpeed);
      }
      
      Serial.println(F("Ramp down complete"));
    }
    
    // Run pump for specific duration
    void runForDuration(unsigned long milliseconds, int speedPercent = 100) {
      Serial.print(F("Running pump for "));
      Serial.print(milliseconds / 1000.0);
      Serial.println(F(" seconds"));
      
      start(speedPercent);
      delay(milliseconds);
      stop();
    }
    
    // Calculate sampling time for desired volume
    // Flow rate: 100 mL/min at 100% speed
    unsigned long calculateSamplingTime(float volumeML, int speedPercent = 100) {
      // Base flow rate: 100 mL/min at 100% speed
      float baseFlowRate = 100.0;  // mL/min
      
      // Adjust flow rate based on speed
      float actualFlowRate = baseFlowRate * (speedPercent / 100.0);
      
      // Calculate time in minutes
      float timeMinutes = volumeML / actualFlowRate;
      
      // Convert to milliseconds
      unsigned long timeMillis = timeMinutes * 60.0 * 1000.0;
      
      Serial.print(F("To collect "));
      Serial.print(volumeML);
      Serial.print(F(" mL at "));
      Serial.print(speedPercent);
      Serial.print(F("% speed: "));
      Serial.print(timeMinutes);
      Serial.println(F(" minutes"));
      
      return timeMillis;
    }
    
    // Collect specific volume of water
    void collectSample(float volumeML, int speedPercent = 100) {
      unsigned long duration = calculateSamplingTime(volumeML, speedPercent);
      
      Serial.print(F("Collecting "));
      Serial.print(volumeML);
      Serial.println(F(" mL sample..."));
      
      // Ramp up for smooth start
      rampUp(speedPercent, 50);
      
      // Run for calculated duration
      delay(duration);
      
      // Ramp down for smooth stop
      rampDown(0, 50);
      
      Serial.println(F("Sample collection complete"));
    }
    
    // Purge/clean pump
    void purge(unsigned long durationMillis = 5000) {
      Serial.println(F("Purging pump..."));
      
      // Run forward
      start(50);
      delay(durationMillis / 2);
      
      // Run reverse
      startReverse(50);
      delay(durationMillis / 2);
      
      stop();
      
      Serial.println(F("Purge complete"));
    }
    
    // Prime pump (remove air from tubing)
    void prime() {
      Serial.println(F("Priming pump (removing air)..."));
      
      // Start slowly
      start(30);
      delay(2000);
      
      // Increase speed
      setSpeed(60);
      delay(3000);
      
      // Full speed briefly
      setSpeed(100);
      delay(2000);
      
      stop();
      
      Serial.println(F("Priming complete"));
    }
    
    // Get pump status
    bool getIsRunning() {
      return isRunning;
    }
    
    int getCurrentSpeed() {
      return currentSpeed;
    }
    
    // Emergency stop (immediate)
    void emergencyStop() {
      digitalWrite(pin_IN1, LOW);
      digitalWrite(pin_IN2, LOW);
      ledcWrite(PWM_CHANNEL, 0);
      
      isRunning = false;
      currentSpeed = 0;
      
      Serial.println(F("!!! EMERGENCY STOP !!!"));
    }
    
    // Self-test
    bool selfTest() {
      Serial.println(F("\n=== Pump Self-Test ==="));
      
      // Test forward direction
      Serial.print(F("Testing forward... "));
      start(50);
      delay(1000);
      if (isRunning) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("FAILED"));
        return false;
      }
      stop();
      delay(500);
      
      // Test reverse direction
      Serial.print(F("Testing reverse... "));
      startReverse(50);
      delay(1000);
      if (isRunning) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("FAILED"));
        return false;
      }
      stop();
      delay(500);
      
      // Test speed control
      Serial.print(F("Testing speed control... "));
      start(25);
      delay(500);
      setSpeed(75);
      delay(500);
      setSpeed(50);
      delay(500);
      stop();
      Serial.println(F("OK"));
      
      Serial.println(F("=== Self-Test Complete ===\n"));
      return true;
    }
};

#endif // PUMP_CONTROL_H
