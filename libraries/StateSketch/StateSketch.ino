// do we want to make a separate library for the states, or integrate
// it into Winklebot?
// this sketch is just the raw code we'll have to copy into the Winklebot.h
// or Winklebot.cpp fles, or make new .h and .cpp files

//ACTUAL CODE WE RUN

#include "StateLib.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Starting BitBot State Machine");
  StateLib_setup();
}

void loop() {
  StateLib_work();
}

//in the .h file

//StateLib.h  - written for the state machine

#ifndef StateLib_h
#define StateLib_h

void StateLib_setup();
void StateLib_work();

#endif

//in the .cpp file

#include "Arduino.h"
#include "StateLib.h"
#include "Timers.h"
#include "Winklebot.h"

// delineate all our states
//DECLARING OUR STATES
typedef enum {
	STARTING_STATE
        ORIENTING
        DRIVING_FROM_STARTING_STATE
        TAPE_REALIGNING
        DRIVING_TOWARDS_SERVER
        COIN_COLLECTING
        SEARCHING_FOR_EXCHANGE
        DRIVING_TOWARDS_EXCHANGE
        COIN_EXCHANGING 
  } STATES;

//VARIABLES
static unsigned char current_state = STARTING_STATE

// WHAT HAPPENS WHEN WE MOVE FROM ONE STATE TO ANOTHER? NEED TO CLEAR TIMERS,
// CLEAR ANY COUNTERS, ETC.

void in_between_states (unsigned char new_state) {
  TMRArd_StopTimer(MAIN_TIMER);
  TMRArd_ClearTimerExpired(MAIN_TIMER);
  current_state = new_state;
}

//WHAT HAPPENS IN EACH STATE?
//(want to declare an if(entered_state) statement for each of these?

void STARTING_STATE {
  // initialize pins
  // set up tape sensors
  // set up beacon sensors
  // set up motors
}


