#include "LedControl.h"
LedControl lc=LedControl(8, 10, 9, 1);

enum Cell {
  body,
  food,
  empty
};

Cell buffer[8][8];

enum Direction {
  LEFT, 
  RIGHT,
  UP,
  DOWN,
};

struct Body {
  int x;
  int y;
  Direction direction;
};
Body head = {3, 4, LEFT};

struct Food {
  int x;
  int y;
};

void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT);
  lc.shutdown(0,false);
  lc.setIntensity(0,5);
  lc.clearDisplay(0);
}

void loop() {
  if (analogRead(0) > 550){
    head.direction = LEFT;
  }
  if (analogRead(0) < 450){
    head.direction = RIGHT;
  }
  if (analogRead(1) > 550){
    head.direction = UP;
  }
  if (analogRead(1) < 450){
    head.direction = DOWN;
  }
  moveSnake();
  delay(250);
}

void moveSnake(){
  lc.setLed(0, head.y, head.x, false);
  switch(head.direction){
    case LEFT:
      head.x += 1;
      if (head.x == 8)
        head.x = 0;
      Serial.println(head.x);
      break;
    case RIGHT:
      head.x -= 1;
      if (head.x == -1)
        head.x = 7;
      Serial.println(head.x);
      break;
    case UP:
      head.y += 1;
      if (head.y == 8)
        head.y = 0;
      Serial.println(head.y);
      break;
    case DOWN:
      head.y -= 1;
      if (head.y == -1)
        head.y = 7;
      Serial.println(head.y);
      break;
  }
  lc.setLed(0, head.y, head.x, true);
}