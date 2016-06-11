//#define USE_RATIO
#define USE_RANDOMNESS
#define DEBUG




#define MOTOR_NUM 15


int anemometerPin=A0;
int motorPin[MOTOR_NUM];




float motorSpeeds [15];


float randomSpeedVariation [15];


// Ratios 0.00001 - 0.99999 for motor "lag" - how slowly the motor reaches the speed defined by the anemometer. Higher = slower!
float ratios [15] = {0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9};


int mapLowIn;
int mapHighIn;
int mapLowOut;
int mapHighOut;

int variationOfSpeed;


void setup() {


motorPin[0]=2;
motorPin[1]=3;
motorPin[2]=4;
motorPin[3]=5;
motorPin[4]=6;
motorPin[5]=7;
motorPin[6]=8;
motorPin[7]=9;
motorPin[8]=10;
motorPin[9]=11;
motorPin[10]=12;
motorPin[11]=13;
motorPin[12]=44;
motorPin[13]=45;
motorPin[14]=46;


//Code you can change
mapLowIn=79;
mapHighIn=142;
mapLowOut=0;
mapHighOut=255;//255 is the max, make value lower if it 's too fast



  for(int i=0;i<MOTOR_NUM;i++){
    
      motorSpeeds[i] = 0;

      randomSpeedVariation[i]=random(0,1000);
      
    }

    variationOfSpeed=60;//value to experiment with
    

  // put your setup code here, to run once:
//optional to debug
#ifdef DEBUG
Serial.begin(9600);
#endif

//we declare which pin we are using
pinMode(anemometerPin,INPUT);



  for(int i=0;i<MOTOR_NUM;i++){

    pinMode(motorPin[i],OUTPUT);

    }

}

int calculateMotorSpeed(int index, int anemometerReading) {
  motorSpeeds[index] = motorSpeeds[index] * ratios[index] + (float)anemometerReading * (1.0 - ratios[index]);
  return (int)floor(motorSpeeds[index]);
}

float randomness(int uniqueValue, int anemometerReading) {

   float v= cos((millis()*0.01)+uniqueValue) * map(anemometerReading,mapLowIn,mapHighIn,0,variationOfSpeed);


  return v;
  
}


void loop() {
  // put your main code here, to run repeatedly:
int anemometerReading;
//get the value from the anemometer
 anemometerReading = analogRead(anemometerPin);
 
//debug
//set motor direction
//digitalWrite(motorPin1,HIGH);
//remap the value from anemometer to the motor // https://www.arduino.cc/en/Reference/Map



#ifdef USE_RATIO
  for(int i=0;i<MOTOR_NUM;i++){
    
      motorSpeeds[i] = map(calculateMotorSpeed(i, anemometerReading),mapLowIn,mapHighIn,mapLowOut,mapHighOut);
      
    }
#else

  for(int i=0;i<MOTOR_NUM;i++){
    
      motorSpeeds[i] = map(anemometerReading,mapLowIn,mapHighIn,mapLowOut,mapHighOut);
    }
    
#endif



#ifdef USE_RANDOMNESS

    for(int i=0;i<MOTOR_NUM;i++){
      motorSpeeds[i] += randomness( randomSpeedVariation[i], anemometerReading);
    }

#endif

    for(int i=0;i<MOTOR_NUM;i++){
 if(motorSpeeds[i]<0){
      motorSpeeds[i]=0;
    }
    
   if(motorSpeeds[i]>255){
      motorSpeeds[i]=255;
    }
    }




      #ifdef DEBUG
      Serial.print("anemometerReading:");
      Serial.println(anemometerReading);
      Serial.println();
      #endif

  //set motor speed
    for(int i=0;i<MOTOR_NUM;i++){
      analogWrite(motorPin[i],motorSpeeds[i]);
  //  delay(1);

      #ifdef DEBUG
      Serial.print("motor pin:");
      Serial.print(motorPin[i]);
      Serial.print("is running at:");
      Serial.println(motorSpeeds[i]);
      #endif
      
    }
  

delay(5);

}

