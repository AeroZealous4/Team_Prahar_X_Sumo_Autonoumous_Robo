#ifndef ROBO_DRIVER_H
#define ROBO_DRIVER_H

#include <Arduino.h>
#include "pin_map.h"

void initMotorDriver() {
    pinMode(PWM_1, OUTPUT);
    pinMode(PWM_2, OUTPUT);
    pinMode(DIR_1, OUTPUT);
    pinMode(DIR_2, OUTPUT);
    
    analogWrite(PWM_1, 0);
    analogWrite(PWM_2, 0);
}

void forward(uint8_t speed) {
    #if DEBUG_ENABLE
    Serial.print(" forward ");
    #endif
    digitalWrite(DIR_1, HIGH);
    digitalWrite(DIR_2, HIGH);
    analogWrite(PWM_1, speed);
    analogWrite(PWM_2, speed);
}

void backward(uint8_t speed) {
    #if DEBUG_ENABLE
    Serial.print(" backward ");
    #endif
    digitalWrite(DIR_1, LOW);
    digitalWrite(DIR_2, LOW);
    analogWrite(PWM_1, speed);
    analogWrite(PWM_2, speed);
}
void rotate_clock(uint8_t speed) {
    #if DEBUG_ENABLE
    Serial.print(" clk ");
    #endif
    digitalWrite(DIR_2, LOW);
    digitalWrite(DIR_1, HIGH);
    analogWrite(PWM_1, speed);
    analogWrite(PWM_2, speed);
}

void rotate_anticlock(uint8_t speed) {
    #if DEBUG_ENABLE
    Serial.print(" anticlk ");
    #endif
    digitalWrite(DIR_2, HIGH);
    digitalWrite(DIR_1, LOW);
    analogWrite(PWM_1, speed);
    analogWrite(PWM_2, speed);
}

void stop_bot() {
    analogWrite(PWM_1, 0);
    analogWrite(PWM_2, 0);
}

#endif
