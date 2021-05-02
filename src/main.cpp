/**
 *  Project: Klahvid_4x4_proMicro
 *  Started: 26.04.2021
 *  Edited:  02.05.2021
 * 
 *  Copyright 2021 Tauno Erik
 * 
 *  Links:
 *  - https://learn.sparkfun.com/tutorials/cherry-mx-switch-breakout-hookup-guide
 **/

#include <Arduino.h>

/* Settings */
const uint8_t NUM_BTN_ROWS = 4;
const uint8_t NUM_BTN_COLS = 4;

// The number of button bounces before trigger a press or release
const uint8_t MAX_DEBOUNCE = 5;

// Switch rows
static const uint8_t ROW_PINS[NUM_BTN_ROWS] = {9, 8, 7, 6};
// Switch columns
static const uint8_t COL_PINS[NUM_BTN_COLS] = {10, 16, 14, 15};

// One debounce counter per switch
static int8_t debounce_count[NUM_BTN_COLS][NUM_BTN_ROWS];

/**********************************************************/

static void scan_row() {
  static uint8_t currentRow = 0;

  // Write current row LOW.
  digitalWrite(ROW_PINS[currentRow], LOW);

  // Scan through switches on this row:
  for (uint8_t j = 0; j < NUM_BTN_COLS; j++) {
    // Read the button. pressed = LOW.
    if (digitalRead(COL_PINS[j]) == LOW) {
      // Increment a debounce counter
      if (debounce_count[currentRow][j] < MAX_DEBOUNCE) {
        debounce_count[currentRow][j]++;
        // If debounce counter hits MAX_DEBOUNCE
        if ( debounce_count[currentRow][j] == MAX_DEBOUNCE ) {
          // Trigger key press
          Serial.print("Key pressed ");
          Serial.println((currentRow * NUM_BTN_COLS) + j);
        }
      }
    } else {
    // Otherwise, button is released
      if (debounce_count[currentRow][j] > 0) {
        debounce_count[currentRow][j]--;
        if (debounce_count[currentRow][j] == 0) {
          // If debounce counter hits 0
          // Trigger key release
          Serial.print("Key released ");
          Serial.println((currentRow * NUM_BTN_COLS) + j);
        }
      }
    }
  }

  // Once done scanning, de-select the switch by writing them HIGH.
  digitalWrite(ROW_PINS[currentRow], HIGH);


  // Increment currentRow, so next time we scan the next row
  currentRow++;
  if (currentRow >= NUM_BTN_ROWS) {
    currentRow = 0;
  }
}


static void setupSwitchPins() {
  // Button drive rows - written LOW when active, HIGH otherwise
  for (uint8_t i = 0; i < NUM_BTN_ROWS; i++) {
    pinMode(ROW_PINS[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(ROW_PINS[i], HIGH);
  }

  // Buttn select columns. Pulled high through resistor. Will be LOW when active
  for (uint8_t i = 0; i < NUM_BTN_COLS; i++) {
    pinMode(COL_PINS[i], INPUT_PULLUP);
  }
}

/**********************************************************/

void setup() {
  Serial.begin(9600);

  setupSwitchPins();

  // Initialize the debounce counter array
  for (uint8_t i = 0; i < NUM_BTN_COLS; i++) {
    for (uint8_t j = 0; j < NUM_BTN_ROWS; j++) {
      debounce_count[i][j] = 0;
    }
  }
}

void loop() {
  scan_row();
}
