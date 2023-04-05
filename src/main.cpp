/**
 *  Project: Klahvid_4x4_proMicro
 *  Started: 26.04.2021
 *  Edited:  08.05.2021
 * 
 *  Copyright 2021 Tauno Erik
 * 
 *  Links:
 *  - https://learn.sparkfun.com/tutorials/cherry-mx-switch-breakout-hookup-guide
 **/

#include <Arduino.h>
#include "Keypad.h"

const int SER_PIN = 10;    // Pin for the SER (serial data input) of the shift register
const int SRCLK_PIN = 11;  // Pin for the SRCLK (shift register clock input) of the shift register
const int RCLK_PIN = 12;   // Pin for the RCLK (storage register clock input) of the shift register


const int ROWS = 4; // Number of rows on the keypad
const int COLS = 4; // Number of columns on the keypad

// Define the keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Define the row and column pins on the Arduino
byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };

// Create the keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600); // Start serial communication

  pinMode(SER_PIN, OUTPUT);
  pinMode(SRCLK_PIN, OUTPUT);
  pinMode(RCLK_PIN, OUTPUT);
}

void loop() {
  char key = keypad.getKey(); // Read the keypad

  if (key != NO_KEY) { // If a key is pressed
    Serial.println(key); // Print the key to the serial monitor
  }

  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, 0b11111111); // Shift out a byte to the shift register
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, 0b11111111);
  digitalWrite(RCLK_PIN, HIGH); // Latch the output to the shift register
  digitalWrite(RCLK_PIN, LOW);
  delay(1000);
}
