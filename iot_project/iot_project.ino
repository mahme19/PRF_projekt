//#include <SPI.h>
//#include <WiFiNINA.h>
//#include <WiFiUdp.h>

//the initial wifi status
//int status = WL_IDLE_STATUS;

//WiFiUDP object used for the communication
//WiFiUDP Udp;

//your network name (SSID) and password (WPA)
char ssid[] = "F23PRF";            
char pass[] = "15FEB2023"; 

//local port to listen on
int localPort = 3002;                               

//IP and port for the server
//IPAddress serverIPAddress(192, 168, 110, 214);
int serverPort = 3001;       



// LED pins
const int ledPin = 6;
const int ledPin2 = 7;






void setup() {
  // put your setup code here, to run once:




  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  Serial.begin(9600);

  /*while (!Serial);

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
  // put your main code here, to run repeatedly:
  analogWrite(ledPin, 0);
  analogWrite(ledPin2, 0);


  Serial.println("OFF");
  delay(2000);

  analogWrite(ledPin, 1);
  Serial.println("ON 160 ");

  delay(2000);

  analogWrite(ledPin, 255);
  Serial.println("ON 255");
  delay(2000);
}


//listens for incoming UDP messages
/*
void listenForUDPMessage() {

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
    int messageValueAsInt = atoi(packetBuffer);

    //set LED to received value
    //setLEDTo(messageValueAsInt);

    //send acknowledgement message
    //sendUDPMessage(Udp.remoteIP(), Udp.remotePort(), "ARDUINO: message was received");
  }
  
}
*/
