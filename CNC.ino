#include <EEPROM.h>
int MX0=11,MX1=3,MX2=4,MX3=5;             //X motor
int MY0=7,MY1=8,MY2=9,MY3=10;             //Y motor
//int RE=8;                               //Pedal relay
//int start=3;                            //start button
//int reset=2;                            //pause and reset interrupts
//int limitUp=A4,limitLeft=A5;


//int s1=6,s2=5,s3=4;

int running;                          //to check if maching in runnning mode
int homming;                            //to check if maching is going to home 0,0
int stopUp,stopLeft;                    //to check if border reached
int plateNum;                           //contains plate number .. the number shown on 7Seg
int numX,numY,disX,disY;                //variables determind by the plate number


int Ystatus=0;                          //Motors status
int Xstatus=0;

int Mdelay=2;                           //delay for motors         
int Ddelay=1000;                        //delay for despenser

void setup() {
  //pinMode(s1,INPUT);
  //pinMode(s2,INPUT);
  //pinMode(s3,INPUT);
  
//  pinMode(RE,OUTPUT);
  pinMode(MX0,OUTPUT);
  pinMode(MX1,OUTPUT);
  pinMode(MX2,OUTPUT);
  pinMode(MX3,OUTPUT);
  pinMode(MY0,OUTPUT);
  pinMode(MY1,OUTPUT);
  pinMode(MY2,OUTPUT);
  pinMode(MY3,OUTPUT);
  
 // pinMode(limitUp,INPUT);
  //pinMode(limitLeft,INPUT);
  //pinMode(start,INPUT);
  
//  attachInterrupt(0,resetM,RISING);

  idle();

  homming=0;
  stopUp=0;
  stopLeft=0;
  plateNum=0;
//  running=0;
  running=1;
}

void loop() {
  plateNum=0;
  //if(digitalRead(s1))plateNum+=1;
  //if(digitalRead(s2))plateNum+=2;
  //if(digitalRead(s3))plateNum+=4;
  

  //set variables of the selected plate
  //numX must be even number or last column will be neglected
  switch(plateNum){
    case 0: numX=6;numY=7;disX=175;disY=137;break;
    case 1: numX=4;numY=5;disX=15;disY=20;break;
    case 2: numX=7;numY=7;disX=2;disY=3;break;
    case 3: numX=2;numY=1;disX=2;disY=3;break;
    case 4: numX=2;numY=1;disX=2;disY=3;break;
    case 5: numX=2;numY=1;disX=2;disY=3;break;
    case 6: numX=2;numY=1;disX=2;disY=3;break;
    case 7: numX=2;numY=1;disX=2;disY=3;break;
  }

  
  
  //start the program
//  if(digitalRead(start)) running=1;

  
  if(homming)goHome();  
  
  while(running){
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
    running=0;
  }
}


void resetM(){
  running=0;                                      //to break moving functions
  homming=1;                                        //to start going home function  
  stopUp=0;                                         //limits switches checkers
  stopLeft=0;
}

void goHome(){
  while(!stopUp||!stopLeft){                        //if any limit switch not reached, continue
    //if(digitalRead(limitUp))stopUp=1;               //upper limit reached
    //if(digitalRead(limitLeft))stopLeft=1;           //letf limit reached
    if(!stopUp)S_CY();                              //if upper limit not reached move up
    if(!stopLeft)S_CCX();                           //if left limit not reached move left
  }
  homming=0;
}

