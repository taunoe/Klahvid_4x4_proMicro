/**
 *  Project: Klahvid_4x4_proMicro
 *  Started: 26.04.2021
 *  Edited:  08.04.2023
 * 
 *  Copyright 2023 Tauno Erik
 * 
 *  Links:
 *  - https://learn.sparkfun.com/tutorials/cherry-mx-switch-breakout-hookup-guide
 **/

#include <Arduino.h>
#include "Keypad.h"
#include "TVout.h"
#include "fontALL.h"
#define DEBUG
#include "utils_debug.h"

// TV Pins:
// 9 - sync
// 7 - video
// Gnd
// 11 audio

const uint RANDOM_SEED_PIN = 0; // A0

// Keypad pins
// Markings on board
const uint8_t C1_PIN =  2;
const uint8_t C2_PIN =  3;
const uint8_t C3_PIN =  4;
const uint8_t C4_PIN =  5;
const uint8_t R1_PIN = 13;
const uint8_t R2_PIN =  8;
const uint8_t R3_PIN = 17;  // A3
const uint8_t R4_PIN =  6;
// Shift register pins
const uint SER_PIN   = 10;    // SER - serial data input
const uint SRCLK_PIN = 11;  // SRCLK - shift register clock input
const uint RCLK_PIN  = 12;   // RCLK - storage register clock input

// Keypad
const uint ROWS_NUM = 4;
const uint COLS_NUM = 4;

// Keymap
const char  C1 = '4';
const char  C2 = '3';
const char  C3 = '2';
const char  C4 = '1';
const char  C5 = '8';
const char  C6 = '7';
const char  C7 = '6';
const char  C8 = '5';
const char  C9 = 'b';
const char C10 = 'a';
const char C11 = '0';
const char C12 = '9';
const char C13 = 'f';
const char C14 = 'e';
const char C15 = 'd';
const char C16 = 'c';

char keys[ROWS_NUM][COLS_NUM] = {
  {C1, C5,  C9, C13},
  {C2, C6, C10, C14},
  {C3, C7, C11, C15},
  {C4, C8, C12, C16}
};



// Row and column pins
byte ROW_PINS[ROWS_NUM] = {C1_PIN, C2_PIN, C3_PIN, C4_PIN};
byte COL_PINS[COLS_NUM] = {R1_PIN, R2_PIN, R3_PIN, R4_PIN};

uint old_random_num;
uint new_random_num;

bool is_first_keypress = true;
const char CURSOR_LETTER = '.';
const uint CURSOR_BLICK_SPEED = 150; 
uint8_t blick_cursor_x = 0;
uint8_t blick_cursor_y = 0;

unsigned long time_prev = 0;
bool is_cursor_blink_time = false;
bool is_cursor_on = false;

// Keypad object
Keypad keypad = Keypad(makeKeymap(keys), ROW_PINS, COL_PINS, ROWS_NUM, COLS_NUM);

TVout TV;

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

// Print all chars
void tv_test_screen() {
  TV.println("Hi, mum!");
  char c = 33;
  for (int i = 0; i < 93; i++){
    TV.print(c);
    TV.delay(100);
    c++;
  }
}

// Print welcome screen
void tv_welcome_screen() {
  TV.println(28, 40, "Hi, MUM!");
  TV.println(28, 50, "6.4.2023");
}

// Store current cursor position
void update_cursor() {
  blick_cursor_x = TV.get_cursor_x();
  blick_cursor_y = TV.get_cursor_y();
  DEBUG_PRINT("x=");
  DEBUG_PRINT(TV.get_cursor_x());
  DEBUG_PRINT(" y=");
  DEBUG_PRINTLN(TV.get_cursor_y());
}

// 
void blick_cursor() {
  if (is_cursor_blink_time && !is_first_keypress) {

    is_cursor_blink_time = false;
    char c;

    if (is_cursor_on) {
      c = 32;  // space
      is_cursor_on = false;
    } else {
      c = CURSOR_LETTER;
      is_cursor_on = true;
    }

    TV.print(blick_cursor_x, blick_cursor_y, c);
  }
}

// Lightup new random key
// Its like a primitive game
void random_key_led_on() {
  uint register_a = 0;
  uint register_b = 0;

  do {
    new_random_num = random(0, 15);
  } while (new_random_num == old_random_num);

  DEBUG_PRINT("Rand:");
  DEBUG_PRINTLN(new_random_num);
    
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


// Current key LED on
void key_led_on(char key) {
  uint16_t led;

  // LED == char
//0b0000000000000001; // c1 0x1
//0b0000000000000010; // c2 0x2
//0b0000000000000100; // c3 0x4
//0b0000000000001000; // c4 0x8
//0b0000000000010000; // c5 0x10
//0b0000000000100000; // c6 0x20
//0b0000000001000000; // c7 0x40
//0b0000000010000000; // c8 0x80
//0b0000000100000000; // c9 0x100
//0b0000001000000000; // c10 0x200
//0b0000010000000000; // c11 0x400
//0b0000100000000000; // c12 0x800
//0b0001000000000000; // c13 0x1000
//0b0010000000000000; // c14 0x2000
//0b0100000000000000; // c15 0x4000
//0b1000000000000000; // c16 0x8000

  switch (key) {
    case  C1: led =    0x1; break;
    case  C2: led =    0x2; break;
    case  C3: led =    0x4; break;
    case  C4: led =    0x8; break;
    case  C5: led =   0x10; break;
    case  C6: led =   0x20; break;
    case  C7: led =   0x40; break;
    case  C8: led =   0x80; break;
    case  C9: led =  0x100; break;
    case C10: led =  0x200; break;
    case C11: led =  0x400; break;
    case C12: led =  0x800; break;
    case C13: led = 0x1000; break;
    case C14: led = 0x2000; break;
    case C15: led = 0x4000; break;
    case C16: led = 0x8000; break;
  }

  uint8_t first_8_bits = (led >> 8) & 0xFF;
  uint8_t last_8_bits = led & 0xFF;
  
  DEBUG_PRINT("first_8_bits ");
  DEBUG_PRINT_BIN(first_8_bits);
  DEBUG_PRINT("\nlast_8_bits ");
  DEBUG_PRINT_BIN(last_8_bits);

  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, first_8_bits);
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, last_8_bits);
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

  TV.begin(NTSC, 120, 96);
  TV.select_font(font6x8);

  //tv_test_screen();
  //TV.delay(2000);
  //TV.clear_screen();
}

void loop() {
  unsigned long time_now = millis();

  if (is_first_keypress) {
    tv_welcome_screen();
  }

  // Read the keypad
  char key = keypad.getKey(); 

  if (key) {
    if (is_first_keypress) {
      TV.clear_screen();
      is_first_keypress = false;
    }

    DEBUG_PRINT("Key:");
    DEBUG_PRINTLN(key);

    TV.print(blick_cursor_x, blick_cursor_y, key);

    update_cursor();

    //random_key_led_on();
    key_led_on(key);

  }

  // Blinking cursor
  if ((time_now - time_prev) >= CURSOR_BLICK_SPEED) {
    is_cursor_blink_time = true;
    time_prev = time_now;
  }

  blick_cursor();
  
}
