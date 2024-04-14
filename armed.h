#ifndef ARMED_H
#define ARMED_H

const int buzzer = 9;                    // PIN
const int led = 10;                      // PIN
const unsigned short beep_length = 125;  // miliseconds
const float init_timer = 45000;          // float because of the formula in findNextBeep()
unsigned short time_left = init_timer;
unsigned long time_of_bomb_activation, next_beep = 1000, last_beep = 0;
bool bomb_is_armed = false;

// Uses a formula to return the exact time of when next beep should happen.
unsigned long findNextBeep(unsigned long current_time);

// Responsible for buzzer and LED activation.
void beep(unsigned long current_time);

#endif