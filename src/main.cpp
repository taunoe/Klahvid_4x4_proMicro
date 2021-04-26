/**
 *  Project: Klahvid_4x4_proMicro
 *  Started: 26.04.2021
 *  Edited:  26.04.2021
 * 
 *  Copyright 2021 Tauno Erik
 * 
 *  Links:
 *  - https://learn.sparkfun.com/tutorials/cherry-mx-switch-breakout-hookup-guide
 **/

#include <Arduino.h>

#define NUM_BTN_ROWS (4)
#define NUM_BTN_COLS (4)


// Debounce built-in to the code. This sets the number of button
// high or low senses that trigger a press or release
#define MAX_DEBOUNCE (5)

// Pins connected to switch rows (2)
static const uint8_t btnRowPins[NUM_BTN_ROWS] = {9, 8, 7, 6};
// Pins connected to switch columns (1)
static const uint8_t btnColPins[NUM_BTN_COLS] = {10, 16, 14, 15};

// One debounce counter per switch
static int8_t debounce_count[NUM_BTN_COLS][NUM_BTN_ROWS];

/**********************************************************/

static void scan_row() {
  static uint8_t currentRow = 0;

  // Write current row LOW.
  digitalWrite(btnRowPins[currentRow], LOW);

  // Scan through switches on this row:
  for (uint8_t j = 0; j < NUM_BTN_COLS; j++) {
    // Read the button. pressed = LOW.
    if (digitalRead(btnColPins[j]) == LOW) {
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
  digitalWrite(btnRowPins[currentRow], HIGH);


  // Increment currentRow, so next time we scan the next row
  currentRow++;
  if (currentRow >= NUM_BTN_ROWS) {
    currentRow = 0;
  }
}


static void setupSwitchPins() {
  // Button drive rows - written LOW when active, HIGH otherwise
  for (uint8_t i = 0; i < NUM_BTN_ROWS; i++) {
    pinMode(btnRowPins[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(btnRowPins[i], HIGH);
  }

  // Buttn select columns. Pulled high through resistor. Will be LOW when active
  for (uint8_t i = 0; i < NUM_BTN_COLS; i++) {
    pinMode(btnColPins[i], INPUT_PULLUP);
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
