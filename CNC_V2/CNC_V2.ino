#include <EEPROM.h>
int m1e=3;          //motor 1 enable
int m2e=4;          //motor 2 enable
int clockPin=5;
int dataPin=6;

void setup() {
  pinMode(m1e,OUTPUT);
  pinMode(m2e,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  digitalWrite(m1e,LOW);
  digitalWrite(m1e,HIGH);

  int32_t word0=00000000000000111111111111000000;
  int32_t word1=00000000000001111111111111000000;
  
  int32_t m1f=word0||101100;
}

void loop() {
  
}

void writeWord(bool m2,int32_t data){
  
  // activating STROBE
  if(m2)
    digitalWrite(m2e,LOW);
  else
    digitalWrite(m1e,LOW);

  
  //data xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
  //ex   xxxx xxxx xxxx x011 1111 1111 1101 0000
  //data is valid starting from bit 13 as word is 19 bits

  for(int i=13;i<32;i++){ 
    //clock LOW
    digitalWrite(clockPin,LOW);

    //set Data
    if(bitRead(data,i))
      digitalWrite(dataPin,HIGH);
    else
      digitalWrite(dataPin,LOW);

    //clock HIGH
    digitalWrite(clockPin,HIGH);
    delay(1);   
  }

  //disable writing
  digitalWrite(m1e,HIGH);
  digitalWrite(m2e,HIGH);
}
