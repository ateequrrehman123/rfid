#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "Edraak-1";
const char *password = "edraak123";
AsyncWebServer server(80);
String fileName;
File file;

void generateRandomValues(uint32_t values[12]) {
    for (int i = 0; i < 12; ++i) {
        values[i] = random();  // Generate a random 32-bit value
    }
}

String getFormattedDate();
String zeroPad(int value);
void writecall();
void readcall();

const int chipSelect = 5;
RTC_DS3231 rtc;

char daysOfWeek[7][12] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};

void setup () {
  Serial.begin(115200);
  srand((unsigned)time(NULL));
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  // SETUP RTC MODULE
  if (! rtc.begin()) {
    Serial.println("RTC module is NOT found");
    Serial.flush();
    while (1);
  }
  if (!SD.begin(chipSelect)) {
  Serial.println("SD card initialization failed!");
  while (1);
}
  // automatically sets the RTC to the date & time on PC this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("Card Mounted");
  // Print the assigned IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  readcall();
}

void loop () {
  DateTime now = rtc.now();
  Serial.print("ESP32 RTC Date Time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  if(now.second() == 0){
    writecall();
  }

  delay(1000); // delay 1 seconds
}
String getFormattedDate() {
  DateTime now = rtc.now();
  String formattedDate = String(now.year(), DEC) + "-" +
                         zeroPad(now.month()) + "-" +
                         zeroPad(now.day());
  return formattedDate;
}

String zeroPad(int value) {
  if (value < 10) {
    return "0" + String(value);
  } else {
    return String(value);
  }
}

void writeDataToCSV(DateTime now, uint32_t vab, uint32_t vbc, uint32_t vca,uint32_t vavg, uint32_t ia, uint32_t ib,uint32_t ic, uint32_t iavg, uint32_t pf,uint32_t freq, uint32_t kwht,uint32_t kwp){
    // Move to the end of the file to append data
    file.seek(file.size());
  file.print(now.hour(), DEC);
  file.print(':');
  file.print(now.minute(), DEC);
  file.print(':');
  file.print(now.second(), DEC);
    //file.print(row);
    file.print(",");
    file.print(vab);
    file.print(",");
    file.print(vbc);
    file.print(",");
    file.print(vca);
    file.print(",");
    file.print(vavg);
    file.print(",");
    file.print(ia);
    file.print(",");
    file.print(ib);
    file.print(",");
    file.print(ic);
    file.print(",");
    file.print(iavg);
    file.print(",");
    file.print(pf);
    file.print(",");
    file.print(freq);
    file.print(",");
    file.print(kwht);
    file.print(",");
    file.println(kwp);
}
void writecall(){
  DateTime now = rtc.now();
  uint32_t generatedValues[12];
    generateRandomValues(generatedValues);
    // Print the generated values 
    for (int i = 0; i < 12; ++i) {
        Serial.print("Variable ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(generatedValues[i]);
    }
   fileName = getFormattedDate() + ".csv";
  Serial.print("Opening file: ");
  Serial.println(fileName);
  // Convert String to const char*
  //const char* charFileName = fileName.c_str();
   file = SD.open("/" + fileName, FILE_APPEND);
  if (file) {
    // If the file is empty, write the header
        if (file.size() == 0) {
            file.println("Date,Vab,Vbc,Vca,Vavg,Ia,Ib,Ic,Iavg,Pf,Freq,KWH(Total),KW(P)");
            Serial.println("no way");
        }
        DateTime now = rtc.now();
  
writeDataToCSV(now, generatedValues[0], generatedValues[1], generatedValues[2],generatedValues[3], generatedValues[4], generatedValues[5],generatedValues[6], generatedValues[7], generatedValues[8],generatedValues[9], generatedValues[10],generatedValues[11]);

    Serial.println("Write success");
  } else {
    Serial.println("Write failed");
    //Serial.println(file.getError(), HEX);
  }
    // Read from the file
  file = SD.open("/" + fileName);
  if (file) {
String header = file.readStringUntil('\n');
    Serial.println("Header: " + header);
    String line = file.readStringUntil('\n');
    Serial.println("Data: " + line);
    String line2 = file.readStringUntil('\n');
    Serial.println("Data: " + line2);
    Serial.println("Data: " + line);   ///here is point
    String line3 = file.readStringUntil('\n');
    Serial.println("Data: " + line3);
    file.close();
  } else {
    Serial.println("Read failed");
  }
}
void readcall(){
  server.on("/getCSV", HTTP_GET, [](AsyncWebServerRequest *request) {
     file = SD.open("/" + fileName);
    if (file) {
      request->send(SD, "/" + fileName, "text/csv");
      file.close();
    } else {
      request->send(404, "text/plain", "File not found");
    }
  });

  server.begin();
}
