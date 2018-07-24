#include<ESP8266WiFi.h>
#include<ArduinoJson.h>
int a=1;
//const char* ssid="fishtank";
const char* ssid="tcd";
//const char* password="cocococo";
const char* password="19950522";
const size_t MAX_CONTENT_SIZE=2000;
const char* host="api.seniverse.com";
//const char* host="api.wunderground.com";
const char* city="Sydney";
const char* APIKEY="tg5pibbybwksednn";
const char* language="en";
char* lr="Light Rain";
char* Mr="Moderate Rain";
char* Hr="Heavy Rain";
const unsigned long HTTP_TIMEOUT=5000;
int i;
//const char* APIKEY="d6fb682fa4d47557";
//const char* city="SYDNEY";
struct UserData{
char city[16];
char weather[32];
};
UserData userData;
WiFiClient client;
char response[MAX_CONTENT_SIZE];
char endOfHeaders[]="\r\n\r\n";

void setup() {
WiFi.mode(WIFI_STA);
Serial.begin(250000);
delay(10);
Serial.print("connecting to");
Serial.print(ssid);
WiFi.begin(ssid,password);
while(WiFi.status()!=WL_CONNECTED)
{delay(500);
Serial.print(".");}

Serial.println("connected!");
delay(500);
Serial.println("IP address:");
Serial.println(WiFi.localIP());
client.setTimeout(HTTP_TIMEOUT);
delay(500);
}
void loop() {
  while(!client.connected()){
    if(!client.connect(host,80))
    {Serial.println("connecting...");
    delay(500);}
    }
if(sendRequest(host,city,APIKEY)&&skipResponseHeaders())
{
  clrEsp8266ResponseBuffer();
    readResponseContent(response, sizeof(response));
    //UserData userData;
    if (parseUserData(response, &userData)) {
      printUserData(&userData);
      i=cmp(&userData);
  }
  delay(500);

}

  if(i==1)
{Serial.print("sending 1 to indicate that it is rainy");
  for(int j=0;j<600;j++)
  {
    
    Serial.write(1);
    delay(1000);
   }
}
 if(i==0)
 {Serial.print("sending 0 to indicate it is not rainy");
  for(int k=0;k<600;k++)
  {
    
    Serial.write(0);
    delay(1000);
   }
 }
}

bool sendRequest(const char* host,const char* city,const char* apikey){
  String GetUrl = "/v3/weather/now.json?key=";
 GetUrl += apikey;
  GetUrl += "&location=";
  GetUrl += city;
  GetUrl += "&language=";
  GetUrl += language;
 // String GetUrl="/api/d6fb682fa4d47557/conditions/q/AU/SYDNEY.json";
  client.print(String("GET ") + GetUrl + " HTTP/1.1\r\n" + "Host:" + host + "\r\n" + "Connection:close\r\n\r\n");
  Serial.println("Create a request:");
  Serial.println(String("GET ")+GetUrl + " HTTP/1.1\r\n"+ "Host:" + host + "\r\n"+"Connection:close\r\n");  
delay(1000);  
 return true;
  }
bool skipResponseHeaders(){
bool ok=client.find(endOfHeaders);
if(!ok){
  Serial.println("No response or invalid response");
  }
  return ok;
}

void readResponseContent(char* content,size_t maxSize){
size_t length=client.peekBytes(content,536);
delay(100);
Serial.println("Get the data from Internet!");
content[length] = 0;
Serial.println(length);
Serial.println(content);
 Serial.println("Read data Over!");
 client.flush();
}
bool parseUserData(char* content, struct UserData* userData) {
DynamicJsonBuffer jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(content);
 if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }
  strcpy(userData->weather, root["results"][0]["now"]["text"]);
return true;
}
void printUserData(const struct UserData* userData){
    Serial.println("Print parsed data :");
    Serial.print("Weather:");
  Serial.print(userData->weather);
}
void stopConnect() {
  Serial.println("Disconnect");
  client.stop();
}
void clrEsp8266ResponseBuffer(void){
    memset(response, 0, MAX_CONTENT_SIZE);      //清空
} 
int cmp(struct UserData* userData)
{
  if(!(strcmp(userData->weather,lr)||strcmp(userData->weather,Mr)||strcmp(userData->weather,Hr)))
  {  return 1;}
  else
  {return 0;}
  }
