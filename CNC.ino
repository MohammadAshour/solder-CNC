#include <EEPROM.h>
int MX0=11,MX1=10,MX2=12,MX3=9;         //X motor
int MY0=A1,MY1=A2,MY2=A3,MY3=A0;        //Y motor
int RE=8;                               //Pedal relay
int start=3;                            //start button
int reset=2;                            //pause and reset interrupts

int s1=6,s2=5,s3=4;

int isRunning;                          //to check if maching in runnning mode
int homming;                            //to check if maching is going to home 0,0
int plateNum;                           //contains plate number .. the number shown on 7Seg
int numX,numY,disX,disY;                //variables determind by the plate number
int totalX,totalY;                      //to know where the machine is respecting to home point


int Ystatus=0;                          //Motors status
int Xstatus=0;

int Mdelay=5;                           //delay for motors         
int Ddelay=1000;                        //delay for despenser

void setup() {
  pinMode(s1,INPUT);
  pinMode(s2,INPUT);
  pinMode(s3,INPUT);
  
  pinMode(RE,OUTPUT);
  pinMode(MX0,OUTPUT);
  pinMode(MX1,OUTPUT);
  pinMode(MX2,OUTPUT);
  pinMode(MX3,OUTPUT);
  pinMode(MY0,OUTPUT);
  pinMode(MY1,OUTPUT);
  pinMode(MY2,OUTPUT);
  pinMode(MY3,OUTPUT);
  
  pinMode(start,INPUT);

  attachInterrupt(0,resetM,RISING);

  idle();

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
        moveDown(disY);
      }
      
      despenser();                                  //despense then move left
      moveRight(disX);
      
      for(int j=1;j<numY;j++){                      //despense and move up
        despenser();
        moveUp(disY);
      }
      
      despenser();                                  //last terminal in the round
      if(i!=numX/2-1){                              //prevent moving left in the last round
        moveRight(disX);
      }
    
    }
    isRunning=0;
  }
}


void resetM(){
  isRunning=0;                            //to break moving functions         down,left and despenser
  homming=1;                              //to start going home functions     right and up
}

void goHome(){
  moveLeft(totalX);
  moveUp(totalY);
  homming=0;
}

void despenser(){
  if(isRunning==1){
    digitalWrite(RE,HIGH);
    delay(500);
    digitalWrite(RE,LOW);   
  }
}


void idle(){
   digitalWrite(MX0,LOW);
   digitalWrite(MX1,LOW);
   digitalWrite(MX2,LOW);
   digitalWrite(MX3,LOW);
   
   digitalWrite(MY0,LOW);
   digitalWrite(MY1,LOW);
   digitalWrite(MY2,LOW);
   digitalWrite(MY3,LOW);
   
   digitalWrite(RE,LOW);
}


void S_CY(){                                  //step clock wise motor Y
     switch(Ystatus){
      case 0:
         digitalWrite(MY3,LOW);
         digitalWrite(MY1,HIGH);
         Ystatus=1;
         break;
      case 1:
         digitalWrite(MY0,LOW);
         digitalWrite(MY2,HIGH);
         Ystatus=2;
         break; 
      case 2:
         digitalWrite(MY1,LOW);
         digitalWrite(MY3,HIGH);
         Ystatus=3;
         break;
      case 3:
         digitalWrite(MY2,LOW);
         digitalWrite(MY0,HIGH);
         Ystatus=0;
         break;
      }
}

void S_CCY(){                                         //step counter clock wise motor Y
   switch(Ystatus){
      case 0:
         digitalWrite(MY1,LOW);
         digitalWrite(MY3,HIGH);
         Ystatus=3;
         break;
      case 3:
         digitalWrite(MY0,LOW);
         digitalWrite(MY2,HIGH);
         Ystatus=2;
         break; 
      case 2:
         digitalWrite(MY3,LOW);
         digitalWrite(MY1,HIGH);
         Ystatus=1;
         break;
      case 1:
         digitalWrite(MY2,LOW);
         digitalWrite(MY0,HIGH);
         Ystatus=0;
         break;
      }
}

void S_CCX(){                                           //step clock wise motor X
   switch(Xstatus){
      case 0:
         digitalWrite(MX1,LOW);
         digitalWrite(MX3,HIGH);
         Xstatus=3;
         break;
      case 3:
         digitalWrite(MX0,LOW);
         digitalWrite(MX2,HIGH);
         Xstatus=2;
         break; 
      case 2:
         digitalWrite(MX3,LOW);
         digitalWrite(MX1,HIGH);
         Xstatus=1;
         break;
      case 1:
         digitalWrite(MX2,LOW);
         digitalWrite(MX0,HIGH);
         Xstatus=0;
         break;
      }
}

void S_CX(){                                                //step counter clock wise motor X
     switch(Xstatus){
      case 0:
         digitalWrite(MX3,LOW);
         digitalWrite(MX1,HIGH);
         Xstatus=1;
         break;
      case 1:
         digitalWrite(MX0,LOW);
         digitalWrite(MX2,HIGH);
         Xstatus=2;
         break; 
      case 2:
         digitalWrite(MX1,LOW);
         digitalWrite(MX3,HIGH);
         Xstatus=3;
         break;
      case 3:
         digitalWrite(MX2,LOW);
         digitalWrite(MX0,HIGH);
         Xstatus=0;
         break;
      }
}

void moveUp(int steps){
  for(int i=0;i<steps;i++){
    if(isRunning==1||homming==1){
      S_CY();
      totalY--;
      delay(Mdelay);
    }
  }
}

void moveDown(int steps){
  for(int i=0;i<steps;i++){
    if(isRunning==1){
      S_CCY();
      totalY++;
      delay(Mdelay);
    }
  }
}



void moveRight(int steps){
  for(int i=0;i<steps;i++){
    if(isRunning==1){
      S_CX();
      totalX++;
      delay(Mdelay);
    }
  }
}

void moveLeft(int steps){
  for(int i=0;i<steps;i++){
    if(isRunning==1||homming==1){
      S_CCX();
      totalX--;
      delay(Mdelay);
    }
  }
}
