#ifndef HEADER_H
#define HEADER_H

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const int buzzer = 12;                   // PIN
const int led = 13;                      // PIN

LiquidCrystal_I2C lcd(0x27, 20, 4);

enum BombStates {
  WaitingForInput,
  Armed,
  Disarmed,
  Exploded,
  SettingsMenu,
  ChangePassword,
  ChangeTimer,
  ChangeDefuse
};

enum BombStates BombState = WaitingForInput;
bool defusing = 0;
float defusing_time = 10000;
unsigned long defusing_started, time_for_next_random;

char keys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[4] = {4, 5, 6, 7};
byte pin_columns[3] = {8, 9, 10};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_columns, 4, 3);

const int beep_length = 125;  // miliseconds
double init_timer = 40000;
unsigned long time_of_bomb_activation, next_beep;

String password = "7355608";                // default password
String stars = "";
unsigned int password_length = 0;

String input_password = "";
String input_timer = "";
unsigned int input_length = 0;

String exmarks = "";
unsigned int number_of_exmarks = 0;

void checkDefusing(char key);

unsigned long findNextBeep(unsigned long current_time, unsigned long time_left);
void beep(unsigned long current_time, unsigned long time_left);
void finalBeep();

void clearInput();
void inputPassword(char key);
void enterPassword();
void wrongPassword();
void inputTimer(char key);
double convertInputTimer();

void restartBomb(char key);

void updatePasswordLength();
void printStars();
void printTimeLeft(unsigned long time_left);
void printDefuseTime();
// Loading Bar 
void printExMarks(unsigned int new_number_of_exmarks); 

#endif