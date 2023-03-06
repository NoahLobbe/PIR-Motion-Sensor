/*
 * Datasheet for PIR Motion sensor:
 * https://cdn-learn.adafruit.com/downloads/pdf/pir-passive-infrared-proximity-motion-sensor.pdf
 */

#define PIR_PIN 7
#define LED_PIN 13 //builtin LED
bool pir_state = false; //to keep track of the sensor's state

void setup() {
  //tell the Arduino what is connected to it
  //eg. output on the builtin LED, and input on PIR_PIN
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  Serial.begin(9600); //connect serial communication with computer
}

void loop() {
  if (digitalRead(PIR_PIN)) { //read the PIR motion sensor's state
    //if motion is detected...
    digitalWrite(LED_PIN, 1);

    if (pir_state == false) { //if motion hasn't been detected
      Serial.println("New Motion detected!");
      pir_state = true;
    }
    
  } else {
    //if motion not detected...
    digitalWrite(LED_PIN, 0);

    if (pir_state) {
      Serial.println("Motion ended");
      pir_state = false;
    }
  }
}
