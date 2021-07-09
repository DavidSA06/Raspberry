//Hola mundo soy David Silva Apango @DavidSA06
//2019 Curso de Intro a IoT NodeMCU Board

//Lib
#include "ESP8266WiFi.h"
#include <aREST.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

//Def
#define DHTPIN D2
#define DHTTYPE DHT22
#define MQ A0

//Init
DHT dht(DHTPIN, DHTTYPE);

//aREST
aREST rest = aREST();

//WiFi 
const char* ssid = "CGA2121_z8yZdf6";         //Nombre del router
const char* password = "wzM37c9ZaaBWfXGzwW";  //Contraseña del router

//Port
#define LISTEN_PORT 80

//Create instance of server
WiFiServer server(LISTEN_PORT);

//Variables API
float mq, humidity, temperature;

void setup(){
  Serial.begin(9600);
  dht.begin();
  
//Init Variables API
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
  rest.variable("contaminacion",&mq);

//Name ID
  rest.set_id("2");
  rest.set_name("sensor_node");

//Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected!");

//Start Server
  server.begin();
  Serial.println("Server started!");

//IP
  Serial.println(WiFi.localIP());
}

void loop() {
  //Wait 1s
  delay(1000);
  mq = analogRead(MQ); //MQ3
  humidity = dht.readHumidity(); //RH %0 - 100 (Punto rocio)
  temperature = dht.readTemperature(); //0 - 100 *C

  //REST Calls
  WiFiClient client = server.available();
  if (!client) {
    return;
    }
  while(!client.available()){
    delay(1);
    }
  rest.handle(client);  
}
