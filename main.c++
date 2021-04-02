
//  Блок инициализации библиотеки
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//  end
//  Блик инициализации переменных настройки wifi 
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
//  end
//  Блок инициализации подключения к Raspberry pi
const char* mqtt_server = "REPLACE_WITH_YOUR_RPI_IP_ADDRESS";
//  end
//  Зарегистрировать подключенное устройствоq
WiFiClient espClient;
PubSubClient client(espClient);
// end
//      Добавление переменной пина лампы 
const int lamp = 4;
// end
//      Переменные для создания переменных обновления
long now = millis();
long lastMeasure = 0;
// end
//      Функция настройки Wifi
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
//      End Функции Wifi
//      Функция приема сообщений с сервера Raspberry pi
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

  if(topic=="room/lamp"){ // Проверка состояния переключения
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
//      End функций сообщений сервера
//      Функция переподключения к серверу 
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
//      End функции переподключения к серверу 
//      Функция инициализации 
void setup() {
  pinMode(lamp, OUTPUT); // инициализация пина
  Serial.begin(115200); // вывод данных в монитор порта
  setup_wifi(); // Подключение к wifi
  client.setServer(mqtt_server, 1883); // Подключение к серверу raspberry pi
  client.setCallback(callback); // проверку состояния
}
//End функций инициализаций

//      Цикличная функция
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
//      End функции цикла
