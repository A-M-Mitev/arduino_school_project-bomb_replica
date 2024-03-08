#include "armed.h"

int findNextBeepInterval() {
    float diff = time_left_on_bomb / init_bomb_timer;
    Serial.print("Time left on bomb: ");
    Serial.print(time_left_on_bomb);
    Serial.print("/ init bomb timer:  ");
    Serial.print(init_bomb_timer);
    Serial.print(", result=");
    Serial.print(diff);
//    Serial.print("\n");
  return (0.1 + 0.9 * diff) * 1000;
}
void beep() {
  tone(buzzer, 2200, beep_length);
  // turn on LED code...
}