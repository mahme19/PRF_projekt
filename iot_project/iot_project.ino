//#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <Stepper.h>


//the initial wifi status
int status = WL_IDLE_STATUS;

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
const int ledPin2 = 7;

int i = 0;

// LDR sensor pin

const int sensorPin = A0;

int sensorValue = 0;

String latestCommand = "";

int stepCount = 0;         // number of steps the motor has taken



void setup() {
  // put your setup code here, to run once:


  
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
 // pinMode(STEPPER_PIN_1, OUTPUT);
//  pinMode(STEPPER_PIN_2, OUTPUT);
  // pinMode(STEPPER_PIN_3, OUTPUT);
  // pinMode(STEPPER_PIN_4, OUTPUT);

    myStepper.setSpeed(60);


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

  int sensorVal = readSensorPin();
  Serial.println(sensorVal);

  //analogWrite(ledPin, 255);

  //setLightning("total_darkness");
  // put your main code here, to run repeatedly:åå
 // latestCommand = listenForUDPMessage();


  // setLightning(latestCommand);

/*

  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);


  analogWrite(ledPin, 0);
  analogWrite(ledPin2, 0);


  Serial.println("OFF");
  delay(2000);

  analogWrite(ledPin, 5);
  Serial.println("ON 160 ");

  delay(2000);

  analogWrite(ledPin, 255);
  Serial.println("ON 255");
  delay(2000);
  */
 
  

/* if(i < 4){
    Serial.println("clockwise");
    myStepper.step(stepsPerRevolution );
    delay(2000);
    i++;
  }
  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution );
  delay(2000);

*/

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
