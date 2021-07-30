#include <MIDIUSB.h>

//#define DEBUG 1 // comment out to start debug section

#ifdef DEBUG
 #define DEBUG_PRINT(x)     Serial.print (x)
 #define DEBUG_PRINTLN()  Serial.println ()
#else
#endif

#define MIDICHANNEL 1
// define the pin numbers
#define LED_PIN 6

#define FSR_PIN1 0     // the FSR 
#define FSR_PIN2 1
#define FSR_PIN3 3
#define FSR_PIN4 4
#define FSR_PIN5 5

#define FABRIC_PIN 2   //fabric

const int THRESHOLD = 10;   //set a threshold

byte fsrReading1;    // top
byte fsrReading2;  //left
byte fsrReading3;  //right
byte fabricReading;
byte fsrReading4;  //front

void setup() {
  Serial.begin(115200);
  digitalWrite(LED_PIN, LOW);
  #ifdef DEBUG
    DEBUG_PRINT("[DEBUG]Start...");
    DEBUG_PRINTLN();
  #endif
}

void loop() {
  fsrReading1 = analogRead(FSR_PIN1);
  fsrReading2 = analogRead(FSR_PIN2);
  fsrReading3 = analogRead(FSR_PIN3);
  fsrReading4 = analogRead(FSR_PIN4);

  fabricReading = analogRead(FABRIC_PIN);

  if (fsrReading1 > THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
    fsrReading1 = fsrReading1 / 2;
    controlChange(MIDICHANNEL, 7, fsrReading1);
    MidiUSB.flush();
  }

  if (fsrReading2 > THRESHOLD && fsrReading3 > THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
    byte sendVal = 0;

    if (fsrReading2 >= fsrReading3) {
      sendVal = fsrReading2;
    } else {
      sendVal = fsrReading3;
    }
    sendVal = sendVal / 2;
    controlChange(MIDICHANNEL, 5, sendVal);
    MidiUSB.flush();
  }

  delay(1000);
  digitalWrite(LED_PIN, LOW);

}

void controlChange(byte channel, byte control, byte value) {
  #ifdef DEBUG
    DEBUG_PRINT("Sending CC--> channel ");
    DEBUG_PRINT(channel);
    DEBUG_PRINT(" , control ");
    DEBUG_PRINT(control);
    DEBUG_PRINT(", value ");
    DEBUG_PRINT(value);
    DEBUG_PRINTLN();
  #endif
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  #ifdef DEBUG
    DEBUG_PRINT("Sending noteOn --> channel ");
    DEBUG_PRINT(channel);
    DEBUG_PRINT(" , pitch ");
    DEBUG_PRINT(pitch);
    DEBUG_PRINT(", velocity ");
    DEBUG_PRINT(velocity);
    DEBUG_PRINTLN();
  #endif
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  #ifdef DEBUG
    DEBUG_PRINT("Sending noteOff --> channel ");
    DEBUG_PRINT(channel);
    DEBUG_PRINT(" , pitch ");
    DEBUG_PRINT(pitch);
    DEBUG_PRINT(", velocity ");
    DEBUG_PRINT(velocity);
    DEBUG_PRINTLN();
  #endif
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
