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
  work_through_States();
}

//in the .h file

//StateLib.h  - written for the state machine

#ifndef StateLib_h
#define StateLib_h

void StateLib_setup();
void work_through_States();

#endif

//in the .cpp file

#include "Arduino.h"
#include "StateLib.h"
#include "Timers.h"
#include "Winklebot.h"

#define TIMER_1  // do we use timers ever? how do we define them? this needs to match


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

// DECLARE A TRANSITION STATE
// WHAT HAPPENS WHEN WE MOVE FROM ONE STATE TO ANOTHER? NEED TO CLEAR TIMERS,
// CLEAR ANY COUNTERS, ETC.

void in_between_states (unsigned char new_state) {
  TMRArd_StopTimer(TIMER_1);
  TMRArd_ClearTimerExpired(TIMER_1);
  current_state = new_state;
}

//WHAT HAPPENS IN EACH STATE?
//(want to declare an if(entered_state) statement for each of these?
// we can do a switch state or an array here

void STARTING_STATE {
  // initialize pins
  // set up tape sensors
  // set up beacon sensors
  // set up motors
}

void ORIENTING {
}

void DRIVING_FROM_STARTING_STATE {
}
        
void TAPE_REALIGNING {
}

void DRIVING_TOWARDS_SERVER {
}

void COIN_COLLECTING {
}

void SEARCHING_FOR_EXCHANGE {
}

void DRIVING_TOWARDS_EXCHANGE {
}

void COIN_EXCHANGING {
}


