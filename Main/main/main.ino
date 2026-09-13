#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "pin_map.h"
#include "robo_driver.h"

int red_thr = 150;
int green_thr = 100;

bool Red[COUNT];
bool Yellow[COUNT];

// 1 = Clockwise Sweeping, 0 = Counter-Clockwise Sweeping
uint8_t current_search_direction = 1; 

// Velocity profiles modified for the new strategy
const uint8_t SPEED_ATTACK = 50; // Heavy shove power 
const uint8_t SPEED_CRUISE = 30; // Safe grinding speed when front hits red
const uint8_t SPEED_PATROL = 30; // Blind searching sweep speed
const uint8_t SPEED_EVADE  = 30; // Rapid getaway escape speed

//const float bot_speed_per_pwm = (150/7.6)/20; //cm per secs
const int bot_speed_per_pwm = 1; //cm per sec per pwm

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
    chooseBus(sensorNum);
    float red, green, blue;
    tcs[sensorNum].setInterrupt(false);  
    tcs[sensorNum].getRGB(&red, &green, &blue);
    tcs[sensorNum].setInterrupt(true);  
  
    Red[sensorNum] = (red > red_thr) ? 1 : 0;
    Yellow[sensorNum] = (green > green_thr || red > red_thr) ? 0 : 1;
}

void setup() {
    #if DEBUG_ENABLE
    Serial.begin(38250);
    Serial.println("Sumo Core Initializing...");
    #endif
    Wire.begin();
    Wire.setClock(10000);
    Wire.setWireTimeout(3000, true);

    pinMode(PROXY_MID, INPUT);
    pinMode(PROXY_LEFT, INPUT);
    pinMode(PROXY_RIGHT, INPUT);

    initMotorDriver();
    initColorSensors();
}

void loop() {

    // --- STEP 1: REFRESH SURFACE READINGS ---
    for (int i = 0; i < COUNT; i++) {
        readColors(i);
    }

    // --- STEP 2: HIGHEST PRIORITY OVERRIDE (BACK RED DETECTED) ---
    // If the back sensor clips red, the opponent is successfully pushed out, or we hit our limit.
    // Retreat backward immediately back into the arena safety zone.
    if (Red[INDEX_BACK]) {
        #if DEBUG_ENABLE
        Serial.println("BACK RED DETECTED! Pulling back into arena...");
        #endif
        backward(SPEED_EVADE);
        delay(15*1000/(((int) SPEED_EVADE)*bot_speed_per_pwm));
        
        //delay(1200); // Drive backwards deep into the ring safely clear of the 6cm line
        
        // Face away from the edge to restart search tracking cleanly
        rotate_clock(SPEED_PATROL);
        delay(200);
        
        current_search_direction = !current_search_direction; // Invert search bias to sweep fresh space
        return;
    }

    // --- STEP 3: COMBAT MATRIX & FRONT SENSOR ADAPTIVE SPEED ---
    int val_mid   = digitalRead(PROXY_MID);
    int val_left  = digitalRead(PROXY_LEFT);
    int val_right = !digitalRead(PROXY_RIGHT);

    #if DEBUG_ENABLE
    Serial.print("Sensors -> L: "); Serial.print(val_left);
    Serial.print(" | M: "); Serial.print(val_mid);
    Serial.print(" | R: "); Serial.print(val_right);
    Serial.print(" | FrontRed: "); Serial.println(Red[INDEX_FRONT]);
    #endif

    if (val_mid == LOW) {
        // Target straight ahead!
        if (Red[INDEX_FRONT]) {
            // Front sensor detects red -> Reduce attack speed to Cruise Speed to grind without overshooting
            #if DEBUG_ENABLE
            Serial.print(" [Front Line! Cruising Shove] ");
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
    else if (val_right == LOW) {
        // Target slipping right -> Turn towards them
        rotate_clock(SPEED_PATROL);
        current_search_direction = 1; 
    } 
    else if (val_left == LOW) {
        // Target slipping left -> Turn towards them
        rotate_anticlock(SPEED_PATROL);
        current_search_direction = 0; 
    } 
    else {
        // Empty Ring Strategy -> Active scanning sweeps using tracking memory direction
        if (current_search_direction == 1) {
            rotate_clock(SPEED_PATROL);
        } else {
            rotate_anticlock(SPEED_PATROL);
        }
    }

//    delay(2);
}
