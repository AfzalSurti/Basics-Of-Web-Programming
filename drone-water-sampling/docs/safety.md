# Safety Guidelines - Drone Water Sampling System

## ⚠️ Critical Safety Information

**READ AND UNDERSTAND ALL SAFETY INFORMATION BEFORE OPERATING THIS SYSTEM**

## Table of Contents

1. [Regulatory Compliance](#regulatory-compliance)
2. [Pre-Flight Safety](#pre-flight-safety)
3. [Operational Safety](#operational-safety)
4. [Emergency Procedures](#emergency-procedures)
5. [Environmental Safety](#environmental-safety)
6. [Personal Protective Equipment](#personal-protective-equipment)

## Regulatory Compliance

### United States (FAA)

**Part 107 Requirements:**
- Remote Pilot Certificate required
- Maximum altitude: 400 feet AGL
- Visual line of sight mandatory
- No operations over people
- Daylight operations only (or with waiver)
- Pre-flight inspection required
- Weather minimums: 3 statute miles visibility

**Registration:**
- UAV must be registered with FAA
- Registration number displayed on aircraft
- Current registration certificate available

### Europe (EASA)

- Follow EU Drone Regulations 2019/947 and 2019/945
- Obtain appropriate operator certificate
- Maintain operational limitations
- Respect national aviation authority rules

### Other Jurisdictions

- Research and comply with local regulations
- Obtain necessary permits and authorizations
- Respect no-fly zones and restricted airspace
- Follow local environmental regulations

## Pre-Flight Safety

### Battery Safety

**Inspection:**
- ✓ Check for swelling, damage, or deformation
- ✓ Verify cell voltage balance (<0.1V difference)
- ✓ Confirm proper charge level (>90% for mission)
- ✓ Inspect connectors for damage or corrosion
- ✓ Check wire insulation integrity

**Handling:**
- Never short circuit battery terminals
- Do not puncture or drop batteries
- Keep away from heat sources and flames
- Store at room temperature (20-25°C)
- Use LiPo safety bag for charging and storage

**Charging:**
- Use appropriate charger only
- Never leave unattended while charging
- Charge on non-flammable surface
- Balance charge every cycle
- Stop if battery becomes warm (>45°C)

**Disposal:**
- Discharge to storage voltage before disposal
- Never discard in regular trash
- Take to battery recycling facility
- Follow local hazardous waste regulations

### Equipment Inspection

**UAV Airframe:**
- [ ] Propellers: No cracks, chips, or deformation
- [ ] Motors: Spin freely, no grinding sounds
- [ ] Arms: No cracks or stress marks
- [ ] Frame: All screws tight, no damage
- [ ] Wiring: Secure, no exposed conductors

**Water Sampling System:**
- [ ] Bottle: Clean, properly seated, leak-free
- [ ] Pump: Operational, correct direction
- [ ] Pipe: Extended properly, filter clean
- [ ] Sensors: Clean, calibrated, responding
- [ ] Mounting: Secure, balanced, no movement

**Electronics:**
- [ ] All connections secure
- [ ] No exposed wires
- [ ] Status LEDs functioning
- [ ] Antennas properly mounted
- [ ] MicroSD card present and functional

### Weather Assessment

**Go/No-Go Criteria:**

| Parameter | Go | No-Go |
|-----------|----|----- |
| Wind Speed | <10 m/s (22 mph) | >10 m/s |
| Wind Gusts | <15 m/s (33 mph) | >15 m/s |
| Visibility | >3 km | <3 km |
| Precipitation | None | Any |
| Temperature | 0-40°C (32-104°F) | Outside range |
| Lightning | None within 10km | Any risk |

**Weather Resources:**
- Local weather service
- Aviation weather (METAR/TAF)
- Weather apps with wind information
- On-site wind meter measurement

## Operational Safety

### Flight Operations

**Before Takeoff:**
1. Clear takeoff area (10m radius minimum)
2. Verify no people in flight path
3. Check for aircraft overhead
4. Confirm GPS lock (8+ satellites)
5. Test controls and response
6. Announce "TAKING OFF" loudly

**During Flight:**
- Maintain visual line of sight at all times
- Keep UAV within 500m horizontal distance
- Never fly over people or moving vehicles
- Monitor battery level continuously
- Watch for other aircraft
- Be ready to abort mission if needed

**Water Sampling:**
- Maintain 0.5m minimum altitude over water
- Be aware of waves and surface conditions
- Do not attempt water landing
- Keep pipe deployed time minimal
- Monitor position hold accuracy

**Landing:**
- Clear landing area
- Announce "LANDING"
- Gradual descent
- Be ready for manual control if needed
- Disarm immediately after touchdown

### Battery Monitoring

**Critical Voltage Thresholds:**
- **Green (>50%)**: Normal operation
- **Yellow (30-50%)**: Begin return to launch
- **Orange (25-30%)**: Return immediately
- **Red (<25%)**: Emergency landing

**Actions for Low Battery:**
1. Abort current operation immediately
2. Select nearest safe landing zone
3. Enable RTL (Return to Launch) if able
4. Prepare for emergency landing
5. Land as soon as possible

### Communication

**LoRa Link Monitoring:**
- Check RSSI (signal strength) regularly
- Link quality should be >-100 dBm
- If link degrades: reduce distance or abort
- Have backup communication ready

**Lost Link Procedure:**
- System automatically initiates RTL
- Monitor UAV position visually
- Attempt to re-establish link
- Be ready for manual override

## Emergency Procedures

### Emergency Stop

**When to Use:**
- Propeller strike imminent
- Loss of control
- Equipment fire
- Danger to people

**Procedure:**
1. Press emergency stop button immediately
2. Motors will stop instantly
3. UAV will fall - clear area below
4. Do not attempt to catch falling UAV
5. Assess damage before attempting restart

### Lost GPS

**Symptoms:**
- GPS LED blinking
- Satellite count drops to 0
- "GPS LOST" warning

**Actions:**
1. Switch to manual control mode immediately
2. Maintain current altitude and position
3. Navigate back to visible landmarks
4. Land in clear area as soon as possible
5. Do not attempt GPS-based navigation

### Motor Failure

**Symptoms:**
- Unusual sounds or vibration
- Loss of altitude
- Spinning or yawing

**Actions:**
1. Reduce throttle immediately if stable
2. Select nearest landing zone
3. Prepare for hard landing
4. Initiate controlled descent if possible
5. Clear area of people

### Fire

**Symptoms:**
- Smoke from electronics
- Burning smell
- Battery swelling rapidly

**Actions:**
1. Land immediately in clear area
2. Disarm motors
3. Disconnect battery if safe to do so
4. Move away from UAV (5+ meters)
5. Call emergency services if needed
6. Do not attempt to extinguish LiPo fire with water
7. Use ABC fire extinguisher or sand

### Fly-away

**Definition:** UAV flies away uncontrolled

**Prevention:**
- Always set proper failsafes
- Verify GPS home point
- Test RTL before mission
- Ensure good radio link

**If Occurs:**
1. Attempt to regain control via radio
2. Try switching flight modes
3. Use kill switch if losing sight
4. Note last known direction and position
5. Report to authorities if required
6. Retrieve using GPS tracker

## Environmental Safety

### Water Body Safety

**Before Sampling:**
- Assess water current and flow
- Check for submerged obstacles
- Verify depth at sampling location
- Note any aquatic life or nesting areas
- Check for boat traffic

**During Operations:**
- Maintain safe altitude over water
- Watch for changing conditions
- Be aware of reflection disorientation
- Monitor wind patterns over water
- Keep pipe extension minimal

### Wildlife Protection

- Do not disturb nesting birds or animals
- Avoid sensitive habitats
- Maintain distance from wildlife
- Do not sample in restricted areas
- Follow seasonal restrictions

### Environmental Contamination

**Prevent Contamination:**
- Clean all equipment before use
- Rinse with distilled water between samples
- Use sterile bottles when required
- Avoid cross-contamination
- Dispose of waste water properly

## Personal Protective Equipment (PPE)

### Required PPE

**Always Wear:**
- Safety glasses or goggles
- Closed-toe shoes
- Long pants
- High-visibility vest (if near roads)

**Recommended:**
- Gloves (when handling samples)
- Sun protection (hat, sunscreen)
- Hearing protection (near operating motors)
- First aid kit nearby

### Team Safety

**Minimum Team Size:** 2 people
- Pilot (remote pilot in command)
- Visual observer/assistant

**Team Responsibilities:**
- Pilot: Focus on flight operations
- Observer: Watch for hazards, people, aircraft
- Clear communication between team members
- Defined hand signals if needed

## Accident Reporting

### When to Report

**Mandatory Reporting (FAA):**
- Serious injury to any person
- Loss of consciousness
- Property damage >$500
- Collision with manned aircraft

**Internal Reporting:**
- Any incident or near-miss
- Equipment failure
- Safety hazard identified
- Procedure violation

### Information to Document

- Date, time, and location
- Weather conditions
- Description of incident
- Actions taken
- Witness information
- Photos of damage
- Lessons learned

## Training Requirements

### Initial Training

- Complete Part 107 or equivalent course
- System assembly and setup
- Pre-flight procedures
- Normal operations
- Emergency procedures
- Hands-on practice (minimum 10 hours)

### Recurrent Training

- Annual refresher course
- Review of new procedures
- Practice emergency scenarios
- Updates on regulations
- Review of incident reports

### Currency Requirements

- Minimum 1 flight every 30 days
- Practice emergency procedures quarterly
- System proficiency check annually

## Medical Fitness

- No known medical conditions affecting judgment
- Adequate vision (corrective lenses OK)
- No impairment from medication, alcohol, or drugs
- Well-rested (minimum 8 hours sleep)
- Fit for outdoor activities

---

## Emergency Contact Information

**Before Each Flight, Know:**
- Local emergency services: _______________
- Nearest hospital: _______________
- Site contact/property owner: _______________
- Insurance company: _______________
- Aviation authority: _______________

## Safety Checklist

Print and use before every flight:

```
□ Reviewed weather forecast
□ Airspace checked (NOTAMs, TFRs)
□ Equipment inspected
□ Batteries checked
□ GPS functional
□ Failsafes set correctly
□ Landing area clear
□ Team briefed
□ Emergency procedures reviewed
□ First aid kit available
□ Fire extinguisher accessible
□ Communication established
□ All personnel wearing PPE
□ "Go" decision confirmed by all
```

---

**Remember: Safety is not optional. When in doubt, don't fly out!**

**Version:** 1.0.0  
**Date:** 2024-01-15  
**Review:** Annual or after any incident
