#ifndef HEADER_H
#define HEADER_H

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

enum BombStates {
  WaitingForInput,
  Armed,
  Disarmed,
  Exploded,
  SettingsMenu,
  ChangePassword,
  ChangeTimer
};

enum BombStates BombState = WaitingForInput;

char keys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[4] = {4, 5, 6, 7};
byte pin_columns[3] = {8, 9, 10};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_columns, 4, 3);

const int buzzer = 12;                   // PIN
const int led = 13;                      // PIN
const unsigned short beep_length = 125;  // miliseconds
float init_timer = 45000;          // float because of the formula in findNextBeep()
unsigned short time_left = init_timer;
unsigned long time_of_bomb_activation, next_beep = 1000, last_beep = 0;

String  password = "1234";
String  input_password = "";
String  stars = "";

// Uses a formula to return the exact time of when next beep should happen.
unsigned long findNextBeep(unsigned long current_time);

// Responsible for buzzer and LED activation.
void beep(unsigned long current_time);

void clearPassword();
void enterPassword();
void wrongPassword();
void inputPassword(char key);

void restartBomb(char key);

void setStars();
void printStars(); 

#endif