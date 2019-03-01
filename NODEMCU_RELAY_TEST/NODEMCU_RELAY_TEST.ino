/*
Автор: Сергей Галочкин
email: decole@rambler.ru
Данный скетч для NodeMCU.
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int Relay1 = 16;// D0
int Relay2 = 5; // D1
int Relay3 = 4; // D2
int Relay4 = 2; // D3
int Relay5 = 14;// D5
int Woter1 = 12;// D6

// Update these with values suitable for your network.

const char* ssid = "DECOLE-WIFI";
const char* password = "A9061706210";
const char* mqtt_server = "192.168.1.5";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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
  String topicValue = "";
  for (int i = 0; i < length; i++) {
    topicValue += (char)payload[i];
  }
  if(topic = "water/trans"){
    if(String(topicValue).indexOf("on") >= 0) {
      Serial.println("water/trans on");
      topicValue = "";      
    }
    else if(String(topicValue).indexOf("off") >= 0) {
      Serial.println("water/trans off");
      topicValue = "";  
    }
  }
  if(topic = "water/major"){
    if(String(topicValue).indexOf("on") >= 0) {
      Serial.println("water/major on");
      topicValue = "";      
    }
    else if(String(topicValue).indexOf("off") >= 0) {
      Serial.println("water/major off");
      topicValue = "";  
    }
  }
  if(topic = "water/1"){
    if(String(topicValue).indexOf("on") >= 0) {
      Serial.println("water/1 on");
      topicValue = "";      
    }
    else if(String(topicValue).indexOf("off") >= 0) {
      Serial.println("water/1 off");
      topicValue = "";  
    }
  }
  if(topic = "water/2"){
    if(String(topicValue).indexOf("on") >= 0) {
      Serial.println("water/2 on");
      topicValue = "";      
    }
    else if(String(topicValue).indexOf("off") >= 0) {
      Serial.println("water/2 off");
      topicValue = "";  
    }
  }
  if(topic = "water/3"){
    if(String(topicValue).indexOf("on") >= 0) {
      Serial.println("water/3 on");
      topicValue = "";      
    }
    else if(String(topicValue).indexOf("off") >= 0) {
      Serial.println("water/3 off");
      topicValue = "";  
    }
  }
  if(topic = "water/alarm"){
    if(String(topicValue).indexOf("1") >= 0) {
      Serial.println("alarm leakage!");
      topicValue = "";      
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()), "esp", "esp99669966q") {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("water/major");
      client.subscribe("water/1");
      client.subscribe("water/2");
      client.subscribe("water/3");
      client.subscribe("water/trans");
      client.subscribe("water/alarm");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Woter1, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  long now = millis();

  if (now - lastMsg > 10000) {
    client.publish("water/major/check", String(digitalRead(Relay1)).c_str(), true);
    client.publish("water/1/check",     String(digitalRead(Relay2)).c_str(), true);
    client.publish("water/2/check",     String(digitalRead(Relay3)).c_str(), true);
    client.publish("water/3/check",     String(digitalRead(Relay4)).c_str(), true);
    client.publish("water/trans/check", String(digitalRead(Relay5)).c_str(), true);
    client.publish("water/leakage",     String(digitalRead(Woter1)).c_str(), true);
  }
}
