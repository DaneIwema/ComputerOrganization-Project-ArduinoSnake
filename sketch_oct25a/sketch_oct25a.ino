#include "LedControl.h"
LedControl lc=LedControl(8, 10, 9, 1);

enum Cell {
  body,
  food,
  empty
};

Cell buffer[8][8];

struct Body {
  int x;
  int y;
  int lr;
  int ud;
};
Body head = {3, 4, -1, 0};
Body tail = {3, 5, -1, 0};

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
    head.lr = -1;
    head.ud = 0;
    tail.lr = -1;
    tail.ud = 0;
  }
  if (analogRead(0) < 450){
    head.lr = 1;
    head.ud = 0;
    tail.lr = 1;
    tail.ud = 0;
  }
  if (analogRead(1) > 550){
    head.lr = 0;
    head.ud = 1;
    tail.lr = 0;
    tail.ud = 1;
  }
  if (analogRead(1) < 450){
    head.lr = 0;
    head.ud = -1;
    tail.lr = 0;
    tail.ud = -1;
  }
  lc.setLed(0, head.x, head.y, true);
  lc.setLed(0, tail.x, tail.y, false);
  moveSnake();
  delay(500);
}

void moveSnake(){
  switch(head.x){
    case 8:
      head.x = 0;
      break;
    case 0:
      head.x = 8;
      break;
    default:
      head.x += head.ud;
  }
  switch(head.y){
    case 8:
      head.y = 0;
      break;
    case 0:
      head.y = 8;
      break;
    default:
      head.y += head.lr;
  }
  switch(tail.x){
    case 8:
      tail.x = 0;
      break;
    case 0:
      tail.x = 8;
      break;
    default:
      tail.x += tail.ud;
  }
  switch(tail.y){
    case 8:
      tail.y = 0;
      break;
    case 0:
      tail.y = 8;
      break;
    default:
      tail.y += tail.lr;
  }
}