#include "LedControl.h"
LedControl MatDisp = LedControl(8, 10, 9, 1);

struct Coord {
  int x;
  int y;
};

unsigned long previousMillis = 0;
const unsigned long interval = 500; //in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT);
  MatDisp.shutdown(0,false);
  MatDisp.setIntensity(0,5);
}

void loop() {
  MatDisp.clearDisplay(0);

  //up = 0, down = 1, left = 2, right = 3
  Coord possibleDirections[4] = {{-1, 0}, {+1, 0}, {0, +1}, {0, -1}};
  Coord snakeHead = {3, 3}; //starting head position
  Coord snakeTail = {4, 3}; //starting tail position
  int curDirection = 0; //starts moving up
  int buffer[8][8] = {0};
  buffer[4][3] = curDirection + 1; //set the position the tail will move to in the begining
  newFood(buffer);
  bool gameOver = false; //sets the gameOver flag for the loop
  while (!gameOver) {
    curDirection = getDirection(curDirection); //get the current direction
    //checks elapsed time
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      buffer[snakeHead.x][snakeHead.y] = curDirection + 1; //save move to buffer
      snakeHead = addCoords(snakeHead, possibleDirections[curDirection]); //update snakeHead coordinates
      snakeHead = checkBounds(snakeHead); //update bounds for snakeHead

      //check the buffer
      switch(buffer[snakeHead.x][snakeHead.y]) {

        //these check if the head runs into the body and end the loop
        case 1: //up
        case 2: //down
        case 3: //left
        case 4: //right
          gameOver = !gameOver;
          break;
        case 5: //food
          MatDisp.setLed(0, snakeHead.x, snakeHead.y, true); //display new head position
          newFood(buffer);
          break;
        case 0: //empty slot
          MatDisp.setLed(0, snakeHead.x, snakeHead.y, true); //display new head position
          
          //get current tail position
          int tempX = snakeTail.x;
          int tempY = snakeTail.y;

          Serial.print("next tail move: ");
          Serial.println(buffer[snakeTail.x][snakeTail.y] - 1);
          snakeTail = addCoords(snakeTail, possibleDirections[buffer[snakeTail.x][snakeTail.y] - 1]); //pulls move from buffer and updates the tail

          buffer[tempX][tempY] = 0; //change last tail position in buffer to 0

          snakeTail = checkBounds(snakeTail); //update tail bounds

          MatDisp.setLed(0, snakeTail.x, snakeTail.y, false); //display tail
          break;
      }
      Serial.print("Head: ");
      Serial.print(snakeHead.x);
      Serial.print(" ");
      Serial.println(snakeHead.y);
      Serial.print("Tail: ");
      Serial.print(snakeTail.x);
      Serial.print(" ");
      Serial.println(snakeTail.y);
      Serial.println();
    }
  }
}

int getDirection(int curDir){
  int upDown = analogRead(0);
  int leftRight = analogRead(1);
  if(leftRight > 450 &&  leftRight < 600 && upDown > 450 && upDown < 600)
    return curDir;
  int up = 512, down = 512, left = 512, right = 512;
  if (leftRight < 500){
    right = leftRight;
    Serial.println("update Right");
  }
  else {
    left = 1023 - leftRight;
    Serial.println("update Left");
  }
  if (upDown < 500) {
    down = upDown;
    Serial.println("update Down");
  }
  else {
    up = 1023 - upDown;
    Serial.println("update Up");
  }
  int minimum = min(min(min(up, down), left), right);
  if (minimum == up) {
      return 0;
  } else if (minimum == down) {
      return 1;
  } else if (minimum == left) {
      return 2;
  } else {
      return 3;
  }
}

Coord addCoords(Coord curPos, Coord nextPos) {
  curPos.x += nextPos.x;
  curPos.y += nextPos.y;
  return curPos;
}

Coord checkBounds(Coord temp) {
  if (temp.x == 8)
    temp.x = 0;
  if (temp.x == -1)
    temp.x = 7;
  if (temp.y == 8)
    temp.y = 0;
  if (temp.y == -1)
    temp.y = 7;
  return temp;
}

void newFood(int buffer[8][8]) {
  int foodX = 0;
  int foodY = 0;
  bool collidesWithBody = true;
  while(collidesWithBody) {
    foodX = random(0, 8);
    foodY = random(0, 8);
    if(buffer[foodX][foodY] == 0)
      collidesWithBody = !collidesWithBody;
  }
  MatDisp.setLed(0, foodX, foodY, true);
  buffer[foodX][foodY] = 5;
  Serial.println("Exited newFood");
}