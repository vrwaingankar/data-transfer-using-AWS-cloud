#include "credits.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Blynk.h>
#include <DHT.h>

#define DHTPIN 2                                                           // what digital pin the sensor is connected to
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

float h;
float t;

int c = 0;

#define AWS_IOT_PUBLISH_TOPIC   "esp32/temp_humid"
//#define AWS_IOT_SUBSCRIBE_TOPIC1 "esp32/port1"      // used to send data if required

BlynkTimer timer;
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void humidity_temperature()
{
  h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  t = dht.readTemperature();
  StaticJsonDocument<200> doc;
  doc["message"] = "Temperature and Humidity values";
  doc["Temp"] = t;
  doc["Hum"] = h;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.println("Message Published");
}

// Timer Callback function
void myTimerEvent()
{

  StaticJsonDocument<200> doc;
  doc["message"] = "Hello";
  doc["Counter"] = c;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  humidity_temperature()
  Serial.println("Message Published");
  c++;
}

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC1);
  Serial.println("AWS IoT Connected!");
}

// used for sending data from esp32 to AWS cloud
//void messageHandler(char* topic, byte* payload, unsigned int length)
//{
//  Serial.print("incoming: ");
//  Serial.println(topic);
//
//  if ( strstr(topic, "esp32/port1") )
//  {
//    StaticJsonDocument<200> doc;
//    deserializeJson(doc, payload);
//    String Relay_data = doc["status"];
//    int r = Relay_data.toInt();
//    digitalWrite(Relay1, !r);
//    Serial.print("Relay1 - "); Serial.println(Relay_data);
//  }
//
//}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  dht.begin();     //initialise DHT11
  pinMode(DHTPIN, INPUT);
  timer.setInterval(500L, myTimerEvent);
  connectAWS();
}

void loop()
{
  timer.run();
  client.loop();
}
