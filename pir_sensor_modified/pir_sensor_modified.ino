/*
 * Datasheet for PIR Motion sensor:
 * https://cdn-learn.adafruit.com/downloads/pdf/pir-passive-infrared-proximity-motion-sensor.pdf
 * 
 * See “Sensitivity Adjustment” and “Time-Delay Adjustment at: https://lastminuteengineers.com/pir-sensor-arduino-tutorial/ 
 */

#include "notes.h" //import all the note frequency values from the notes.h file

//pin definitions
#define PIR_PIN 2
#define LED_PIN 13 //builtin LED
#define PIEZO_PIN 3
#define DEBUG_MODE false

//set alarm constants
#define tune_arr_len 4
const int alarm_tune[] = {NOTE_G4, NOTE_A4, NOTE_A4, NOTE_B4};
const int note_times[] = {      2,       2,       2,       2}; // note durations: 4 = quarter note, 8 = eighth note, etc. Actual time (ms) = 1000/noteTime[note_index]

//flags and variables
bool is_motion_detected = false; //to keep track of the sensor's state
bool play_alarm = false;
bool tune_finished = false;
uint16_t note_index = 0;

void setup() {
  //tell the Arduino what is connected to it
  //eg. output on the builtin LED and PIEZO_PIN, and input on PIR_PIN
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  Serial.begin(9600); //connect serial communication with computer
}

//make a block of code to play the alarm notes
void playNote() {
  if (DEBUG_MODE) {
    Serial.println("...playing note...");
  }
  //determine the time (ms) that a note should play for
  int note_duration = 1000 / note_times[note_index];

  //on the piezo pin, play the frequency, for note duration
  tone(PIEZO_PIN, alarm_tune[note_index], note_duration);

  //wait for the note duration plus a little extra...
  delay(note_duration * 1.3);

  //stop the note because its duration has past
  noTone(PIEZO_PIN);  
}

void loop() {  
  ////Get inputs
  if (digitalRead(PIR_PIN)) { //read the PIR motion sensor's state
    //if motion is detected...
    digitalWrite(LED_PIN, 1);

    if (is_motion_detected == false) { //if motion hasn't been detected
      Serial.println("New Motion detected!");
      is_motion_detected = true;
      play_alarm = true; //set the flag to play the alarm
      note_index = 0; //reset
      
    } else {
      if (DEBUG_MODE) {
        Serial.println("Old Motion...");
      }
    }
    
  } else {
    //if motion not detected...
    digitalWrite(LED_PIN, 0);

    if (is_motion_detected) {
      Serial.println("Motion ended");
      is_motion_detected = false;
    }
  }


  ////Do something about the inputs
  if (play_alarm) {
    if (DEBUG_MODE) {
      Serial.println(note_index); 
    }
    playNote();
  }

  ////Check and update variables and flags
  note_index++; //update
  if (note_index > tune_arr_len-1) { //keep note_index within the range of the alarm tune array. (Array indexing starts at 0 going to tune_arr_len-1)
    note_index = 0;
    tune_finished = true; 
  }
  
  if (tune_finished && !is_motion_detected) { //only tell the alarm to stop once the tune has actually finished
    //reset flags and index
    play_alarm = false;
    tune_finished = false;
    note_index = 0;
  }

  
}
