#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "HCSR04.h"
#include "pin_map.h"
#include "robo_driver.h"

int red_thr = 170;
int green_thr = 100;
int blue_thr = 100;

bool Red[COUNT];
bool Yellow[COUNT];

// 1 = Clockwise Sweeping, 0 = Counter-Clockwise Sweeping
uint8_t current_search_direction = 1; 
uint8_t mid_state_if_previously_detected = 0; 

// Velocity profiles modified for the new strategy
const uint8_t SPEED_ATTACK = 40; // Heavy shove power 
const uint8_t SPEED_CRUISE = 40;// Safe grinding speed when front hits red
const uint8_t SPEED_PATROL = 40; // Blind searching sweep speed
const uint8_t SPEED_EVADE  = 40; // Rapid getaway escape speed

//const float bot_speed_per_pwm = (150/7.6)/20; //cm per secs
const int bot_speed_per_pwm = 1; //cm per sec per pwm
const int move_safe_distance = 10; //cm per sec per pwm
const int num_of_max_scans = 3;
//
const unsigned long ROT_45_DEG_MS = 400; // ms to rotate 45 degrees at PATROL speed — tune on hardware 308 ms per 45 degree

//Ultrasound
#define DEF_DET_RADIUS 50

Adafruit_TCS34725 tcs[] = {
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X),
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X)
};

void chooseBus(uint8_t bus) {
    Wire.beginTransmission(MUX_ADDR);
    Wire.write(1 << (bus + 1)); 
    Wire.endTransmission();
}

void initColorSensors() {
    for (int i = 0; i < COUNT; i++) {
        chooseBus(i);
        if (tcs[i].begin()) {
            #if DEBUG_ENABLE
            Serial.print("Found Sensor at Slot: "); Serial.println(i + 1);
            #endif
        }
    }
}

void readColors(byte sensorNum) {
    if(sensorNum==2)
      chooseBus(5);
    else
    chooseBus(sensorNum);
    
    float red, green, blue;
    tcs[sensorNum].setInterrupt(false);  
    tcs[sensorNum].getRGB(&red, &green, &blue);
    tcs[sensorNum].setInterrupt(true);  

    #if DEBUG_ENABLE
    Serial.println(" ");
    Serial.print("RGB Raw: ");
    Serial.print(sensorNum);
    Serial.print(" ");
    Serial.print((int)red);
    Serial.print(" ");
    Serial.print((int)green);
    Serial.print(" ");
    Serial.println((int)blue);
    #endif

    
    Red[sensorNum] = (red > red_thr && green < green_thr ) ? 1 : 0;
    Yellow[sensorNum] = (green > green_thr || red > red_thr) ? 0 : 1;
}

// Rotate until opposite edge sensor sees red OR max_ms elapses — whichever first
bool alignToEdge(uint8_t checkIdx, unsigned long ms) {
    unsigned long t0 = millis();
    while (millis() - t0 < ms) {
        readColors(checkIdx);
        if (Red[checkIdx]) return true;
        delay(5);
    }
    return false;
}
// Back up a set distance; optionally watch edge sensors and halt early if red reappears
void forwardCm(uint8_t speed, int cm, bool sensorCheck) {
    unsigned long ms = (unsigned long)cm * 1000UL / ((unsigned long)speed * bot_speed_per_pwm);
    unsigned long t0 = millis();
    forward(speed);
    while (millis() - t0 < ms) {
        if (sensorCheck) {
            for (int i = 0; i < COUNT; i++) readColors(i);
            if (Red[INDEX_LEFT] || Red[INDEX_RIGHT])
            {     
              backwardCm(speed, move_safe_distance, false);       
              break;
            }
        }
        delay(5);
    }
    stop_bot();
}
// Back up a set distance; optionally watch edge sensors and halt early if red reappears
void backwardCm(uint8_t speed, int cm, bool sensorCheck) {
    unsigned long ms = (unsigned long)cm * 1000UL / ((unsigned long)speed * bot_speed_per_pwm);
    unsigned long t0 = millis();
    backward(speed);
    while (millis() - t0 < ms) {
        if (sensorCheck) {
            for (int i = 0; i < COUNT; i++) readColors(i);
            if (Red[INDEX_LEFT] || Red[INDEX_RIGHT])
            {
              forwardCm(speed, move_safe_distance, false);              
              break;
            }
        }
        delay(5);
    }
    stop_bot();
}

// 4 cm blind, then 15 cm with edge sensing live — retreat clear of the boundary
void retreatFromEdge() {
    backwardCm(SPEED_EVADE, move_safe_distance, false);
    backwardCm(SPEED_EVADE, 50, true);
}

