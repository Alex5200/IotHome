
//  ���� ������������� ����������
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//  end
//  ���� ������������� ���������� ��������� wifi 
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
//  end
//  ���� ������������� ����������� � Raspberry pi
const char* mqtt_server = "REPLACE_WITH_YOUR_RPI_IP_ADDRESS";
//  end
//  ���������������� ������������ ����������q
WiFiClient espClient;
PubSubClient client(espClient);
// end
//      ���������� ���������� ���� ����� 
const int lamp = 4;
// end
//      ���������� ��� �������� ���������� ����������
long now = millis();
long lastMeasure = 0;
// end
//      ������� ��������� Wifi
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
//      End ������� Wifi
//      ������� ������ ��������� � ������� Raspberry pi
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

  if(topic=="room/lamp"){ // �������� ��������� ������������
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(lamp, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(lamp, LOW);
        Serial.print("Off");
      }
  }
  Serial.println();
}
//      End ������� ��������� �������
//      ������� ��������������� � ������� 
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
   
      client.subscribe("room/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
//      End ������� ��������������� � ������� 
//      ������� ������������� 
void setup() {
  pinMode(lamp, OUTPUT); // ������������� ����
  Serial.begin(115200); // ����� ������ � ������� �����
  setup_wifi(); // ����������� � wifi
  client.setServer(mqtt_server, 1883); // ����������� � ������� raspberry pi
  client.setCallback(callback); // �������� ���������
}
//End ������� �������������

//      ��������� �������
void loop() { 
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
  now = millis();
  if (now - lastMeasure > 30000) {
    lastMeasure = now;
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    float hic = dht.computeHeatIndex(t, h, false);
    static char temperatureTemp[7];
    dtostrf(hic, 6, 2, temperatureTemp);
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);
}
//      End ������� �����
