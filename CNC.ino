#include <EEPROM.h>
int MX0=11,MX1=3,MX2=4,MX3=5;             //X motor
int MY0=7,MY1=8,MY2=9,MY3=10;             //Y motor
int RE=12;                                //Pedal relay

int reset=2;                             //pause and reset interrupts
int s1=A0,s2=A1,s3=A2;
int start=A3;                             //start button
int limitUp=A4,limitLeft=A5;




int running;                            //to check if maching in runnning mode
int homming;                            //to check if maching is going to home 0,0
int stopUp,stopLeft;                    //to check if border reached
int plateNum;                           //contains plate number .. the number shown on 7Seg
int numX,numY,disX,disY;                //variables determind by the plate number


int Ystatus=0;                          //Motors status
int Xstatus=0;



int Mdelay=2;                           //delay for motors         
int Ddelay=1000;                        //delay for despenser

void setup() {
  attachInterrupt(0,resetM,RISING);

  pinMode(RE,OUTPUT);
  pinMode(MX0,OUTPUT);
  pinMode(MX1,OUTPUT);
  pinMode(MX2,OUTPUT);
  pinMode(MX3,OUTPUT);
  pinMode(MY0,OUTPUT);
  pinMode(MY1,OUTPUT);
  pinMode(MY2,OUTPUT);
  pinMode(MY3,OUTPUT);
  
  pinMode(RE,OUTPUT);
  
  pinMode(limitUp,INPUT);                     //set inputs and pullup resistors
  pinMode(limitLeft,INPUT);
  pinMode(start,INPUT);
  pinMode(s1,INPUT);
  pinMode(s2,INPUT);
  pinMode(s3,INPUT);
  
  digitalWrite(limitUp,HIGH);
  digitalWrite(limitLeft,HIGH);
  digitalWrite(start,HIGH);
  digitalWrite(s1,HIGH);
  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);
  
  
  homming=0;
  stopUp=1,stopLeft=1;
  plateNum=0;
  running=0;

  idle();
}

void loop() {
  plateNum=0;
  if(!digitalRead(s1))plateNum+=1;
  if(!digitalRead(s2))plateNum+=2;
  if(!digitalRead(s3))plateNum+=4;

  //set variables of the selected plate
  //numX must be even number or last column will be neglected
  switch(plateNum){
    case 0: numX=3;numY=3;disX=175;disY=137;break;
    case 1: numX=6;numY=7;disX=175;disY=137;break;
    case 2: numX=2;numY=4;disX=175;disY=137;break;
    case 3: numX=1;numY=6;disX=175;disY=137;break;
    case 4: numX=6;numY=5;disX=175;disY=137;break;
    case 5: numX=6;numY=1;disX=175;disY=137;break;
    case 6: numX=2;numY=2;disX=175;disY=137;break;
    case 7: numX=4;numY=3;disX=175;disY=137;break;
  }

  //if homming mode go home
  if(homming)goHome();  
  
  
  //start the program
  if(!digitalRead(start)) running=1;

  
  while(running){
    //go to origin
    goHome();

    
    for(int i=0;i<numY/2;i++){
        
      for(int j=1;j<numX;j++){                      //despense and move down
        despenser();                              
        moveRight(disX);
      }
      
      despenser();                                  //despense then move left
      moveUp(disY);
      
      for(int j=1;j<numX;j++){                      //despense and move up
        despenser();
        moveLeft(disX);
      }
      
      despenser();                                  //last terminal in the round
      if(i!=numY/2-1){                              //prevent moving left in the last round
        moveUp(disY);
      }  
    }
    //special case for odd number
    if(numY%2!=0){
      moveUp(disY);
      for(int j=1;j<numX;j++){                      //despense and move down
        despenser();                              
        moveRight(disX);
      }
      despenser();                              
    }
    running=0;
    homming=1;
  }
}


void resetM(){
  running=0;                                      //to break moving functions
  homming=1;                                      //to start going home function  
}

void goHome(){
  stopUp=0;
  stopLeft=0;
  while(!stopUp||!stopLeft){                        //if any limit switch not reached, continue
    if(digitalRead(limitUp))stopUp=1;               //upper limit reached
    if(digitalRead(limitLeft))stopLeft=1;           //letf limit reached
    
    if(!stopUp)S_CX();                              //if upper limit not reached move up
    if(!stopLeft)S_CY();                           //if left limit not reached move left
    delay(Mdelay);
  }
  homming=0;
  idle();                                           //no brake
}

void despenser(){
  if(running){
    digitalWrite(RE,HIGH);
    delay(Ddelay);
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

void S_CCX(){                                  //step clock wise motor Y
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

void S_CX(){                                         //step counter clock wise motor Y
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

void S_CCY(){                                  //step clock wise motor Y
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

void S_CY(){                                         //step counter clock wise motor Y
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

void moveLeft(int steps){
  for(int i=0;i<steps;i++){
    if(!running){EEPROM.write(0,1);break;}            //for test // doesnot work without it for no reason
    S_CY();
    delay(Mdelay);
 }
}

void moveRight(int steps){
  for(int i=0;i<steps;i++){
    if(!running)break;
    S_CCY();
    delay(Mdelay);
  }
}



void moveDown(int steps){
  for(int i=0;i<steps;i++){
    if(!running)break;
    S_CX();
    delay(Mdelay);
  }
}

void moveUp(int steps){
  for(int i=0;i<steps;i++){
    if(!running)break;
    S_CCX();
    delay(Mdelay);
  }
}
