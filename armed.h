#ifndef ARMED_H
#define ARMED_H

// NOT FOR HERE
const int buzzer = 9; // PIN

bool bomb_is_armed = false;
const unsigned short beep_length = 125; // miliseconds
const float init_bomb_timer = 45000; // miliseconds
float time_left_on_bomb = init_bomb_timer; // miliseconds
unsigned long time_of_bomb_activation = 0;

// Calculates when the next beep should happen. Returns it in miliseconds.
int findNextBeepInterval();

// Turn on LED & Buzzer for beep_length
void beep();

#endif