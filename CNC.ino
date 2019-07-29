#include <EEPROM.h>
int MU=12,MD=11,MR=13,ML=10,RE=9;       //temp pins simulates motors and relay
int start=29;                           //start button
int reset=3;                            //pause and reset interrupts

int s1=26,s2=27,s3=28;

int isRunning;                          //to check if maching in runnning mode
int homming;                            //to check if maching is going to home 0,0
int plateNum;                           //contains plate number .. the number shown on 7Seg
int numX,numY,disX,disY;                //variables determind by the plate number
int totalX,totalY;                      //to know where the machine is respecting to home point

void setup() {
  pinMode(s1,INPUT);
  pinMode(s2,INPUT);
  pinMode(s3,INPUT);
  
  pinMode(RE,OUTPUT);
  pinMode(MU,OUTPUT);
  pinMode(MD,OUTPUT);
  pinMode(MR,OUTPUT);
  pinMode(ML,OUTPUT);

  pinMode(start,INPUT);

  attachInterrupt(1,resetM,RISING);

  digitalWrite(RE,LOW);
  digitalWrite(MU,LOW);
  digitalWrite(MD,LOW);
  digitalWrite(MR,LOW);
  digitalWrite(ML,LOW);

  homming=0;
  plateNum=0;
  isRunning=0;
  totalX=0;totalY=0;
}

void loop() {
  plateNum=0;
  if(digitalRead(s1))plateNum+=1;
  if(digitalRead(s2))plateNum+=2;
  if(digitalRead(s3))plateNum+=4;

  EEPROM.write(0,plateNum);
  

  //set variables of the selected plate
  //numX must be even number or last column will be neglected
  switch(plateNum){
    case 0: numX=10;numY=7;disX=5;disY=4;break;
    case 1: numX=4;numY=5;disX=5;disY=5;break;
    case 2: numX=7;numY=7;disX=2;disY=3;break;
    case 3: numX=2;numY=1;disX=2;disY=3;break;
    case 4: numX=2;numY=1;disX=2;disY=3;break;
    case 5: numX=2;numY=1;disX=2;disY=3;break;
    case 6: numX=2;numY=1;disX=2;disY=3;break;
    case 7: numX=2;numY=1;disX=2;disY=3;break;
  }

  
  
  //start the program
  if(digitalRead(start)) isRunning=1;

  
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


void resetM(){
  isRunning=0;                            //to break moving functions         down,left and despenser
  homming=1;                              //to start going home functions     right and up
}

void goHome(){
  for(totalX;totalX>0;totalX--){
    moveRight();
  }
  for(totalY;totalY>0;totalY--){
    moveUp();
  }
  homming=0;
}

void despenser(){
  if(isRunning==1){
    digitalWrite(RE,HIGH);
    delay(500);
    digitalWrite(RE,LOW);   
  }
}

void moveUp(){
  if(isRunning==1||homming==1){
    digitalWrite(MU,HIGH);
    delay(100);
    digitalWrite(MU,LOW);
  }
}

void moveDown(){
  if(isRunning==1){ 
    digitalWrite(MD,HIGH);
    delay(100);
    digitalWrite(MD,LOW);
  }
}



void moveRight(){
  if(isRunning==1||homming==1){
    digitalWrite(MR,HIGH);
    delay(100);
    digitalWrite(MR,LOW);
  }
}

void moveLeft(){
  if(isRunning==1){ 
    digitalWrite(ML,HIGH);
    delay(100);
    digitalWrite(ML,LOW);
  }
}
