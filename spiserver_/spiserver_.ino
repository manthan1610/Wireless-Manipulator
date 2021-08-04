#define SELPIN 15 //Selection Pin 
#define DATAOUT 13//MOSI 
#define DATAIN  12//MISO 
#define SPICLOCK  14//Clock 
#include <ESP8266WiFi.h>
const char *ssid = "Pomiary";
const char *pass = "pomiar_mocy";
const int httpPort = 80;
int x;int i=0;
int  token= 1;                    //number of measuring device to ask
WiFiServer server(httpPort);
String req="";
int z=0;
int readvalue1,readvalue2; 
int readvalue[4];

void setup(){ 
 //set pin modes 
 delay(100);
  Serial.begin(115200);
  Serial.println("Configuring AP");
  WiFi.mode(WIFI_AP);
  boolean result = WiFi.softAP(ssid,pass);
  if(result == true) Serial.println("Ready");
  else Serial.println("Failed!");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");

  delay(1000);
 pinMode(SELPIN, OUTPUT); 
 pinMode(DATAOUT, OUTPUT); 
 pinMode(DATAIN, INPUT); 
 pinMode(SPICLOCK, OUTPUT); 
 //disable device to start with 
 digitalWrite(SELPIN,HIGH); 
 digitalWrite(DATAOUT,LOW); 
 digitalWrite(SPICLOCK,LOW); 
} 

int read_adc(int channel){
  int adcvalue = 0;
  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)

  //allow channel selection
  commandbits|=((channel-1)<<3);

  digitalWrite(SELPIN,LOW); //Select adc
  // setup bits to be written
  for (int i=7; i>=3; i--){
    digitalWrite(DATAOUT,commandbits&1<<i);
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);    
  }

  digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
  digitalWrite(SPICLOCK,LOW);
  digitalWrite(SPICLOCK,HIGH);  
  digitalWrite(SPICLOCK,LOW);

  //read bits from adc
  for (int i=11; i>=0; i--){
    adcvalue+=digitalRead(DATAIN)<<i;
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }
  digitalWrite(SELPIN, HIGH); //turn off device
  return adcvalue;
}


void loop() { 
WiFiClient client = server.available();
 if  (client.connected())
 { 
 client.flush();
  readvalue[i] = read_adc(1+i); 
if(i==0){if(readvalue[i]>=448&&readvalue[i]<=3812){readvalue[i]=map(readvalue[i],3812,448,0,180);}}
else if(i==1){if(readvalue[i]>=0&&readvalue[i]<=2600){readvalue[i]=map(readvalue[i],0,2600,181,361);}}
else if(i==2) {readvalue[i]=map(readvalue[i],7,4095,542,812);}
else if(i==3){readvalue[i]=map(readvalue[i],7,4095,813,1083);}

 client.println(readvalue[i]);
Serial.println(readvalue[i]); 
if(i==3){i=0;}
else {i++;}
delay(40);
} 

}
 
