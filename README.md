# Создание умного дома
### Данный умный дом основан на микроконтроллере ESP8266 и микрокомпьютере raspberry pi 


<a href=https://www.buymeacoffee.com/alexandrlyachov" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-blue.png" alt="Buy Me A Coffee" style="height: 30px !important;width: 108px !important;" ></a>


# ESP8266

  - Для использования esp8266 нам понадобиться скачать библиотеки (ESP8266, Pubclient)
- И любые внешние датчики 
Код для микроконтроллере в файле main.cpp  
# Raspberry pi

Настройка raspberry pi состоит в том что бы: 
* Подключить его к той же сети что и esp8266 
* Установить на него [Node-red](https://nodered.org/)
* Со всевозможными пакетами для голосового помошника [Алисы](https://flows.nodered.org/node/node-red-contrib-yandex-alice-command)

### Использование
#### Код на esp8266
Библиотеки
```sh
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
```
Подключение к локальной сети 

```sh
const char* ssid = "Имя сети";
const char* password = "Пароль сети";
```
Подключение к Raspberry pi 
```sh
const char* mqtt_server = "IP Raspberry pi ";
```
##### Для просмотра ip raspberry pi в локально сети используют команду
#
```sh
hostname -I
```
##### Создание клиента сервера
#
```sh
WiFiClient espClient;
PubSubClient client(espClient);
```
##### Подключаем внешнее устройство 
###### В моем случае лампу
#
```sh
const int lamp = 4;
```
##### Создаем таймер для проверки состояния 
#
```sh
long now = millis();
long lastMeasure = 0;
```
#### Создаем функции для WiFi
###### Создаем функцию для подключения к WiFi
#
```sh
void setup_wifi() 
```
###### Создаем функцию для повторного подключения к WiFi
#
```sh
void reconnect()
``` 
Так же в этой фунции реализованна подписка на переменную лампы
```sh
  Serial.println("connected");  
  client.subscribe("room/lamp");
``` 
##### Присваивание esp8266 локального ip и порта node-red
###### В функции void setup()
```sh
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 ```
###### В функции void loop()
В ней у нас происходит постоянная проверка функции подключение и если она не работает то включается переподключение
```sh
  if (!client.connected()) {
    reconnect();
  }
 ```
