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
// we can do a switch state in the code or an array here
// copy/paste/modify test code here -- test codes are most up to date
// make sure we use the functions that are defined in the Winklebot library

void STARTING_STATE {
  // initialize pins
  // set up tape sensors
  // set up beacon sensors
  // set up motors
}

void ORIENTING {
  // turn on beacon sensors to search for 850 signal
  // rotate CW (*add a branch to rotate CCW instead? how will we deal with different orientations we could be placed in at the start? what do we do if we rotate into a wall?)
  //if (850 Hz beacon sensed), stop rotation, switch to offsetting state
  //offsetting state:
  //rotate 5 degrees CCW (*make sure  this eventually takes into account different starting orientations we could be placed in)
  //switch to driving from starting state
}

void DRIVING_FROM_STARTING_STATE {
  // drive forward, full speed
  //if (tape sensed), switch to tape realign state
  // if (bumped) (*is this possible? -- it is if we drive in the gap between where the long line of server-tape ends and the exchanges. Or if we drive into the side of the Server, instead of the front. Not sure if either of these cases is possible with our sensor precision)
}
        
void TAPE_REALIGNING {
  // rotate CW
  // if (beacon sensed), stop rotation, switch to driving towards server state
}

void DRIVING_TOWARDS_SERVER {
  // drive forward, full speed
  // if (front bumper activated) - include the rotation if only the front left, or only the front right bumper is hit, so the bot pivots until both fronts are active), stop, switch to coin collecting state
}

void COIN_COLLECTING {
  // activate button pusher to push the algorithm - based on other teams, we were discussing ditching the button pusher and just driving backwards/forwards into the button
  // count pushes
  // check counter
  // if (counter = X), switch to search for exchange state
  // else (activate button pusher N+1), counter = counter + 1
}

void SEARCHING_FOR_EXCHANGE {
  // move backwards 3 inches
  // rotate CW
  // if (3 Hz beacon sensed), stop rotation, switch to driving towards exchange state
}

void DRIVING_TOWARDS_EXCHANGE {
  // drive forward, full speed
  // if (tape sensed) (*what if this is the server tape?),  - another counter? “tape_line_counter”, then after bumpers are hit, assess the # of counts and drop coins accordingly
  // if (front bumper activated), stop, switch to coin exchanging state
}

void COIN_EXCHANGING {
  // use bumpers to align
  // lift coin collector door
  // (activate servo to push ramp up)
  // both bummpers have to be activated for one second before the ramp tilts

}


