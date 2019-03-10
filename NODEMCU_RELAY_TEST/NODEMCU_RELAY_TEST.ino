/*
Автор: Сергей Галочкин
email: decole@rambler.ru
Данный скетч для NodeMCU (Arduino IDE)
4 электроклапана работают через 4 реле

1 реле - включает блок питания и главный клапан, без этого включенного топика ни один из электроклапанов полива не будет работать (ИП отключен)
2 реле - включает клапан полива 1
3 реле - включает клапан полива 2
4 реле - включает клапан полива 3

При топике water/alarm отключаются все клапаны
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int Relay1 = 16;// D0
int Relay2 = 5; // D1
int Relay3 = 4; // D2
int Relay4 = 2; // D4
int Woter1 = 12;// D6

// Update these with values suitable for your network.
const char* ssid = "DECOLE-WIFI";
const char* password = "A9061706210";
const char* mqtt_server = "192.168.1.5";
long lastMsg = 0;
char msg[50];

WiFiClient espClient;
PubSubClient client(espClient);


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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {  
  // Allocate the correct amount of memory for the payload copy
  byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer
  memcpy(p,payload,length);
  
 // в топиках передавать 1 или 0
  if(String(topic).indexOf("water/major")>= 0){
    if(p[0] == '0') {
      digitalWrite(Relay1, HIGH);    
    }
    else if(p[0] == '1') {
      digitalWrite(Relay1, LOW);
    }
  }
  if(String(topic).indexOf("water/1")>= 0){
    if(p[0] == '0') {
      digitalWrite(Relay2, HIGH);    
    }
    else if(p[0] == '1') {
      digitalWrite(Relay2, LOW); 
    }
  }
  if(String(topic).indexOf("water/2")>= 0){
    if(p[0] == '0') {
      digitalWrite(Relay3, HIGH); 
    }
    else if(p[0] == '1') {
      digitalWrite(Relay3, LOW); 
    }
  }
  if(String(topic).indexOf("water/3")>= 0){
    if(p[0] == '0') {
      digitalWrite(Relay4, HIGH);    
    }
    else if(p[0] == '1') {
      digitalWrite(Relay4, LOW); 
    }
  }
  if(String(topic).indexOf("water/alarm")>= 0){
    if(p[0] == '1') {
      // оключаем все реле
      digitalWrite(Relay1, HIGH); 
      digitalWrite(Relay2, HIGH);
      digitalWrite(Relay3, HIGH);
      digitalWrite(Relay4, HIGH); 
    }
  }
  free(p);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Создаем рандомный client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Пробуем подключиться
    if (client.connect(clientId.c_str()), "esp", "esp99669966q") {
      Serial.println("connected");
      // После конекта отправка тестового сообщения и подписка на топики
      client.publish("outTopic", "hello world");
      // ... и подписываемся
      client.subscribe("water/major");
      client.subscribe("water/1");
      client.subscribe("water/2");
      client.subscribe("water/3");
      client.subscribe("water/alarm");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Ждем 5 секунд и запускаем еще раз
      delay(5000);
    }
  }
}

void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Woter1, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  digitalWrite(Relay1, HIGH); 
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  long now = millis();

  if (now - lastMsg > 10000) {
    client.publish("water/check/major", String(digitalRead(Relay1)).c_str(), true);
    client.publish("water/check/1",     String(digitalRead(Relay2)).c_str(), true);
    client.publish("water/check/2",     String(digitalRead(Relay3)).c_str(), true);
    client.publish("water/check/3",     String(digitalRead(Relay4)).c_str(), true);
    client.publish("water/leakage",     String(digitalRead(Woter1)).c_str(), true);
  }
}
