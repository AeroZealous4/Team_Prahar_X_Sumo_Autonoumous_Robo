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
#define PROXY_MID   48   
#define PROXY_LEFT  42   
#define PROXY_RIGHT 32   

// --- RGB I2C Multiplexer Sensor Mapping ---
#define COUNT         2      
#define MUX_ADDR      0x70   
#define INDEX_FRONT   0      // Front Ground Edge Sensor
#define INDEX_BACK    1      // Rear Ground Edge Sensor

#endif

