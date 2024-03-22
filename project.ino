#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
 #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
// TODO -- Define WiFi credentials
#define ssid "RouterName"
#define password "Password"
String baseURL = "http://proiectia.bogdanflorea.ro/api/hearthstone-minions/";
String adder="";
String payload="";
#define btcServerName "ProjectName"
byte BTData;
bool deviceConnected = false;
// TODO -- Generate a unique UUID for your Bluetooth service
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "f9c0361c-f28f-11ec-b939-0242ac120002"
String apiString;
String phRes;
String newID;
int typeReq;
// Define the BluetoothSerial
BluetoothSerial SerialBT;
// Received data
String data = "";
// The receivedData function is called when data is available on Bluetooth (see loop function)
 String receivedData() {
 // Read the data using the appropriate SerialBT functions
 // according to the app specifications
 // The data is terminated by the new line character (\n)
 String dataFromApp = "";
 while (SerialBT.available()) {
 dataFromApp = SerialBT.readStringUntil('\n');
 }

 DynamicJsonDocument doc(4096);
 DeserializationError error = deserializeJson(doc, dataFromApp);

 // Test if parsing succeeds.
 if (error) {
 Serial.print(F("Do something: "));
 Serial.println(error.f_str());

 return "empty";
 }

 String action=doc["action"];
 if(action=="fetchData"){
 Serial.print("New action: ");
 Serial.println(action);
 return action;
 }
 else if(action=="fetchDetails"){
 String cardId=doc["cardId"];
 Serial.print("New action: ");
 Serial.print(action);
 Serial.print(" for cardId: ");
 Serial.println(cardId);
 newID=cardId;
 return action;
 }
}
String httpGETRequest(String adder) {
 HTTPClient http;
 String path=baseURL+adder;
 String payload ="";
 Serial.print("Attempting to connect to: ");
 Serial.println(path); // <-- This is the message sent from the app, according to the specs
 //Specify the URL
 http.setTimeout(12000);
 http.begin(path);
 //Make the request
 int httpCode = http.GET();
 if (httpCode == 200) { //Check for the returning code

 payload = http.getString();
 Serial.println("GOOD");
 }
 else {
 Serial.print("Error on HTTP request: ");
 Serial.println(httpCode);
 }

 http.end(); //Free the resources

 return payload;
}
 // Possible steps:
 // 1. Deserialize data using the ArduinoJson library
 // 2. Get the action from the JSON object
 // 3. Check action and perform the corresponding HTTP request (check the method in the API specifications)
// (Define the API url corresponding to the action and get the response)
 // IMPORTANT -- make sure to set the http request timeout to 10s or more
 // 4. Check the response code and deserialize the response data
 // IMPORTANT -- If using the ArduinoJson library, use a DynamicJsonDocument with the size of 15000
 // 5. Define the response structure, according to the data returned by the API
 // (Use JsonArray or JsonObject, depending on the response type)
 // IMPORTANT -- The cacapcity must not exceed 512B, especially for BLE
 // 6. Populate the response object according to the API and app specs
 // 7. Encode the response object as a JSON string
 // 8. Write value to the appropriate characteristic and notify the app

 // TODO -- Write your code
 void sendDataToApp_Details(String payload){
 StaticJsonDocument <768> JSONDocument;
 DeserializationError error = deserializeJson(JSONDocument, payload.c_str());
 if (error) {
 Serial.print("Problem with fetchDetails: ");
 Serial.println(error.c_str());
 } else {
 String responseString;
 String cardId=JSONDocument["cardId"].as<String>();
 String name=JSONDocument["name"].as<String>();
 String cardset=JSONDocument["cardset"].as<String>();
 String type=JSONDocument["type"].as<String>();
 String cost=JSONDocument["cost"].as<String>();
 String attack=JSONDocument["attack"].as<String>();
 String health=JSONDocument["health"].as<String>();
 String img=JSONDocument["img"].as<String>();
 String description="Card set: "+cardset+"\n"+"Type: "+type+"\n"+"Cost: "+cost+"\n"+"Attack: "+attack+"\n"+"Health:
"+health;
 StaticJsonDocument <512> newJSONDocument;//https://arduinojson.org/v6/assistant
 JsonObject object = newJSONDocument.to<JsonObject>();

 object["cardId"] = cardId;
 object["name"] = name;
 object["img"] = img;
object["description"] = description;
 serializeJson(newJSONDocument, responseString);

 SerialBT.println(responseString);
 Serial.println(responseString);

 }
}
void sendDataToApp(String payload){
 DynamicJsonDocument JSONDocument(8192);
 DeserializationError error = deserializeJson(JSONDocument, payload.c_str());
 if (error) {
 Serial.print("Problem with fetchData: ");
 Serial.println(error.c_str());
 } else {
 // Define a JsonArray from the JSONDocument, since the JSONString is an array of objects
 JsonArray list = JSONDocument.as<JsonArray>();
 // Iterate the JsonArray array (see docs for the library)
 int index = 1;
 for (JsonVariant value : list) {
 JsonObject listItem = value.as<JsonObject>();
 // Get the current item in the iterated list as a JsonObject

 String cardId=listItem["cardId"].as<String>();
 String name=listItem["name"].as<String>();
 String img=listItem["img"].as<String>();
 String responseString;
 StaticJsonDocument <4096> newJSONDocument;//https://arduinojson.org/v6/assistant
 JsonObject object = newJSONDocument.to<JsonObject>();
 object["cardId"] = cardId;
object["name"] = name;
 object["img"] = img;
 serializeJson(newJSONDocument, responseString);
 SerialBT.println(responseString);
 Serial.println(responseString);

 delay(100);
 index++;
 }
 }
}
void setup() {

 // put your setup code here, to run once:
 Serial.begin(115200);
 // TODO -- Connect WiFi
 // TODO -- Write your code
 Serial.print("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 // Print local IP address and start web server
 Serial.println("");

 // No need to change the lines below
 // Initialize BTC
SerialBT.begin(btcServerName); //Bluetooth device name
Serial.println("The device started, now you can pair it with bluetooth!");

}
void loop() {

 // put your main code here, to run repeatedly:
 // Check available Bluetooth data and perform read from the app
 if(WiFi.status()== WL_CONNECTED){
 if(SerialBT.connected()){
 phRes=receivedData();
 if(phRes=="fetchData"){
 String payload=httpGETRequest("minions");
 sendDataToApp(payload);
 }
 else if(phRes=="fetchDetails"){
 String payload=httpGETRequest("minion/" + newID);
 sendDataToApp_Details(payload);
 }
 else if(phRes=="empty"){
 Serial.println("Waiting for a new task");
 }
 }
 else {
 Serial.print("Waiting for device to connect through bluetooth");
 while(!SerialBT.connected()){
 delay(500);
 Serial.print(".");
 }
 Serial.println();
 }
 }
 else {
 Serial.println("WiFi Disconnected");
 }
 delay(400); } 
