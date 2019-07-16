#include <EEPROM.h>
int MU=12,MD=11,MR=13,ML=10,RE=9;       //temp pins simulates motors and relay
int d0=15,d1=16,d2=17,d3=18;            //4 pins for 7Seg
int start=29;                           //start button
int pause=2,reset=3;                    //pause and reset interrupts
int up=26,right=25,left=24,down=23;     //directions for manual control
int switchManual=22;                    //switch for choosing manual control

bool paused;                            //to pause running program
int manual;                             //to check if manual mode is activated
int isRunning;                          //to check if maching in runnning mode
int homming;                            //to check if maching is going to home 0,0
int plateNum;                           //contains plate number .. the number shown on 7Seg
int numX,numY,disX,disY;                //variables determind by the plate number
int totalX,totalY;                      //to know where the machine is respecting to home point

void setup() {
  pinMode(d0,OUTPUT);
  pinMode(d1,OUTPUT);
  pinMode(d2,OUTPUT);
  pinMode(d3,OUTPUT);
  
  pinMode(RE,OUTPUT);
  pinMode(MU,OUTPUT);
  pinMode(MD,OUTPUT);
  pinMode(MR,OUTPUT);
  pinMode(ML,OUTPUT);

  pinMode(start,INPUT);
  pinMode(pause,INPUT);
  pinMode(reset,INPUT);

  pinMode(switchManual,INPUT);
  pinMode(up,INPUT);
  pinMode(right,INPUT);
  pinMode(down,INPUT);
  pinMode(left,INPUT);
  
  pinMode(0,INPUT);

  //for depugging
  pinMode(4,OUTPUT);

  attachInterrupt(0,pauseM,RISING);
  attachInterrupt(1,resetM,RISING);

  digitalWrite(d0,LOW);  
  digitalWrite(d1,LOW);  
  digitalWrite(d2,LOW);  
  digitalWrite(d3,LOW);  
  digitalWrite(RE,LOW);
  digitalWrite(MU,LOW);
  digitalWrite(MD,LOW);
  digitalWrite(MR,LOW);
  digitalWrite(ML,LOW);

  homming=0;
  plateNum=0;
  manual=0;
  isRunning=0;
  totalX=0;totalY=0;
  paused=false;  
}

void loop() {
  //get plate number from the potentiometer and display it on the 7Seg
  plateNum=analogRead(0)*9/1023;            //map potentiometer reading to 0-9 value
  showDigit(plateNum);

  //set variables of the selected plate
  //numX must be even number or last column will be neglected
  switch(plateNum){
    case 0: numX=4;numY=3;disX=5;disY=4;break;
    case 1: numX=4;numY=3;disX=20;disY=30;break;
    case 2: numX=10;numY=20;disX=2;disY=3;break;
    case 3: numX=10;numY=20;disX=2;disY=3;break;
    case 4: numX=10;numY=20;disX=2;disY=3;break;
    case 5: numX=10;numY=20;disX=2;disY=3;break;
    case 6: numX=10;numY=20;disX=2;disY=3;break;
    case 7: numX=10;numY=20;disX=2;disY=3;break;
    case 8: numX=10;numY=20;disX=2;disY=3;break;
    case 9: numX=10;numY=20;disX=2;disY=3;break;
  }

  //selecting mode
  if(digitalRead(switchManual))manual=1;
  else manual=0;

  //manual control if manual is selected
  if(digitalRead(up)&&manual) moveUp();
  if(digitalRead(down)&&manual) moveDown();
  if(digitalRead(right)&&manual) moveRight();
  if(digitalRead(left)&&manual) moveLeft();

  //start the program if manual not selected
  if(digitalRead(start)&&!manual) isRunning=1;

  
  if(homming)goHome();  
  
  while(isRunning){
    for(int i=0;i<numX/2;i++){
      for(int j=1;j<numY;j++){                      //despense and move down
        despenser();                              
        for(int b=0;b<disY;b++){
          moveDown();
          totalY++;
        }
      }
      despenser();                                  //despense then move left
      for(int a=0;a<disX;a++){
        moveLeft();
        totalX++;  
      }
      for(int j=1;j<numY;j++){                      //despense and move up
        despenser();
        for(int b=0;b<disY;b++){
          moveUp();
          totalY--;
        }
      }
      despenser();                                  //last terminal in the round
      if(i!=numX/2-1){                              //prevent moving left in the last round
        for(int a=0;a<disX;a++){
          moveLeft();
          totalX++;
        }
      }
    }
    isRunning=0;
    homming=1; 
  }
}


void pauseM(){
  paused=!paused;
}

void resetM(){
  isRunning=0;                            //to break moving functions         down,left and despenser
  homming=1;                              //to start going home functions     right and up
}

void goHome(){
  while(paused){delay(10);}
  
  for(totalX;totalX>0;totalX--){
    moveRight();
  }
  for(totalY;totalY>0;totalY--){
    moveUp();
  }
  homming=0;
}

void despenser(){
  while(paused){delay(10);}
  if(isRunning==1){
    digitalWrite(RE,HIGH);
    delay(500);
    digitalWrite(RE,LOW);   
  }
}

void moveUp(){
  while(paused){delay(10);}
  if(isRunning==1||homming==1||manual){
    digitalWrite(MU,HIGH);
    delay(100);
    digitalWrite(MU,LOW);
  }
}

void moveDown(){
  while(paused){delay(10);}
  if(isRunning==1||manual){ 
    digitalWrite(MD,HIGH);
    delay(100);
    digitalWrite(MD,LOW);
  }
}



void moveRight(){
  while(paused){delay(10);}
  if(isRunning==1||homming==1||manual){
    digitalWrite(MR,HIGH);
    delay(100);
    digitalWrite(MR,LOW);
  }
}

void moveLeft(){
  while(paused){delay(10);}
  if(isRunning==1||manual){ 
    digitalWrite(ML,HIGH);
    delay(100);
    digitalWrite(ML,LOW);
  }
}


void showDigit(int x){
  digitalWrite(d0,x%2);
  x/=2;
  digitalWrite(d1,x%2);
  x/=2;
  digitalWrite(d2,x%2);
  x/=2;
  digitalWrite(d3,x%2);
}
