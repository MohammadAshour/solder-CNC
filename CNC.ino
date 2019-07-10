int RL=21,BL=20,GL=19;
int d0=15,d1=16,d2=17,d3=18;
int start=29;
int pause=2,reset=3;
int up=26,right=25,left=24,down=23;
int switchMode=22;

int termNum;
int mode;
int isRunning;
int currentX;
int currentY;
int homming;
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

  pinMode(switchMode,INPUT);
  pinMode(up,INPUT);
  pinMode(right,INPUT);
  pinMode(down,INPUT);
  pinMode(left,INPUT);
  
  pinMode(0,INPUT);

  digitalWrite(d0,LOW);  
  digitalWrite(d1,LOW);  
  digitalWrite(d2,LOW);  
  digitalWrite(d3,LOW);  
  digitalWrite(GL,LOW);
  digitalWrite(RL,LOW);
  digitalWrite(BL,LOW);


  attachInterrupt(0,pauseM,RISING);
  attachInterrupt(1,resetM,RISING);

  homming=0;
  termNum=0;
  mode=0;
  isRunning=0;
  currentX=0,currentY=0;
}

void loop() {
   termNum=analogRead(0)*9/1023;
   showDigit(termNum);

   if(digitalRead(switchMode))mode=1;
   else mode=0;

   if(digitalRead(up)&&mode) moveUp();
   if(digitalRead(down)&&mode) moveDown();
   if(digitalRead(right)&&mode) moveRight();
   if(digitalRead(left)&&mode) moveLeft();

   if(digitalRead(start)&&!mode) isRunning=1;
   if(digitalRead(pause)&&!mode) isRunning=0; 
  
  if(homming)goHome();  
   while(isRunning){
    moveUp();
    }
}
void pauseM(){
  isRunning=0;
}

void resetM(){
  isRunning=0;
  homming=1;
}

void goHome(){
  isRunning=0;
  currentX=0;
  currentY=0;
  for(int i=0;i<3;i++){
    moveDown();
    moveLeft();
  }
  homming=0;
}
void moveUp(){
    digitalWrite(RL,HIGH);
    delay(100);
    digitalWrite(RL,LOW);
    delay(100);
}

void moveDown(){
    digitalWrite(RL,HIGH);
    delay(500);
    digitalWrite(RL,LOW);
    delay(500);
}

void moveRight(){
    digitalWrite(BL,HIGH);
    delay(100);
    digitalWrite(BL,LOW);
    delay(100);
}

void moveLeft(){
    digitalWrite(BL,HIGH);
    delay(500);
    digitalWrite(BL,LOW);
    delay(500);
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
