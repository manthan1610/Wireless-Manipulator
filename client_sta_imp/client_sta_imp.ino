#include <ESP8266WiFi.h>
#include<Servo.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
const char* ssid = "Pomiary";
const char* pass = "pomiar_mocy";
const int Port = 80;
IPAddress host(192,168,4,1);
WiFiClient client;                          
int z=0;
String req;
int pos,pos1;
void setup() 
{
 myservo1.attach(0) ;myservo2.attach(2) ; myservo3.attach(4) ; myservo4.attach(5) ; myservo5.attach(12) ; 
  delay(10); 
  Serial.begin(115200);
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);                      
  WiFi.begin(ssid, pass);                    
  while (WiFi.status() != WL_CONNECTED) {    
    delay(5); 
 //   Serial.print(".");
  }
  
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());             
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
     Serial.println("reconnecting");
    WiFi.begin(ssid, pass);}
    while (WiFi.status() != WL_CONNECTED) {
      delay(50);
      //Serial.print(".");
      }
    conn:  
    client.connect(host,Port);
    client.flush();
while(!client.connected()){
    client.flush();  
    goto conn;
 //   Serial.println("\nCoonection failed\n");
  }
client.flush();
if(client.connected()){
  String k=client.readStringUntil('\r');
  int pos=k.toInt();
  if(pos>=0&&pos<=180)
  {
    Serial.print("Servo 1 angle is ");Serial.println(pos);myservo1.write(pos);
    pos1=map(pos,0,180,180,0);myservo2.write(pos1);
  }
  else if(pos>=181&&pos<=361)
  {
    Serial.print("Servo 2 angle is ");pos=map(pos,181,361,0,180);Serial.println(pos);myservo3.write(pos);
  }
  else if(pos>=542&&pos<=812)
  {
    Serial.print("Servo 3 angle is ");pos=map(pos,542,812,0,270);Serial.println(pos);myservo4.write(pos);
    }
    else if(pos>=813&&pos<=1083)
  {
    Serial.print("Servo 4 angle is ");pos=map(pos,813,1083,0,270);Serial.println(pos);myservo5.write(pos);
    }
    
  client.flush();
  delay(10);
  z++;}
  
    
}
