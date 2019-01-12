//thanks to http://educ8s.tv/arduino-fingerprint-sensor-module-tutorial/ for the base code to work from

//imports
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>

//serial setup
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = -1;

//servo setup
Servo servo1;
int pos = 0;

void setup(void) {
  //starting the fingerprint sensor and assigning the servo to pin 9
  startFingerprintSensor();
  servo1.attach(9);
}

//main loop of program
void loop() {
  //intial check of whether a finger is placed or not - if not keep the servo off of the safety button
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER) {
    Serial.println("SAFETYON");
    servo1.write(126);
    delay(1000);
  }
  else
  {
      //if a fingerprint is detected - run an ID on it to ensure it is mine
      fingerprintID = getFingerprintID();
      delay(50);
      //if so swing the servo over to the safety button, allowing the gun to fire
      if(fingerprintID == 1)
      {
        Serial.println("SAFETYOFF");
        servo1.write(145);
        delay(1000);
      }
  }
  Serial.print(fingerprintID);

}

void startFingerprintSensor()
{
  Serial.begin(9600);
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor");
  }
  Serial.println("Waiting for valid finger...");
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

