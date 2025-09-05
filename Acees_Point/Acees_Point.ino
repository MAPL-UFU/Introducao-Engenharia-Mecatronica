#include <EWiFi.h>
#include <PubSubClient.h>
#include <WiFiPassword.h>
#include <CLPIoT.h>

#if 0
  #include <SPI.h>
  #include <PN532_SPI.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

  PN532_SPI pn532spi(SPI, 10);
  NfcAdapter nfc = NfcAdapter(pn532spi);
  PN532 tag(pn532spi);
#elif 1
  #define NDEF_DEBUG

  #include <PN532_HSU.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

  PN532_HSU pn532hsu(16, 17);
  NfcAdapter nfc(pn532hsu);
  PN532 tag(pn532hsu);
#else
  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

  PN532_I2C pn532_i2c(Wire);
  NfcAdapter nfc = NfcAdapter(pn532_i2c);
  PN532 tag(pn532_i2c);
#endif

/* DECLARATION OF A OBJECT */
WiFiClient espClient;
PubSubClient MQTT(espClient);
TaskHandle_t Task1;

/* DECLARATION OF GLOBAL VARIABLES */
String tagId;
volatile bool flag = false;

void setup(void) {
  Serial.begin(115200);
  clpiot.begin();
  nfc.begin();
  tag.begin();

  do {
    if(flag) Serial.println("\n\nConnection error, trying again.\n");

    if (password != "XXXX")
      ewifi.setWiFi(SSID, password);
    else
      ewifi.setWiFi(SSID, WPA2_AUTH_PEAP, anonymous, username, userpassword);
    ewifi.connect();

    flag = true;
  } while(ewifi.status() != WL_CONNECTED);
  
  uint32_t versiondata = tag.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Modulo PN53x não encontrado");
    while (1); // Halt
  }
  // Got valid data, print it out!
  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); 
  Serial.println((versiondata >> 8) & 0xFF, DEC);
  tag.SAMConfig();
  
  // Setup of Server
  MQTT.setServer(MQTTServer, MQTTPort);
  MQTT.setCallback(callback);

  pinMode(OUTPUT_1, OUTPUT);

  clpiot.print(0, "Acesso LEM4", "center");

  // Setup a Task using the free core of ESP32
  int CORE_ID = int(!xPortGetCoreID());
  xTaskCreatePinnedToCore(
    Task1code, // Task function
    "Task1",   // Name of the Task to atach
    10000,     // Size to alocated to Task (Word or Byte)
    NULL,      // Task argument to be passed (void*)
    0,         // Priority of the Task [0 a 25]
    &Task1,    // Task ID
    CORE_ID);  // Task core ID (0 ou 1)

  delay(500);
}

/* TASK FUNCTION USING FREE CORE OF ESP32 */
void Task1code(void* pvParameters) {
  Serial.print("Conexões (Wi-Fi e MQTT) rodando no nucleo ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    if (!MQTT.connected()) conectarMQTT();

    MQTT.loop();
  }
}

void loop() {
  getTagID();
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for(int i = 0; i < length; i++) msg += (char) payload[i];

  printf("Tópico: %s \tMensagem: %s", topic, msg);
  
  msg.toLowerCase();

  if (msg == "on") {
    clpiot.digitalW(OUTPUT_1, HIGH);
    clpiot.print(1, "Liberado", "Center");
    MQTT.publish(topicStatus, "ON");
  }
  
  if (msg == "on" || msg == "off") {
    delay(1000);
    clpiot.digitalW(OUTPUT_1, LOW);
    MQTT.publish(topicStatus, "OFF");
  }
}

void getTagID() {
  clpiot.print(1, "Insira uma TAG", "center");

  while (!nfc.tagPresent());

  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  tagId = "";

  tag.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  for (uint8_t i = 0; i < uidLength; i++) tagId += String(uid[i], HEX);
  tagId.toUpperCase();

  delay(1000);

  MQTT.publish(topicTagId, tagId.c_str());
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