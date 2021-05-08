int LEDState=0;
int RLEDPin=8;
int GLEDPin=9;
int buttonPin=12;
int buttonNew;
int buttonOld=1;
int dt=100;

#include <Chrono.h>

/////////////////////////////Tipping Bucket definitions/////////////////////////////////
#define       RAIN_PIN                          3               //Tipping bucket input
#define       CALC_INTERVAL                     1000            //Increment of measurements in milliseconds
#define       DEBOUNCE_TIME                     80              //Time in milliseconds to get through bounce noise

const double  bucketAmount        =             0.408;          //mm equivalent of ml to trip the bucket
double        totalRain           =             0.0;
double        rainRate            =             0.0;           

unsigned long nextCalc;                                         //"Average Switch closure time is 135 ms"
unsigned long timer;                                            //"Bounce Settling Time: 0.75 ms" 

unsigned int  rainTrigger         =             0;
unsigned long last_micros_rg;                                   //will store last time bucket tipped
unsigned long tipTime;

void countingRain() { 
  if ((long)(micros() - last_micros_rg) >= DEBOUNCE_TIME) {
   tipTime = (micros() - last_micros_rg);
   rainTrigger += 1;
   last_micros_rg = micros();
  }  
}
//////////////////////////////////////////////
#include  "DHT.h";
#include  "NewPing.h"

#define   DHTPIN            7                        // DHT-11 Output Pin connection
#define   DHTTYPE           DHT11                    // DHT Type is DHT 11 (AM2302)
#define   TRIGGER_PIN_1     10
#define   ECHO_PIN_1        10
#define   TRIGGER_PIN_2     5
#define   ECHO_PIN_2        5
#define   MAX_DISTANCE      400

NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);

double    hum;                                        // Stores humidity value in percent
double    temp;                                       // Stores temperature value in Celcius
double    duration1;                                  // Stores First HC-SR04 pulse duration value
double    duration2;                                  // Stores Second HC-SR04 pulse duration value
double    distance1;                                  // Stores calculated distance in cm for First Sensor
double    distance2;                                  // Stores calculated distance in cm for Second Sensor
double    distanceA;                                  // Stores calculated average distance in cm for two sensors
double    soundsp;                                    // Stores calculated speed of sound in M/S
double    soundcm;                                    // Stores calculated speed of sound in cm/ms
int       iterations =      5;

DHT dht(DHTPIN, DHTTYPE);

#define   SAMPLE_COUNT      4
double    dis[SAMPLE_COUNT];                           // Array to hold the sample data
int       counter   =       0;                         // Counter to keep track of the current sample location

///////Chrono 
Chrono ultra;                                          // Metro timer for 10 second intervals (used to time publishing level sensor to Serial )
Chrono wait;
Chrono tipping;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(RLEDPin, OUTPUT);
pinMode(GLEDPin, OUTPUT);
pinMode(buttonPin, INPUT);

dht.begin();
  
  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), countingRain, RISING); 
  
  pinMode(RAIN_PIN, INPUT);
  nextCalc = millis() + CALC_INTERVAL;
    
}

void loop() {
buttonNew=digitalRead(buttonPin);
if(buttonOld==0 && buttonNew==1){
  if (LEDState==0){
    digitalWrite(RLEDPin,HIGH);
    digitalWrite(GLEDPin, LOW);
    LEDState=1;
    Serial.print("RED");
  }
  else{
    digitalWrite(RLEDPin, LOW);
    digitalWrite(GLEDPin, HIGH);
    LEDState=0;
    Serial.print("GREEN");
  }
}
buttonOld=buttonNew;
delay(dt);
}
