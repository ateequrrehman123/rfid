#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <PubSubClient.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <XTronical_ST7735.h>
#include <SPI.h>

#define buzzerpin  3  //GPIO3
#define redled  21 //GPIO1
#define grnled  22  //GPIO22

#define VSPI_MISO   19
#define VSPI_MOSI   23
#define VSPI_SCLK   18
#define VSPI_SS     5
#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCLK   14
#define HSPI_SS     15
#define RST_PIN  0  // D3
#define TFT_DC   25  //D25
#define TFT_RST  26  //D26

#define WIFI_TIMEOUT 1000
#define STA_SSID "Redmi Note 11"
#define STA_PASS "87654321"

Adafruit_ST7735 tft = Adafruit_ST7735(HSPI_SS, TFT_DC, TFT_RST);

void clear_screen();
void set_screen();
void show_employ_scrn();
void show_op_scrn();
//void popup_true_scrn();
//void popup_false_scrn();

const char* mqttServer = "20.198.114.109";        // Broker address
const char *ID        = "0001";  // Name of our device, must be unique



String tag;
long lastMsg = 0;
char msg[50];

MFRC522 rfid(VSPI_SS, RST_PIN);    // Instance/object of the class
MFRC522::MIFARE_Key key;            //object/instance of class MFRC522
/*This function will fetch data from MQTT server, when ever data is available*/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
}
StaticJsonDocument<1024> doc; // Adjust the buffer size based on your JSON size
DeserializationError error = deserializeJson(doc, payload);

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}
Serial.println("Received JSON payload:");
Serial.println((char*)payload);

bool status = doc["status"]; // false
bool popup = doc["popup"]; // false
JsonArray data = doc["data"];
const char* data_0_dispText = data[0]["dispText"]; // "Employee"
const char* data_0_value = data[0]["value"]; // "123: Usman"
const char* data_1_dispText = data[1]["dispText"]; // "Operation"
const char* data_1_value = data[1]["value"]; // "None"
const char* data_2_dispText = data[2]["dispText"]; // "Style"
const char* data_2_value = data[2]["value"]; // "None"
const char* data_3_dispText = data[3]["dispText"]; // "BundlePCs"
int data_3_value = data[3]["value"]; // 30
const char* data_4_dispText = data[4]["dispText"]; // "Total PCs"
int data_4_value = data[4]["value"]; // 30
const char* data_5_dispText = data[5]["dispText"]; // "Efficiency"
const char* data_5_value = data[5]["value"]; // "20%"
const char* data_true = doc["data"];
  if(popup){
    //popup_true_scrn();
    if(status){
  clear_screen();
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 0);
  tft.print(data_true);
  digitalWrite(buzzerpin, HIGH);
      digitalWrite(grnled, HIGH);
      delay(2000);
      digitalWrite(buzzerpin, LOW);
      digitalWrite(grnled, LOW);

  delay(3000);
    }else{
      clear_screen();
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 0);
  tft.print("not authenticate");
digitalWrite(buzzerpin, HIGH);
      digitalWrite(redled, HIGH);
      delay(500);
      digitalWrite(buzzerpin, LOW);
      digitalWrite(redled, LOW);
delay(500);
      digitalWrite(buzzerpin, HIGH);
      digitalWrite(redled, HIGH);
      delay(500);
      digitalWrite(buzzerpin, LOW);
      digitalWrite(redled, LOW);
    }
    

    Serial.print(data_true);
  }else{
    //processJson(doc.as<JsonObject>());
   // popup_false_scrn();
   clear_screen();
tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 0);
  tft.print(data_0_dispText);
  //tft.setCursor(11, 0);
  tft.print(": ");
  //tft.setCursor(12, 0);
  tft.println(data_0_value);
  
  tft.setCursor(10, 20);
  tft.print(data_1_dispText);
  //tft.setCursor(11, 10);
  tft.print(": ");
  //tft.setCursor(12, 10);
  tft.println(data_1_value);

  tft.setCursor(10, 40);
  tft.print(data_2_dispText);
  //tft.setCursor(11, 20);
  tft.print(": ");
  //tft.setCursor(12, 20);
  tft.println(data_2_value);

  tft.setCursor(10, 60);
  tft.print(data_3_dispText);
  //tft.setCursor(11, 30);
  tft.print(": ");
  //tft.setCursor(12, 30);
  tft.println(data_3_value);

  tft.setCursor(10, 70);
  tft.print(data_4_dispText);
  //tft.setCursor(11, 40);
  tft.print(": ");
  //tft.setCursor(12, 40);
  tft.println(data_4_value);

  tft.setCursor(10, 80);
  tft.print(data_5_dispText);
  //tft.setCursor(11, 50);
  tft.print(": ");
  //tft.setCursor(12, 50);
  tft.println(data_5_value);
Serial.print(data_0_dispText);
Serial.print(": ");
Serial.println(data_0_value);

Serial.print(data_1_dispText);
Serial.print(": ");
Serial.println(data_1_value);

Serial.print(data_2_dispText);
Serial.print(": ");
Serial.println(data_2_value);

Serial.print(data_3_dispText);
Serial.print(": ");
Serial.println(data_3_value);

Serial.print(data_4_dispText);
Serial.print(": ");
Serial.println(data_4_value);

