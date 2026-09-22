#ifndef PIN_MAP_H
#define PIN_MAP_H

// --- Top Level Global Configuration Flags ---
#define DEBUG_ENABLE 1   // Set to 1 to compile Serial prints, 0 for clean competition speed

// --- Motor Driver Channels ---
#define PWM_1 11   
#define PWM_2 12   
#define DIR_1 28   
#define DIR_2 26   

// --- Digital Tracking Proximity Matrix ---
#define PROXY_MID   46   
#define PROXY_LEFT  48   
#define PROXY_RIGHT 42   

// --- RGB I2C Multiplexer Sensor Mapping ---
#define COUNT         2      
#define MUX_ADDR      0x70   
#define INDEX_LEFT    1      // Left Ground Edge Sensor
#define INDEX_RIGHT   0      // Right Ground Edge Sensor
#define INDEX_BACK   2      // Right Ground Edge Sensor

//Ultra sonic pins
#define ultra_f_pin 35
byte triggerPin = 33;


#define ultra_idx_f 0

byte echoCount = 1;
byte* echoPins = new byte[echoCount]{  ultra_f_pin };//  ultra_l, ultra_r, ultra_f, ultra_b{ 9,10, 11 };//, 


#endif
