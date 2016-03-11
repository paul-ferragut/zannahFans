#define motorNumber 2

const int anemometerPin = A0;

int anemometerReading;

int directionPin[] = {25,23};
int pmwPin[] = {2,3};




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



for(int i=0;i<motorNumber;i++){

  digitalWrite(directionPin[i], HIGH);//set direction
  analogWrite(pmwPin[i], 255);//set speed

}
delay(5);

  

}


