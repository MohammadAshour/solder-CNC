int RL=21,BL=20,GL=19;                  //temp pins simulates motors and relay
int d0=15,d1=16,d2=17,d3=18;            //4 pins for 7Seg
int start=29;                           //start button
int pause=2,reset=3;                    //pause and reset interrupts
int up=26,right=25,left=24,down=23;     //directions for manual control
int switchmanual=22;                    //switch for choosing manual control

int manual;                             //to check if manual mode is activated
int isRunning;                          //to check if maching in runnning mode
int homming;                            //to check if maching is going to home 0,0
int plateNum;                           //contains plate number .. the number shown on 7Seg
int numX,numY,disX,disY;                //variables determind by the plate number
int currentX,currentY;                  //to where the machine is respecting to home point

void setup() {
  pinMode(d0,OUTPUT);
  pinMode(d1,OUTPUT);
  pinMode(d2,OUTPUT);
  pinMode(d3,OUTPUT);
  
  pinMode(GL,OUTPUT);
  pinMode(RL,OUTPUT);
  pinMode(BL,OUTPUT);

  pinMode(start,INPUT);
  pinMode(pause,INPUT);
  pinMode(reset,INPUT);

  pinMode(switchmanual,INPUT);
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
  digitalWrite(GL,LOW);
  digitalWrite(RL,LOW);
  digitalWrite(BL,LOW);

  homming=0;
  plateNum=0;
  manual=0;
  isRunning=0;
  currentX=0;currentY=0;
}

void loop() {
  //get plate number from the potentiometer and display it on the 7Seg
  plateNum=analogRead(0)*9/1023;
  showDigit(plateNum);

  //set variables of the selected plate
  switch(plateNum){
    case 0: numX=4;numY=3;disX=2;disY=3;break;
    case 1: numX=10;numY=20;disX=2;disY=3;break;
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
  if(digitalRead(switchmanual))manual=1;
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
      for(int j=1;j<numY;j++){
        despenser();
        for(int b=0;b<disY;b++)moveDown();
        currentY++;
      }
      despenser();
      for(int a=0;a<disX;a++)moveLeft();
      currentX++;
      for(int j=1;j<numY;j++){
        despenser();
        for(int b=0;b<disY;b++)moveUp();
        currentY--;
      }
      despenser(); 
      if(i!=numX/2-1){
        for(int a=0;a<disX;a++)moveLeft();
        currentX++;
      }
      
      
    }
    isRunning=0;
    homming=1; 
  }
}


void pauseM(){
  isRunning=0;
  digitalWrite(4,HIGH);
}

void resetM(){
  isRunning=0;
  homming=1;
}

void goHome(){
  isRunning=0;
  int totalX=(currentX-1)*disX;
  for(int i=0;i<totalX;i++){
    moveRight();
  }
  int totalY=(currentY-1)*disY;
  for(int i=0;i<totalY;i++){
    moveUp();
  }
  homming=0;
  currentX=0;
  currentY=0;
}

void despenser(){
  if(isRunning==1){
    digitalWrite(GL,HIGH);
    delay(1000);
    digitalWrite(GL,LOW);   
  }
}

void moveUp(){
  digitalWrite(4,isRunning);
  if(isRunning==1){
    digitalWrite(RL,HIGH);
    delay(100);
    digitalWrite(RL,LOW);
    delay(100);
  }
}

void moveDown(){
  digitalWrite(4,isRunning);
  if(isRunning==1){ 
    digitalWrite(RL,HIGH);
    delay(200);
    digitalWrite(RL,LOW);
    delay(100);
  }
}



void moveRight(){
  digitalWrite(4,isRunning);
  if(isRunning==1){
    digitalWrite(BL,HIGH);
    delay(100);
    digitalWrite(BL,LOW);
    delay(100);
  }
}

void moveLeft(){
  digitalWrite(4,isRunning);
  if(isRunning==1){ 
    digitalWrite(BL,HIGH);
    delay(200);
    digitalWrite(BL,LOW);
    delay(100);
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
