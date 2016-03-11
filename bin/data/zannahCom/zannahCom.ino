#define motorNumber 2

const int anemometerPin = A0;

int anemometerReading;

int directionPin[] = {23,25};
int pmwPin[] = {2,3};


long motorSpeed[motorNumber];


bool twoFrames=true;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);

for(int i=0;i<motorNumber;i++){

  pinMode(directionPin[i],OUTPUT);
  pinMode(pmwPin[i],OUTPUT);
  
  }
  
  pinMode(anemometerPin, INPUT);
}

void loop() { 


if(twoFrames==true){

serialFromOf();


for(int i=0;i<motorNumber;i++){

  digitalWrite(directionPin[i], LOW);//set direction
  analogWrite(pmwPin[i], motorSpeed[i]);//set speed

}
delay(5);

}

  if(twoFrames==false){
   Serial.println(analogRead(anemometerPin)); 
   delay(5);
  }

  if(twoFrames==false){
  twoFrames=true;
  }
  else if(twoFrames==true){
  twoFrames=false;
  // Serial.println(""); 
  // Serial.flush();
  }

  

}


void serialFromOf(){
  
//Listen Serial
    while (Serial.available() > 0) {
      
 
    //  Serial.println("Serial in!"+Serial.read());
    // look for the next valid integer in the incoming serial stream:
unsigned long receive[motorNumber];


 for(int i=0;i<motorNumber;i++){
  receive[i] = Serial.parseInt(); 
 }
 
   

    char lastC=Serial.read();

    if (lastC=='n') 
    {
    for(int i=0;i<motorNumber;i++){
    motorSpeed[i]=receive[i]; 
    }

  Serial.println("end");
    }
  
  }
  
  
}
