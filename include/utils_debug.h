/*
File: utils_debug.h
Source: https://www.youtube.com/watch?v=7kRlQDxGR9A
Tauno Erik
08.04.2023

Usage:
#define DEBUG
#include "utils_debug.h"

DEBUG_PRINT("Message");
DEBUG_PRINTLN("Message");
DEBUG_PRINT_ALL("Message");
*/

#ifdef DEBUG
  #define DEBUG_PRINT(x) \
  Serial.print(x);

  #define DEBUG_PRINTLN(x) \
  Serial.println(x);

  #define DEBUG_PRINT_BIN(x) \
  Serial.print(x, BIN);

  #define DEBUG_PRINT_ALL(x) \
  Serial.print(millis()); \
  Serial.print(": "); \
  Serial.print(__PRETTY_FUNCTION__); \
  Serial.print(" in "); \
  Serial.print(__FILE__); \
  Serial.print(":"); \
  Serial.print(__LINE__); \
  Serial.print(" "); \
  Serial.println(x);
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT_BIN(x)
  #define DEBUG_PRINT_ALL(x)
#endif