Serial.print(data_5_dispText);
Serial.print(": ");
Serial.println(data_5_value);
    }
  }
 

WiFiClient wifiClient;
PubSubClient   mqttClient(mqttServer, 1883, callback, wifiClient);

void reconnect()
{
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqttServer);

    if (mqttClient.connect(ID, "edraak", "edra@k112233"))//, "edraak", "edra@k112233"
    {
      Serial.println("...connected");
      
            String data = "Hello MQTT";
            //mqttClient.publish("rfidOut", data.c_str());
            mqttClient.subscribe("InTopic");

    }
    else
    {
      Serial.print("...failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void WiFiBegin(const char* ssid, const char* pass)
{
  WiFi.begin(ssid, pass);
  Serial.printf("Waiting for AP connection ...\n");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(WIFI_TIMEOUT);
    Serial.printf(".");
}
  IPAddress ip = WiFi.localIP();
  Serial.printf("\nConnected to AP. IP : %d.%d.%d.%d\n", 
    ip[0],ip[1],ip[2],ip[3]);
}
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI FSPI
#endif

static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
SPIClass * hspi = NULL;
void setup() {
//  pinMode(D8,OUTPUT);// this pin is used for flood/pump on or off purpose
    //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  pinMode(buzzerpin,OUTPUT);
  pinMode(redled,OUTPUT);
  pinMode(grnled,OUTPUT);
  vspi = new SPIClass(VSPI);
  hspi = new SPIClass(HSPI);

  Serial.begin(9600);
  // Initialize device.
  //dht.begin();
  WiFi.mode(WIFI_STA);
  WiFiBegin(STA_SSID, STA_PASS);
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(1000);
 //SPI.begin();
  vspi->begin();
   //SPI.begin(RFID_SCLK, RFID_MISO, RFID_MOSI, SS_PIN);
  rfid.PCD_Init();
  Serial.println("RFID reader initialized.");
  Serial.println(F("reading"));
  //TFT Start
  SPI.begin(HSPI_SCLK, -1, HSPI_MOSI, -1);
 //hspi->begin();
  tft.init();
  set_screen();
 // tft.print("ateeq!");
  //TFT End
}
void loop() {
  //digitalWrite(redled, LOW);
 // digitalWrite(grnled, LOW);
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      DynamicJsonDocument jsonDoc(256); // Create a JSON document

      // Add data to the JSON document
      jsonDoc["deviceID"] = ID;
      String tagValue;
      for (byte i = 0; i < 4; i++) {
        tagValue += rfid.uid.uidByte[i];
      }
      jsonDoc["rfID"] = tagValue;

      // Convert JSON document to a string
      String jsonString;
      serializeJson(jsonDoc, jsonString);

      // Publish JSON string to MQTT topic
      mqttClient.publish("rfID", jsonString.c_str());


      Serial.print("JSON message sent to broker: ");
      Serial.println(jsonString);
      /*digitalWrite(buzzerpin, HIGH);
      digitalWrite(grnled, HIGH);
      delay(500);
      digitalWrite(buzzerpin, LOW);
      digitalWrite(grnled, LOW);
      delay(500);*/

      tag = "";
    }
  }
  delay(100);
}
void clear_screen(){
  tft.fillRect(0, 0, tft.width() - 0, tft.height(), ST7735_BLACK);
}
void set_screen(){
tft.setRotation(0);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 0);
  tft.setTextColor(ST7735_WHITE);
  tft.print("Employe:");
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.print("Operation:");
  tft.setTextSize(1);
  tft.setCursor(10, 20);
  tft.print("BundlePcs:");
  tft.setTextSize(1);
  tft.setCursor(10, 30);
  tft.print("Tot.Pcs:");
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.print("Style:");
  tft.setTextSize(1);
  tft.setCursor(10, 50);
  tft.print("Eff%:");
}
/*void popup_true_scrn(){
  clear_screen();
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 0);
  tft.print(data_true);
  delay(3000);
}*/
/*void popup_false_scrn(){
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 0);
  tft.print(data_0_dispText);
  tft.setCursor(11, 0);
  tft.print(": ");
  tft.setCursor(12, 0);
  tft.println(data_0_value);
  
  tft.setCursor(10, 10);
  tft.print(data_1_dispText);
  tft.setCursor(11, 10);
  tft.print(": ");
  tft.setCursor(12, 10);
  tft.println(data_1_value);

  tft.setCursor(10, 20);
  tft.print(data_2_dispText);
  tft.setCursor(11, 20);
  tft.print(": ");
  tft.setCursor(12, 20);
  tft.println(data_2_value);

  tft.setCursor(10, 30);
  tft.print(data_3_dispText);
  tft.setCursor(11, 30);
  tft.print(": ");
  tft.setCursor(12, 30);
  tft.println(data_3_value);

  tft.setCursor(10, 40);
  tft.print(data_4_dispText);
  tft.setCursor(11, 40);
  tft.print(": ");
  tft.setCursor(12, 40);
  tft.println(data_4_value);

  tft.setCursor(10, 50);
  tft.print(data_5_dispText);
  tft.setCursor(11, 50);
  tft.print(": ");
  tft.setCursor(12, 50);
  tft.println(data_5_value);
}*/


