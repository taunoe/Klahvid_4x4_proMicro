/**
 *  Project: Klahvid_4x4_proMicro
 *  Started: 26.04.2021
 *  Edited:  05.04.2023
 * 
 *  Copyright 2023 Tauno Erik
 * 
 *  Links:
 *  - https://learn.sparkfun.com/tutorials/cherry-mx-switch-breakout-hookup-guide
 **/

#include <Arduino.h>
#include "Keypad.h"

const uint RANDOM_SEED_PIN = 0;

// Shift register pins
const uint SER_PIN = 10;    // SER - serial data input
const uint SRCLK_PIN = 11;  // SRCLK - shift register clock input
const uint RCLK_PIN = 12;   // RCLK - storage register clock input

uint old_random_num;
uint new_random_num;

// Keypad
const uint ROWS_NUM = 4;
const uint COLS_NUM = 4;

// Keymap
char keys[ROWS_NUM][COLS_NUM] = {
  {'4','8','b','f'},
  {'3','7','a','e'},
  {'2','6','0','d'},
  {'1','5','9','c'}
};

// Markings on board
const uint8_t C1_PIN = 2;
const uint8_t C2_PIN = 3;
const uint8_t C3_PIN = 4;
const uint8_t C4_PIN = 5;
const uint8_t R1_PIN = 6;
const uint8_t R2_PIN = 7;
const uint8_t R3_PIN = 8;
const uint8_t R4_PIN = 9;

// Row and column pins
byte ROW_PINS[ROWS_NUM] = {C1_PIN, C2_PIN, C3_PIN, C4_PIN};
byte COL_PINS[COLS_NUM] = {R1_PIN, R2_PIN, R3_PIN, R4_PIN};

// Keypad object
Keypad keypad = Keypad(makeKeymap(keys), ROW_PINS, COL_PINS, ROWS_NUM, COLS_NUM);

// Check if the number is even
bool is_even(uint num) {
  if (num % 2 == 0) {
    //Serial.println(num + " is even.");
    return true;
  } else { 
    //Serial.println(num + " is odd.");
    return false;
  }
}

void all_leds_off() {
  for (uint i = 0; i < 2; i++) {
    shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, 0b000000);
  }
  digitalWrite(RCLK_PIN, HIGH);
  digitalWrite(RCLK_PIN, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(SER_PIN, OUTPUT);
  pinMode(SRCLK_PIN, OUTPUT);
  pinMode(RCLK_PIN, OUTPUT);

  // if analog input pin is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(RANDOM_SEED_PIN));
  all_leds_off();
}

void loop() {
  char key = keypad.getKey(); // Read the keypad

  if (key) {
    Serial.print("Key:");
    Serial.println(key);

    // Lightup new key
    uint register_a = 0;
    uint register_b = 0;

    do {
      new_random_num = random(0, 15);
    } while (new_random_num == old_random_num);

    Serial.print("Rand:");
    Serial.println(new_random_num);
    old_random_num = new_random_num;

    if (new_random_num < 8) {
      register_a = 1<<new_random_num;
    } else {
      register_b = 1<<(new_random_num-8);
    }

    shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, register_a);
    shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, register_b);
    digitalWrite(RCLK_PIN, HIGH);
    digitalWrite(RCLK_PIN, LOW);
  }


/*
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, 0b00000000);
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, 0b00000000);
  digitalWrite(RCLK_PIN, HIGH); // Latch the output to the shift register
  digitalWrite(RCLK_PIN, LOW);
*/
}
