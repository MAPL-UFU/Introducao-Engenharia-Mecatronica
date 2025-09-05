#include <EWiFi.h>
#include <PubSubClient.h>
#include <WiFiPassword.h>
#include <CLPIoT.h>

/* DECLARATION OF OBJECTS AND ATRIBUTES OF A CLASS */
WiFiClient espClient;
PubSubClient MQTT(espClient);

/* DECLARATION OF GLOBAL VARIABLES */
String status = "";
volatile bool flag = false;

void setup() {
  Serial.begin(115200);
  clpiot.begin();

  do {
    if(flag) Serial.println("\n\nConnection error, trying again.\n");

    if (password != "XXXX")
      ewifi.setWiFi(SSID, password);
    else
      ewifi.setWiFi(SSID, WPA2_AUTH_PEAP, anonymous, username, userpassword);
    ewifi.connect();

    flag = true;
  } while(ewifi.status() != WL_CONNECTED);

  // Setup of Server
  MQTT.setServer(MQTTServer, MQTTPort);
  MQTT.setCallback(callback);

  pinMode(OUTPUT_1, OUTPUT);
}

void loop() {
  if (!MQTT.connected()) conectarMQTT();

  MQTT.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for(int i = 0; i < length; i++) msg += (char) payload[i];

  printf("Tópico: %s \tMensagem: %s", topic, msg);
  
  if (status != msg) {
    status = msg;

    msg.toLowerCase();

    if (msg == "on") {
      clpiot.digitalW(OUTPUT_1, HIGH);
      clpiot.print(0, "LED ON", "Center");
    }
    if(msg == "off") {
      clpiot.digitalW(OUTPUT_1, LOW);
      clpiot.print(0, "LED OFF", "Center");
    }

    msg.toUpperCase();

    if (msg == "ON" || msg == "OFF") MQTT.publish(topicStatus, msg.c_str());
  }
}

/* MQTT FUNCTION FOR CONECT TO SERVER */
void conectarMQTT() {
  int num = 20;
  char esp_id[num];

  snprintf(esp_id, num, "ESP32-%s", ewifi.getmacAddress());
  // printf("%s\n", esp_id);
  while (!MQTT.connected())
    if (MQTT.connect(esp_id)) MQTT.subscribe(topicLed);
}