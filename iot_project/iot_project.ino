//#include <SPI.h>
#include <WiFiUdp.h>
#include <Stepper.h>
#include <EEPROM.h>


//the initial wifi status
//int status = WL_IDLE_STATUS;

//WiFiUDP object used for the communication
WiFiUDP Udp;

//your network name (SSID) and password (WPA)
char ssid[] = "F23PRF";            
char pass[] = "15FEB2023"; 

//local port to listen on
int localPort = 3002;                               

//IP and port for the server
IPAddress serverIPAddress(192, 168, 110, 214);
int serverPort = 3001;       


#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12
int step_number = 1;

const int stepsPerRevolution = 300;

Stepper myStepper = Stepper(stepsPerRevolution, 8, 9, 10, 11);



// LED pins
const int ledPin = 6;

int i = 0;

// LDR sensor pin
const int sensorPin = A0;
int sensorValue = 0;

int lightValue = 0;

String latestCommand = "";

int stepCount = 0; // number of steps the motor has taken
int curtainUpperbound = 9000;
int curtainLowerbound = 0;

void setup() {
  analogWrite(ledPin, 0);
  delay(500);
  // put your setup code here, to run once:  
  pinMode(ledPin, OUTPUT);
  
  // pinMode(STEPPER_PIN_1, OUTPUT);
  // pinMode(STEPPER_PIN_2, OUTPUT);
  // pinMode(STEPPER_PIN_3, OUTPUT);
  // pinMode(STEPPER_PIN_4, OUTPUT);

  myStepper.setSpeed(100);

  stepCount = readFromEEPROM().toInt();

  Serial.begin(9600);
/*
  while (!Serial);

  //check the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    
    //don't continue
    while (true);
  }

  //attempt to connect to WiFi network
  while (status != WL_CONNECTED) {

    //connect to WPA/WPA2 network
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    //wait 10 seconds for connection
    delay(10000);
  }
  
  Serial.println("Connected to WiFi");
  
  //if you get a connection, report back via serial:
  Udp.begin(localPort);



 */
}


void loop() {

  //analogWrite(ledPin, 255);

  //setLightning("total_darkness");
  // put your main code here, to run repeatedly:åå
 // latestCommand = listenForUDPMessage();


  // setLightning(latestCommand);

/*

  updateSensor();
  Serial.println(sensorValue);


  analogWrite(ledPin, 0);
  


  Serial.println("OFF");
  delay(2000);

  analogWrite(ledPin, 5);
  Serial.println("ON 160 ");

  delay(2000);

  analogWrite(ledPin, 255);
  Serial.println("ON 255");
  delay(2000);
  */
 
  // Positiv step = op | Negativ step = ned
  

  if(i < 1){
    Serial.println("Lighten to 800");
    Serial.println("before");
    Serial.println(sensorValue);
    brightenTo(800);
    Serial.println("after");
    Serial.println(sensorValue);

    delay(5000);
    Serial.println("Dim to 100");
    Serial.println("before");
    Serial.println(sensorValue);
    dimTo(300);
    Serial.println("after");
    Serial.println(sensorValue);
    i++;
  }
  

}




//listens for incoming UDP messages
/*
String listenForUDPMessage() {

  //on package received
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    
    //buffer for incoming packages
    char packetBuffer[256]; 
  
    //read the packet into packetBufffer
    int msgLength = Udp.read(packetBuffer, 255);
    if (msgLength > 0) {
      packetBuffer[msgLength] = NULL;
    }

    //print message from packet
    Serial.print("\nReceived message: ");
    Serial.println(packetBuffer);

    //convert message value to int
    String messageValueAsString = String(packetBuffer);
    return messageValueAsString
    //set LED to received value
  //   setLEDTo(messageValueAsString);

    //send acknowledgement message
    //sendUDPMessage(Udp.remoteIP(), Udp.remotePort(), "ARDUINO: message was received");
  }
  
}
void sendUDPMessage(IPAddress remoteIPAddress, int remoteport, String message) {
  Serial.println("sendUDPMessageToServer");

  //get message string length (+1 to store a null value indicating the end of the message)
  int messageLength = message.length() + 1;
  
  //create char array 
  char messageBuffer[messageLength];

  //copy string message to char array
  message.toCharArray(messageBuffer, messageLength);

  //send the packet to the server
  Udp.beginPacket(remoteIPAddress, remoteport);
  Udp.write(messageBuffer);
  Udp.endPacket(); 
}
*/

void setLightning(String command) {
  
  int sensorVal = readSensorPin();
  Serial.println(sensorVal);
  
  if(command == "total_darkness"){
    
    analogWrite(ledPin, 0);
    //shutdown light

  } else if (command == "darkness"){
    analogWrite(ledPin, 5);

  } else if (command == "medium")
  {
    analogWrite(ledPin, 75);

  } else if (command == "bright") {
    analogWrite(ledPin, 150);

  } else if (command == "very_bright"){
    analogWrite(ledPin, 255);

  }
}


int readSensorPin(){
  return analogRead(sensorPin);
}


// METHODS FOR DIMMING AND BRIGHTEN
void brightenTo(int desired) {
  int preSensorValue = readSensorPin();

  // curtains up
  motorStep(600);
  
  int postSensorValue = readSensorPin();

  // Only runs if the curtains has an effect
  if ((postSensorValue - preSensorValue)>1) { // Virker ikke optimalt
    while (sensorValue < desired && stepCount+stepsPerRevolution < curtainUpperbound) {
      motorStep(stepsPerRevolution);
      updateSensor();
    }
  }
  
  while (sensorValue < desired && lightValue < 255) {
    changeLight(lightValue+5);
    delay(50);
    updateSensor();
  }
  
}

void dimTo(int desired) {
  while (sensorValue > desired && lightValue > 4) {
    changeLight(lightValue-5);
    delay(50);
    updateSensor();
  }
  while (sensorValue > desired && stepCount-stepsPerRevolution > curtainLowerbound) {
    motorStep(-stepsPerRevolution);
    updateSensor();
  }
} 

// Run this
void updateSensor() {
  sensorValue = analogRead(sensorPin);
}

// Alle steder hvor lys ændres, skal være her
void changeLight(int value) {
  analogWrite(ledPin, value);
  lightValue = value;
}

// Run this
void motorStep(int value) {
  if(stepCount+value < curtainUpperbound) {
    myStepper.step(value);
    stepCount+=value;
  }

  writeToEEPROM(String(stepCount));
}

void writeToEEPROM(String str) {
  int len = String(str).length();
  EEPROM.write(0, len);

  for (int i = 0; i < len ; i++) {
    EEPROM.write(0+1+i, str[i]);
  }
}

String readFromEEPROM() {
  int len = EEPROM.read(0);

  char data[len +1];
  for (int i = 0 ; i < len ; i++) {
    data[i] = EEPROM.read(0 + 1 + i);
  }
  data[len] = '\0';
  return String(data);
}
