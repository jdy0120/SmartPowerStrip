/*
이지탭
아두이노 소스
*/


#include <SPI.h>
#include "WizFi250.h"

char ssid[] = "HyunPil_V20";            // your network SSID (name)
char pass[] = "45685213";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "203.234.48.128";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

// Initialize the WiFi client object
WiFiClient client;

void httpRequest();
void printWifiStatus();

int relay1 = 5;
int relay2 = 6;
int relay3 = 9;

String currentLine = "";
String FirstString = "";
String SecondString = "";
String ThirdString = "";
String compare = " 1";

boolean readingFirst = false;
boolean readingSecond = false;
boolean readingThird = false;

int temp = 0;

void setup()
{

  currentLine.reserve(100);
  FirstString.reserve(10);
  SecondString.reserve(10);
  ThirdString.reserve(10);
  
  // initialize serial for debugging
  Serial.begin(115200);

  pinMode(relay1,OUTPUT);
  digitalWrite(relay1,LOW);

  pinMode(relay2,OUTPUT);
  digitalWrite(relay2,LOW);

  pinMode(relay3,OUTPUT);
  digitalWrite(relay3,LOW);
  
  WiFi.init();

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

  printWifiStatus();
}

void loop()
{
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    currentLine += c;

    if(c == '\n'){
      //줄바꿈 문자열이 전송되면 데이터를 보내지 않는다.
      currentLine = "";
     }

    if(currentLine.endsWith("First : ")){
      //현재 스트링이 "First"로 끝났다면 첫번째 릴레이 스위치 상태를 받을 준비가 되었다.
      readingFirst = true;
      FirstString = "";
     }

     if(readingFirst){
      if(c != '<'){
        FirstString += c;
       }
       else{
        readingFirst = false;

        Serial.print("-First: ");
        Serial.print(FirstString);
        
        if(FirstString.equals(compare)){
            digitalWrite(relay1 , HIGH);
        }else{
            digitalWrite(relay1 , LOW);
        }
       }
      }


    if(currentLine.endsWith("Second : ")){
      //현재 스트링이 "Second"로 끝났다면 첫번째 릴레이 스위치 상태를 받을 준비가 되었다.
      readingSecond = true;
      SecondString = "";
     }

     if(readingSecond){
      if(c != '<'){
        SecondString += c;
       }
       else{
        readingSecond = false;

        Serial.print("-Second: ");
        Serial.print(SecondString);

        if(SecondString.equals(compare)){
          digitalWrite(relay2,HIGH);
        }else{
          digitalWrite(relay2,LOW);
        }
       }
      }

          if(currentLine.endsWith("Third : ")){
      //현재 스트링이 "Third"로 끝났다면 첫번째 릴레이 스위치 상태를 받을 준비가 되었다.
      readingThird = true;
      ThirdString = "";
     }

     if(readingThird){
      if(c != '<'){
        ThirdString += c;
       }
       else{
        readingThird = false;

        Serial.print("-Third: ");
        Serial.print(ThirdString);

        if(ThirdString.equals(compare)){
          digitalWrite(relay3,HIGH);
        }else{
          digitalWrite(relay3,LOW);
        }
       }
      }
  }

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}

// this method makes a HTTP connection to the server
void httpRequest()
{
  Serial.println();

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");

    // send the HTTP PUT request
    client.println(F("GET /list.php HTTP/1.1"));
    client.println(F("Host: 203.234.48.128"));
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
