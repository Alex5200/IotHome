
//  add Libery 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//  end
//  add peremennie dlya wifi 
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
//  end
//  connect server Raspberry pi
const char* mqtt_server = "REPLACE_WITH_YOUR_RPI_IP_ADDRESS";
//  end
//  ad client for wifi server 
WiFiClient espClient;
PubSubClient client(espClient);
// end
//      pin controll relay
const int relay = 4;
// end
//      reload timer
long now = millis();
long lastMeasure = 0;
// end
//    setimgs for connect Wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
//      End function setup Wifi
//      check pocket for Raspberry pi
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if(topic=="room/relay"){ // recuest data releay
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(relay, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(relay, LOW);
        Serial.print("Off");
      }
  }
  Serial.println();
}
//      End  check pocket for Raspberry pi
//      Wifi reconect 
void reconnect_server_raspberryPi() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
   
      client.subscribe("room/relay");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
//      End  Wifi reconect 
//      Setup pins and serial port
void setup() {
  pinMode(relay, OUTPUT); // pin out for controll relay
  Serial.begin(115200); // setings monitor
  setup_wifi(); // setups wifi
  client.setServer(mqtt_server, 1883); //connct to raspberry pi server
  client.setCallback(callback); // send message
}
//End  Setup pins and serial port

//      loop
void loop() { 
  if (!client.connected()) {
    reconnect_server_raspberryPi();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
  now = millis();
  if (now - lastMeasure > 30000) {
    lastMeasure = now;
    client.getCallback(callback());
  }
}
//      End loop
