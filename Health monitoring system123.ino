
#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>
#include <dht.h> 
#include <OneWire.h>
#include <DallasTemperature.h>  
#include "HX711.h"
#define DEBUG_HX711  
#include <Servo.h>     
//  Variables
const int PulseWire = 0;       // pulse sensor pin A0
const int LED1 = 1;          //led pin red at 1
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value.                                
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
// atmospheric temperarture and humidity
#define outPin 22 // dht11 pin 22
dht DHT;
// MQ135
int sensorValue;    // Mq135 At A2
int digitalValue;     //  led for MQ 135 at 23
//Body temperature
#define ONE_WIRE_BUS 5      // ds18b20 at 5
OneWire oneWire(ONE_WIRE_BUS);	
DallasTemperature sensors(&oneWire);
float Body_temp ;
const int LED11 = 11; // YELLOW LED FOR bODY TEMP
const int LED12 = 12; // RED LED FOR BODY TEMP
// body shivering
int vib_pin=6; //vibration sensor pin 6
int led_pin=7;  //vibration section led 7
// body moisture
const int sensor_pin = A1;  // Body moisture at A1

const int led8 = 8 ; //led for moisture sensor is at 8
// led for pulse sensor
const int LED9 = 9;   // yellow led  
const int LED10 = 10;  // red led
// weight sensor
#define CALIBRATION_FACTOR 20780.0
// data pin and clock pin
byte pinData = 3;
byte pinClk = 2;
// define HX711
HX711 scale;
const int LED42 = 42; //Green
const int LED44 = 44;// yellow
const int LED46 = 46;// red
float glucoseW ;
//servo rotation for glucose
Servo myServo;
int servoPin = 13;

int button1 = 24;      //-180
int button2 = 26;      //-90
int button3 = 28;      //0
int button4 = 30;      //90
int button5 = 40;      //180
 


int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;
int button5State = 0;



int servoAngle = 0;

void setup() {   
  Serial.begin(9600); 
  pinMode(4, OUTPUT);// buzzer at 4
  // pulse sensor 
  pinMode(LED9, OUTPUT);
  pinMode(LED10, OUTPUT);        
  pulseSensor.analogInput(PulseWire);   
  
  pulseSensor.setThreshold(Threshold);   
  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

  // MQ135
  pinMode(23, OUTPUT);
  
  // body shivering
  pinMode(vib_pin,INPUT);
  pinMode(led_pin,OUTPUT);
  // body moisture
  pinMode(led8, OUTPUT);
  // body temp
  pinMode(LED11, OUTPUT);
  pinMode(LED12, OUTPUT);
 // weight sensor
  #ifdef DEBUG_HX711
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("[HX7] Sensor start HX711");
  #endif

  //Initializing sensor
  scale.begin(pinData, pinClk);
  // apply the calibration value
  scale.set_scale(CALIBRATION_FACTOR);
  // Initialize the tare
  //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale.tare();
  // servo motor for glucose
  myServo.attach(servoPin);
  myServo.write(-180);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(button5, INPUT);

  

}



