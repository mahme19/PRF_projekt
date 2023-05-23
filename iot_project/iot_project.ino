//#include <SPI.h>
//#include <WiFiUdp.h>
#include <WiFiNINA.h>
#include <Stepper.h>
#include <EEPROM.h>
#include <ArduinoMqttClient.h>



//the initial wifi status
//int status = WL_IDLE_STATUS;

//WiFiUDP object used for the communication
//WiFiUDP Udp;

//Wifi name and password
char ssid[] = "iPhone";    // your network SSID (name)
char pass[] = "netdel100";    // your network password (use for WPA, or use as key for WEP)

//Wifi client
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

//local port to listen on
int localPort = 3002;                               

//IP and port for the server
IPAddress serverIPAddress(192, 168, 110, 214);
int serverPort = 3001;       


#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11
int step_number = 1;

const int stepsPerRevolution = 300;

Stepper myStepper = Stepper(stepsPerRevolution, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);

// MQTT 
const char broker[] = "broker.hivemq.com"; 
int        port     = 1883;

const char LDR_sensor_topic[] = "PRF/2023/LDR";
const char STEPPER_motor_topic[] = "PRF/2023/STEPPER";
const char LIGHT_level_topic[] = "PRF/2023/LIGHT";

const char COMMANDS_topic[] = "PRF/2023/COMMANDS";

// LED pins
const int ledPin = 6;

int i = 0;

// LDR sensor pin
const int sensorPin = A0;
int sensorValue = 0;

int ledValue = 0;

String latestCommand = "";

int stepCount = 9000; // number of steps the motor has taken
int curtainUpperbound = 9000;
int curtainLowerbound = 0;

void setup() {

  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);

  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, ledValue);

  myStepper.setSpeed(100);
  stepCount = readFromEEPROM().toInt();

  Serial.begin(9600);

  //Wifi network:
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(5000);
  }
  Serial.println("Network connection established");


  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (true);
  }
  Serial.println("MQTT connection established");
  
  //Subscribe to MQTT topic and set the callback function for handling messages
  mqttClient.subscribe(COMMANDS_topic);
  mqttClient.onMessage(onMqttMessage);

  // Check LDR sensor
  updateSensor();

}

void onMqttMessage(int messageSize) {
  String message;
  int value;

  while (mqttClient.available()) {
    message += (char)mqttClient.read();
  }
  value = message.toInt();
  setLightning(value);
}

int previousTime = 0;  // To store execution time
int interval = 10000; 

void loop() {
  //Pools the MQTT broker and avoids disconnecting
  mqttClient.poll();
  
  int currentTime = millis();  // Get the current time

  // Check if the desired interval has passed
  if (currentTime - previousTime >= interval) {
    sendSensorToMQTT();
    previousTime = currentTime;  // Update the previous execution time
  }
}

void setLightning(int value) {

  updateSensor();
  if (sensorValue > value) {
    dimTo(value);
  } else {
    brightenTo(value);
  }
}


int readSensorPin(){
  return analogRead(sensorPin);
}


// METHODS FOR DIMMING AND BRIGHTEN
void brightenTo(int desired) {
  //int preSensorValue = readSensorPin();

  // curtains up
  //motorStep(600);
  
  //int postSensorValue = readSensorPin();

  // Only runs if the curtains has an effect
  //if ((postSensorValue - preSensorValue)>1) { // Virker ikke optimalt
    while (sensorValue < desired && stepCount+stepsPerRevolution < curtainUpperbound) {
      motorStep(stepsPerRevolution);
      updateSensor();
    }
  //}
  Serial.println("should we dim lights?");
  while (sensorValue < desired && ledValue < 255) {
    Serial.println("YES!");
    changeLED(ledValue+5);
    delay(50);
    updateSensor();
  }
  
}

void dimTo(int desired) {

  while (sensorValue > desired && ledValue > 4) {
    changeLED(ledValue-5);
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
void changeLED(int value) {
  analogWrite(ledPin, value);
  ledValue = value;
  publishToMQTT(LIGHT_level_topic, String(ledValue));
}

// Positiv step = op | Negativ step = ned
void motorStep(int value) {
  if(stepCount+value < curtainUpperbound) {
    myStepper.step(value);
    stepCount+=value;
  }
  publishToMQTT(STEPPER_motor_topic ,String(stepCount));
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

void publishToMQTT(String topic, String message) {
  mqttClient.beginMessage(topic);
  mqttClient.print(String(message));
  mqttClient.endMessage();
}

void sendSensorToMQTT() {
  updateSensor();
  mqttClient.beginMessage("PRF/2023/LDR");
  mqttClient.print(sensorValue);
  mqttClient.endMessage();
}