void despenser(){
  if(running){
 //   digitalWrite(RE,HIGH);
    delay(Ddelay);
   // digitalWrite(RE,LOW);   
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
   
//   digitalWrite(RE,LOW);
}

void S_CX(){                                  //step clock wise motor Y
     switch(Xstatus){
      case 0:
        digitalWrite(MX0,HIGH);
        digitalWrite(MX1,LOW);
        digitalWrite(MX2,HIGH);
        digitalWrite(MX3,LOW);
        Xstatus=1;
        break;
      case 1:
        digitalWrite(MX0,LOW);
        digitalWrite(MX1,HIGH);
        digitalWrite(MX2,HIGH);
        digitalWrite(MX3,LOW);
        Xstatus=2;
        break; 
      case 2:
        digitalWrite(MX0,LOW);
        digitalWrite(MX1,HIGH);
        digitalWrite(MX2,LOW);
        digitalWrite(MX3,HIGH);
        Xstatus=3;
        break;
      case 3:
        digitalWrite(MX0,HIGH);
        digitalWrite(MX1,LOW);
        digitalWrite(MX2,LOW);
        digitalWrite(MX3,HIGH);
        Xstatus=0;
        break;
      }
}

void S_CCX(){                                         //step counter clock wise motor Y
   switch(Xstatus){
      case 0:
        digitalWrite(MX0,LOW);
        digitalWrite(MX1,HIGH);
        digitalWrite(MX2,LOW);
        digitalWrite(MX3,HIGH);
        Xstatus=3;
        break;
      case 3:
        digitalWrite(MX0,LOW);
        digitalWrite(MX1,HIGH);
        digitalWrite(MX2,HIGH);
        digitalWrite(MX3,LOW);
        Xstatus=2;
        break; 
      case 2:
        digitalWrite(MX0,HIGH);
        digitalWrite(MX1,LOW);
        digitalWrite(MX2,HIGH);
        digitalWrite(MX3,LOW);
        Xstatus=1;
        break;
      case 1:
        digitalWrite(MX0,HIGH);
        digitalWrite(MX1,LOW);
        digitalWrite(MX2,LOW);
        digitalWrite(MX3,HIGH);
        Xstatus=0;
        break;
      }
}

void S_CY(){                                  //step clock wise motor Y
     switch(Ystatus){
      case 0:
        digitalWrite(MY0,HIGH);
        digitalWrite(MY1,LOW);
        digitalWrite(MY2,HIGH);
        digitalWrite(MY3,LOW);
        Ystatus=1;
        break;
      case 1:
        digitalWrite(MY0,LOW);
        digitalWrite(MY1,HIGH);
        digitalWrite(MY2,HIGH);
        digitalWrite(MY3,LOW);
        Ystatus=2;
        break; 
      case 2:
        digitalWrite(MY0,LOW);
        digitalWrite(MY1,HIGH);
        digitalWrite(MY2,LOW);
        digitalWrite(MY3,HIGH);
        Ystatus=3;
        break;
      case 3:
        digitalWrite(MY0,HIGH);
        digitalWrite(MY1,LOW);
        digitalWrite(MY2,LOW);
        digitalWrite(MY3,HIGH);
        Ystatus=0;
        break;
      }
}

void S_CCY(){                                         //step counter clock wise motor Y
   switch(Ystatus){
      case 0:
        digitalWrite(MY0,LOW);
        digitalWrite(MY1,HIGH);
        digitalWrite(MY2,LOW);
        digitalWrite(MY3,HIGH);
        Ystatus=3;
        break;
      case 3:
        digitalWrite(MY0,LOW);
        digitalWrite(MY1,HIGH);
        digitalWrite(MY2,HIGH);
        digitalWrite(MY3,LOW);
        Ystatus=2;
        break; 
      case 2:
        digitalWrite(MY0,HIGH);
        digitalWrite(MY1,LOW);
        digitalWrite(MY2,HIGH);
        digitalWrite(MY3,LOW);
        Ystatus=1;
        break;
      case 1:
        digitalWrite(MY0,HIGH);
        digitalWrite(MY1,LOW);
        digitalWrite(MY2,LOW);
        digitalWrite(MY3,HIGH);
        Ystatus=0;
        break;
      }
}

void moveUp(int steps){
  for(int i=0;i<steps;i++){
    if(!running){EEPROM.write(0,1);break;}            //for test // doesnot work without it for no reason
    S_CY();
    delay(Mdelay);
 }
}

void moveDown(int steps){
  for(int i=0;i<steps;i++){
    if(!running)break;
    S_CCY();
    delay(Mdelay);
  }
}



void moveRight(int steps){
  for(int i=0;i<steps;i++){
    if(!running)break;
    S_CX();
    delay(Mdelay);
  }
}

void moveLeft(int steps){
  for(int i=0;i<steps;i++){
    if(!running)break;
    S_CCX();
    delay(Mdelay);
  }
}