void loop() {
  
  // pulse sensor
  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                                // "myBPM" hold this BPM value now. 
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 
   Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
   Serial.print("BPM: ");                        // Print phrase "BPM: " 
   Serial.println(myBPM);                        // Print the value inside of myBPM. 
   if (myBPM < 60){
     digitalWrite(LED9, HIGH);
     digitalWrite(4, HIGH);      
    }
   else if(myBPM > 100){
     digitalWrite(LED10, HIGH);
     digitalWrite(4, HIGH);
   } 
   else{
     digitalWrite(LED9, LOW);
     digitalWrite(LED10, LOW);
     digitalWrite(4, LOW);
    }        
  }
  delay(1000);                    // considered best practice in a simple sketch.
  // Dht 11
  int readData = DHT.read11(outPin);

	float t = DHT.temperature;        // Read temperature
	float h = DHT.humidity;           // Read humidity

	Serial.print("Surrounding Temperature = ");
	Serial.print(t);
	Serial.print("°C | ");
	Serial.print((t*9.0)/5.0+32.0);        // Convert celsius to fahrenheit
	Serial.println("°F ");
	Serial.print(" Surrounding Humidity = ");
	Serial.print(h);
	Serial.println("% ");
	Serial.println("");

	delay(1000);
   //MQ135
  sensorValue = analogRead(0); // read analog input pin 0
 
  if (sensorValue > 350)
  {
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);    
  }
  else
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  Serial.println(sensorValue, DEC); // prints the value read
  Serial.println(digitalValue, DEC);
  delay(1000);  
  // Ds18b20
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  Serial.print("Body Temperature: ");
  
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print((char)176);//shows degrees character
  Serial.print("C  |  ");
  
  //print the temperature in Fahrenheit
  Body_temp = (sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0;
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);//shows degrees character
  Serial.println("F");
  if(Body_temp < 97){
    digitalWrite(LED10, HIGH);
    digitalWrite(4, HIGH);    
  }  
  else if(Body_temp > 99.5){
    digitalWrite(LED11, HIGH);
    digitalWrite(4, HIGH);
  }
  else{
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW); 
    digitalWrite(4, LOW);        
  }
  delay(1000); 
  // body shivering   
  int val;
  val=digitalRead(vib_pin);
  if(val==1)
  {
    digitalWrite(led_pin,HIGH);
    digitalWrite(4, HIGH);
    delay(1000);
    
  }
  else{
  digitalWrite(led_pin,LOW);
  digitalWrite(4, LOW);
  }
  // moisture sensor
  
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%\n\n");
  if(sensor_analog > 25){
    digitalWrite(led8,HIGH);
    digitalWrite(4,HIGH);    
  } 
  else{
    digitalWrite(led8,LOW);
    digitalWrite(4,LOW); 
  }
  delay(1000);
  //glucose sensor    
  #ifdef DEBUG_HX711
  Serial.print("[HX7] Reading: ");
  Serial.print(scale.get_units(), 2);
  Serial.print(" Kgs");
  Serial.println();
  #endif

  glucoseW = (scale.get_units(), 2);
  if (0.45<= glucoseW <0.55){
    if(glucoseW > 0.3){
      digitalWrite(LED42, HIGH);
    }
    else if (0.1 < glucoseW <= 0.3){
      digitalWrite(LED44, HIGH);
    }
    else if(glucoseW <= 0.07){
     digitalWrite(LED44, HIGH);
     digitalWrite(4, HIGH);
     myServo.write(-180);
    }
    else{
      digitalWrite(LED44, LOW);
      digitalWrite(LED44, LOW);
      digitalWrite(LED44, LOW);
      digitalWrite(4, LOW);
    }
  }
  else if (0.23 <= glucoseW < 0.29){
    if(glucoseW > 0.15){
      digitalWrite(LED42, HIGH);
    }
    else if (0.04 < glucoseW <= 0.15){
     digitalWrite(LED44, HIGH);
    }
    else if(glucoseW <= 0.04){
      digitalWrite(LED44, HIGH);
      digitalWrite(4, HIGH);
      myServo.write(-180);
    }
    else{
      digitalWrite(LED44, LOW);
      digitalWrite(LED44, LOW);
      digitalWrite(LED44, LOW);
      digitalWrite(4, LOW);
    }
  }
  else{}

  // servo motor for glucose
  button1State = digitalRead(button1);
  button2State = digitalRead(button2);
  button3State = digitalRead(button3);
  button4State = digitalRead(button4);
  button5State = digitalRead(button5);


  if (button1State == HIGH) {
    servoAngle = -180;
    myServo.write(servoAngle);
    delay(50);
  }
  
  if (button2State == HIGH) {
    servoAngle = -90;
    myServo.write(servoAngle);
    delay(50);
  }  
  
  if (button3State == HIGH) {
    servoAngle = 0;
    myServo.write(servoAngle);
    delay(50);
  }
  
  if (button4State == HIGH) {
    servoAngle = 90;
    myServo.write(servoAngle);
    delay(50);
  }
    
  if (button5State == HIGH) {
    servoAngle = 180;
    myServo.write(servoAngle);
    delay(50);
  }
}