void setup() {
    #if DEBUG_ENABLE
    Serial.begin(38250);
    Serial.println("Sumo Core Initializing...");
    #endif
    Wire.begin();
    Wire.setClock(10000);
    Wire.setWireTimeout(3000, true);

    HCSR04.begin(triggerPin, echoPins, echoCount);
      
    pinMode(PROXY_MID, INPUT);
    pinMode(PROXY_LEFT, INPUT);
    pinMode(PROXY_RIGHT, INPUT);

    initMotorDriver();
    initColorSensors();

    forwardCm(SPEED_EVADE
    , 50, true); 
}
void loop() {


    // --- STEP 1: REFRESH SURFACE READINGS ---
    for (int i = 0; i < COUNT; i++) {
        readColors(i);
    }

    // --- STEP 2: HIGHEST PRIORITY OVERRIDE (EDGE RED DETECTED) ---
    // Right-only red → rotate CW until left sees red OR 45° (perpendicular to boundary)
    if (Red[INDEX_RIGHT] && !Red[INDEX_LEFT]) {
        #if DEBUG_ENABLE
        Serial.println("RIGHT RED — aligning perpendicular (CW)...");
        #endif
        rotate_clock(SPEED_PATROL);
        //otate_clock_slow(SPEED_PATROL);
        alignToEdge(INDEX_LEFT, ROT_45_DEG_MS);
        stop_bot();
        retreatFromEdge();
        return;
    }

    // Left-only red → rotate CCW until right sees red OR 45° (perpendicular to boundary)
    if (Red[INDEX_LEFT] && !Red[INDEX_RIGHT]) {
        #if DEBUG_ENABLE
        Serial.println("LEFT RED — aligning perpendicular (CCW)...");
        #endif
        rotate_anticlock(SPEED_PATROL);
        //rotate_anticlock_slow(SPEED_PATROL);
        alignToEdge(INDEX_RIGHT, ROT_45_DEG_MS);
        stop_bot();
        retreatFromEdge();
        return;
    }

    // Both red → we are cornered / fully on boundary, back out immediately
    if (Red[INDEX_LEFT] && Red[INDEX_RIGHT]) {
        #if DEBUG_ENABLE
        Serial.println("BOTH RED — cornered, backing out...");
        #endif
        retreatFromEdge();
        return;
    }

    // --- STEP 3: COMBAT MATRIX & FRONT SENSOR ADAPTIVE SPEED ---
    int val_mid   = digitalRead(PROXY_MID);
    int val_left  = digitalRead(PROXY_LEFT);
    int val_right = !digitalRead(PROXY_RIGHT);

    double* distances = HCSR04.measureDistanceCm();
    #if DEBUG_ENABLE
    Serial.println("");
    Serial.print("Ultra dist: "); Serial.print(distances[0]);
    Serial.println("");
    Serial.print("Sensors -> L: "); Serial.print(val_left);
    Serial.print(" | M: "); Serial.print(val_mid);
    Serial.print(" | R: "); Serial.print(val_right);
    Serial.print(" | LeftRed: "); Serial.println(Red[INDEX_LEFT]);
    #endif

    //if (val_mid == LOW)
    if (val_mid == LOW || distances[0] < DEF_DET_RADIUS) 
    {
      if(mid_state_if_previously_detected == 0)
         {
          //delay(50);
          mid_state_if_previously_detected = 1;
         }
       
        // Target straight ahead!
        if (Red[INDEX_LEFT]) {
            // Left sensor detects red -> Reduce attack speed to Cruise Speed to grind without overshooting
            #if DEBUG_ENABLE
            Serial.print(" [Left Line! Cruising Shove] ");
            #endif
            forward(SPEED_CRUISE);
        } else {
            // Field is clear -> Shove at full attack speed
            #if DEBUG_ENABLE
            Serial.print(" [Clear Target! Full Attack Shove] ");
            #endif
            forward(SPEED_ATTACK);
        }
    } 
    else if (val_left == LOW) {
        // Target slipping left -> Turn towards them
        rotate_anticlock(SPEED_PATROL);
        current_search_direction = 0; 
        mid_state_if_previously_detected = 0;
    }
     else if (val_right == LOW) {
        // Target slipping right -> Turn towards them
        rotate_clock(SPEED_PATROL);
        current_search_direction = 1; 
        mid_state_if_previously_detected = 0;
    }
    else {
        // Empty Ring Strategy -> Active scanning sweeps using tracking memory direction
        if (current_search_direction == 1) {
            rotate_clock(SPEED_PATROL);
        } else {
            rotate_anticlock(SPEED_PATROL);
        }
        mid_state_if_previously_detected = 0;
    }
}
