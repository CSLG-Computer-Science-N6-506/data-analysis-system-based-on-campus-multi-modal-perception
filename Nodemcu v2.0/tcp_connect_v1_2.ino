/* 这是wifi调试库~
 *  
 *  
 *  
 */
//SCL -D1 SDA -D2 IIC
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>   // 使用WiFiMulti库 
#include <DS1302.h>
#ifndef STASSID
#define STASSID "N6-506-Student"
#define STAPSK  "N6-506-Student"
#endif
#define SERVER_IP "192.168.31.45:9999"
WiFiClient client;
#include <Adafruit_Sensor.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C
String Deviceid="\"Nmcu-N_c3-Tem-Press-Lig\"";
String temperature="-1";
String humidity="-1";
String pressure="-1";
String Light="-1";
String co2="-1";
String jiaquan="-1";
String location="\"N6-506-测试\"";
String str,data,local_time;
#include <BH1750.h>
BH1750 lightMeter(0x23);                  // GY30
namespace {
const int kCePin   = D5;  // Chip Enable
const int kIoPin    = D4;  // Input/Output
const int kSclkPin = D3;  // Serial Clock
//const int kCePin   = D5;  // Chip Enable
//const int kIoPin    = D4;  // Input/Output
//const int kSclkPin = D0;  // Serial Clock
}
String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday:        return "Sunday";
    case Time::kMonday:       return "Monday";
    case Time::kTuesday:       return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday:     return "Thursday";
    case Time::kFriday:          return "Friday";
    case Time::kSaturday:      return "Saturday";
  }
  return "(unknown day)";
}
#include <ArduinoJson.h>
DS1302 rtc(kCePin, kIoPin, kSclkPin); // 时钟模块配置
#include <ESP8266HTTPClient.h>
HTTPClient http;
void setup(void){ //开机运行的函数
  Serial.begin(115200);
  delay(100);
  WiFi.begin(STASSID, STAPSK); //连接wifi
  bmp.begin(0x76);  //bmp280启动
lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);//GY30启动
  lightMeter.readLightLevel();
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("wifiw未连接......");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
setclock();
}





void loop(void)//一直循环运行的函数
{
  if ((WiFi.status() == WL_CONNECTED)) {
     delay(2000);
  Time t = rtc.time();
    Serial.print(t.hr);Serial.print(":");Serial.print(t.min);Serial.print(":");Serial.print(t.sec);
   Serial.print("\n");
    if(int(t.min)%2==0 && t.sec<5 && t.sec>2){
      calculator();
  Light=  lightMeter.readLightLevel();
  senddatapost();
    }
  }
}

void calculator(){
    temperature = bmp.readTemperature();
  pressure = bmp.readPressure() / 100.0F;
}




void senddatapost(){//用post请求发送json格式数据
    Serial.print("[HTTP] begin...\n");
    http.begin(client, "http://" SERVER_IP "/api/sensor"); //HTTP
    http.addHeader("Content-Type", "application/json");
    Serial.print("[HTTP] POST...\n");

    Serial.print("[HTTP] POST...\n");
    String Postdata="{\n\"Deviceid\":"+Deviceid+",\n";
    Postdata +="\"temp\":" + temperature + ",\n";
    Postdata +="\"humity\":" + humidity + ",\n";
    Postdata +="\"press\":" + pressure + ",\n";
    Postdata +="\"light\":" + Light + ",\n";
    Postdata +="\"co2\":" + co2 + ",\n";
    Postdata +="\"jiaquan\":" + jiaquan + ",\n";
    Postdata +="\"location\":" + location + "\n";
    Postdata+="}";
    Serial.println(Postdata);
   int httpCode = http.POST(Postdata);

    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }

  

  void setclock(){
    http.begin(client, "http://" SERVER_IP "/api/time"); //HTTP
    http.addHeader("Content-Type", "application/json");
    Serial.print("[HTTP] POST...\n");
    String PostTime="{\n\"Deviceid\":"+Deviceid+"\n";
    PostTime+="}";
    Serial.println(PostTime);
    int httpCode = http.POST(PostTime);
    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        const String& data = http.getString();
    Serial.println(data);
//     Serial.println(data[0]);
//          Serial.println(data[1]);
//            Serial.println(data[2]);
//        Serial.println(data[3]);
//        Serial.println(data[4]);
//            Serial.println(data[5]);
//        Serial.println(data[6]);
//        Serial.println(data[7]);
//        Serial.println(data[8]);
    rtc.writeProtect(false);
    rtc.halt(false);
    int mon, date, min, sec, hr;
    String temp, temp1;
    temp = data[6];
    temp1 = data[7];
    mon = temp.toInt() * 10 + temp1.toInt();
    temp = data[9];
    temp1 = data[10];
    date = temp.toInt() * 10 + temp1.toInt();
    temp = data[12];
    temp1 = data[13];
    hr = temp.toInt() * 10 + temp1.toInt();  //yes
    temp = data[15];
    temp1 = data[16];
    min = temp.toInt() * 10 + temp1.toInt();
    temp = data[18];
    temp1 = data[19];
    sec = temp.toInt() * 10 + temp1.toInt();
    if (sec+5 > 60){
        min=min+1;
        sec=sec-55;
    }
    else{
        sec=sec+5;
    }                                                                             
    Time t(2019, mon, date, hr, min, sec, Time::kSunday);
    rtc.time(t);
      }
    } else {
      Serial.printf("[HTTP] POST... failed,Time-wrong, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
   
}
