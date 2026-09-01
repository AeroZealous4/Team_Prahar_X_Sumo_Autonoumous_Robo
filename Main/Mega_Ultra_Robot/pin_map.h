#ifndef pin_map_h
#define pin_map_h

//Servo Motor Pin
#define SERVO_PIN 45
#define SERVO_DELAY 250

//RGB Sensor
#define RGB_I2C_L1 1
#define RGB_I2C_L2 3
#define RGB_I2C_R1 0
#define RGB_I2C_R2 2

// RGB from Nano
#define RGB_INTR 18
#define RGB_D0 15
#define RGB_D1 16
#define RGB_D2 17

//LED Status PIN
const uint8_t LED_PIN = 13;

// Motor pins
/*
const uint8_t ENA_L1 = 7;//4;
const uint8_t IN1_L1 = 6;//49;
const uint8_t IN2_L1 = 5;//48;

const uint8_t ENA_L2 = 13;//5;
const uint8_t IN1_L2 = 12;//51;
const uint8_t IN2_L2 = 11;//50;

const uint8_t ENA_R1 = 2;//2;
const uint8_t IN1_R1 = 4;//45;
const uint8_t IN2_R1 = 3;//44;

const uint8_t ENA_R2 = 8;//3;
const uint8_t IN1_R2 = 9;//47;
const uint8_t IN2_R2 = 10;//46;
*/
const uint8_t ENA_L1 = 9;//4;
const uint8_t IN1_L1 = 2;//49;
const uint8_t IN2_L1 = 3;//48;

const uint8_t ENA_L2 = 10;//5;
const uint8_t IN1_L2 = 4;//51;
const uint8_t IN2_L2 = 5;//50;

const uint8_t ENA_R1 = 11;//2;
const uint8_t IN1_R1 = 7;//45;
const uint8_t IN2_R1 = 8;//44;

const uint8_t ENA_R2 = 6;//3;
const uint8_t IN1_R2 = 12;//47;
const uint8_t IN2_R2 = 13;//46;


//Ultra sonic pins
#define ultra_l 42
#define ultra_r 46
#define ultra_f 48
#define ultra_b 44

byte triggerPin = 52;
byte rotate_dir;
byte fast_rotate = 1;

#define ultra_idx_f 0
#define ultra_idx_b 1
#define ultra_idx_l 2
#define ultra_idx_r 3

byte echoCount = 4;
byte* echoPins = new byte[echoCount]{  ultra_f, ultra_b, ultra_l, ultra_r };//  ultra_l, ultra_r, ultra_f, ultra_b{ 9,10, 11 };//, 

unsigned long previousMillis = 0;
unsigned long currentMillis  = 0;
int servo_val = 37;

#endif
