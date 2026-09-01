#ifndef BOT_STRATGY_H
#define BOT_STRATGY_H

// #include "rgb_sense.h"
#include "robo_driver.h"

#define DET_RADIUS 50     //Radius in which detected object will be consider as bot and will be attacked
#define DET_RADIUS_LR 20     //
#define DEF_DET_RADIUS 10 //

#define CONST_ROT_SPEED 30 
#define CONST_FWD_SPEED 99 
#define CONST_ROT_SPEED_FAST 90

#define EDGE_DET_SPEED_FOR_MOTION 50
#define EDGE_DET_SPEED_FOR_ROT 50

#define TURN_LEFT  1
#define TURN_RIGHT -1

int bot_strategy_curent_rotation_dir = 1; //1: Right and -1 is Left

void edge_detected_move(int);
void update_const_rot_dir(int);
int read_const_rot_dir(void);

void update_const_rot_dir(int rot_dir)
{
  bot_strategy_curent_rotation_dir = rot_dir;
}
int read_const_rot_dir()
{
  return bot_strategy_curent_rotation_dir;
}

void edge_detected_move(int rgb_state)
{
  // int rgb_state;
  // rgb_state =  (digitalRead(RGB_D2) << 2) | (digitalRead(RGB_D1) << 1) | digitalRead(RGB_D0);
  // Serial.println(rgb_state);

  switch (rgb_state){

    case 1: //R2 
      forward_cm(30,EDGE_DET_SPEED_FOR_MOTION); //Forward by 3 cm
      rotate_left_degree(45,EDGE_DET_SPEED_FOR_ROT);             
      update_const_rot_dir(TURN_LEFT); //Update Rotation to left direction
      break;
    
    case 2: //R1 
      backward_cm(30,EDGE_DET_SPEED_FOR_MOTION); //Forward by 3 cm
      rotate_left_degree(90,EDGE_DET_SPEED_FOR_ROT);//Rotate left by 90 degrees
      update_const_rot_dir(TURN_LEFT); //Update Rotation to left direction
      // rotate_right(135); //Rotate left by 45 degrees
      break;

    case 6: //R1 & R2
      rotate_left_soft_degree(90,EDGE_DET_SPEED_FOR_ROT); //Rotate left by 90 degrees
      update_const_rot_dir(TURN_LEFT); //Update Rotation to left direction
      forward_cm(10,EDGE_DET_SPEED_FOR_MOTION); //Forward by 10 cm
      break;

    case 0: //L2
      forward_cm(30,EDGE_DET_SPEED_FOR_MOTION); //Forward by 3 cm
      rotate_right_degree(45,EDGE_DET_SPEED_FOR_ROT); //Rotate right by 45 degrees
      update_const_rot_dir(TURN_RIGHT); //Update Rotation to right direction
      break;

    case 5: //L2 & R2
      forward_cm(30,EDGE_DET_SPEED_FOR_MOTION); //Forward by 3 cm
      // rotate_right(180); //Rotate left by 180 degrees
      break;
    
    case 3: //L1
      backward_cm(30,EDGE_DET_SPEED_FOR_MOTION); //Forward by 3 cm
      rotate_right_degree(90,EDGE_DET_SPEED_FOR_ROT); //Rotate left by 90 degrees
      update_const_rot_dir(TURN_RIGHT); //Update Rotation to right direction
      // rotate_right(135); //Rotate left by 45 degrees
      break;
    
    case 7: //L1 & R1
      backward_cm(30,EDGE_DET_SPEED_FOR_MOTION); //Forward by 3 cm
      rotate_right_degree(90,EDGE_DET_SPEED_FOR_ROT); //Rotate left by 90 degrees
      update_const_rot_dir(TURN_RIGHT); //Update Rotation to right direction
      break;
    
    case 4: //L1 & L2
      rotate_right_soft_degree(90,EDGE_DET_SPEED_FOR_ROT);
      update_const_rot_dir(TURN_LEFT); //Update Rotation to left direction
      forward_cm(30,EDGE_DET_SPEED_FOR_MOTION); //Forward by 3 cm
      break;         

    default:
      break;
  }
}
#endif
