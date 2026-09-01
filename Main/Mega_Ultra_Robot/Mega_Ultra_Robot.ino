#include <Servo.h>

#include "HCSR04.h"
#include "FablabL298Driver.h"
#include "bot_strategy.h"
#include "pin_map.h"
#include "robo_driver.h"
// #include "rgb_sense.h"
#include "rgb_from_nano.h"


Servo myservo;  // create servo object to control a servo

void setup () {
  // Serial.begin(9600);
 // HCSR04.begin(triggerPin, echoPins, echoCount);
 // myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object

  //Inititialize RGB from Nano
 // rgb_from_nano_begin();
  // Initialize the motor
  robo_begin();
}
/*

void loop () {
  unsigned long fn_time;
    
  // fn_time = millis();
  // int rgb_det = update_rgb();
  if(read_rgb_intr())
  {
    edge_detected_move(read_rgb_state());
    clear_rgb_intr();
  }
  // fn_time= millis() - fn_time;
  // Serial.print("RGB:");
  // Serial.print(fn_time);

  // fn_time = millis();
  double* distances = HCSR04.measureDistanceCm();
  // fn_time= millis() - fn_time;
  // Serial.print("ultra:");
  // Serial.print(fn_time);

  // Serial.print("FRONT:");
  // Serial.print(distances[0]);
  // Serial.print("                  ,BACK:");
  // Serial.print(distances[1]);
  // Serial.print("                  ,RIGHT:");
  // Serial.print(distances[2]);
  // Serial.print("                  ,LEFT:");
  // Serial.println(distances[3]);
  // delay(1000);

  if(distances[ultra_idx_b] < DEF_DET_RADIUS)
  {
    // Serial.println(" BACK ");
    
    update_const_rot_dir(TURN_RIGHT);
    hard_rotate(99 * read_const_rot_dir());
    rotate_dir = 0;
    delay(10);
  }

  else if(distances[ultra_idx_f] < DET_RADIUS){
    //attack 
    robo_speed_update(CONST_FWD_SPEED, 0);
    // Serial.println(" FRONT ");

  }
  
  // CHANGE ROTATION BASED ON SIDE SENSORS DETECTION OF ROBOT
  else if( (distances[ultra_idx_l] < DET_RADIUS_LR) || distances[ultra_idx_r] < DET_RADIUS_LR ){
    
    int lt, rt;

    lt = distances[ultra_idx_l];
    rt = distances[ultra_idx_r];
    
    if (min(lt, rt) == lt)
      rotate_dir = 1;
    else
      rotate_dir = 0;

    // Serial.print(" ROTATE: ");
    // Serial.println(rotate_dir);
    if (rotate_dir){
      update_const_rot_dir(TURN_LEFT); 
      hard_rotate( (CONST_ROT_SPEED_FAST)* read_const_rot_dir());
      delay(100);
    }
    else{
      update_const_rot_dir(TURN_RIGHT); 
      hard_rotate( (CONST_ROT_SPEED_FAST)* read_const_rot_dir());
      delay(100);
    }
  }
  
  else{   
    // SEARCH ROBOT
    // Serial.print(" SEARCH: ");
    // Serial.println(rotate_dir);

    // update_const_rot_dir(TURN_LEFT); 
    // hard_rotate( (CONST_ROT_SPEED)* read_const_rot_dir());
  
    if (rotate_dir){
      update_const_rot_dir(TURN_LEFT); 
      hard_rotate( (CONST_ROT_SPEED)* read_const_rot_dir());
    }
    else{
      update_const_rot_dir(TURN_RIGHT); 
      hard_rotate( (CONST_ROT_SPEED)* read_const_rot_dir());
    }
  }

  // delay(1000);
  // Serial.println("-------------\n");
  // SERVO WEAPON CODE 
  currentMillis = millis();
  if (currentMillis - previousMillis >= SERVO_DELAY) {
    previousMillis = currentMillis;
    if (servo_val == 37) 
      servo_val = 143;
    else 
      servo_val = 37;


    myservo.write(servo_val);
  }
}

*/

//         TEST CASE
 void loop(){
  robo_speed_update(CONST_FWD_SPEED, 0); 
 }
