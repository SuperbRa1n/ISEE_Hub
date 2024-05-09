#include "network.h"
#include "DS1302.h"
#include <Arduino.h>

#define NTP1 "ntp1.aliyun.com"
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"

// wifi联网
void Network::init(const char *ssid, const char *password)
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// SmartConfig配网
void Network::SmartConfig()
{
  // Init WiFi as Station, start SmartConfig
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();

  // Wait for SmartConfig packet from mobile
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone())
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("SmartConfig received.");

  // Wait for WiFi to connect to AP
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected.");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// 获取bilibili粉丝数
unsigned int Network::getBilibiliFans(String uid)
{
  String fansCount = "";

  HTTPClient http;
  http.begin("https://api.bilibili.com/x/relation/stat?vmid="+uid);

  // start connection and send HTTP headerFFF
  int httpCode = http.GET();
    

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // httpCode will be negative on error 如果是负值的话就是error
		// Serial.printf("HTTP Get Code: %d\r\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      int pos = (payload.indexOf("follower"));
      fansCount = payload.substring(pos + 10, payload.length() - 2);
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  Serial.printf("fansCount=%s\r\n",fansCount.c_str());
  return atol(fansCount.c_str());
}

// 获取时间
unsigned int Network::getTime(void)
{

  String data = "";
  HTTPClient https;
  https.begin("http://quan.suning.com/getSysTime.do");

  // start connection and send HTTP headerFFF
  int httpCode = https.GET();

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    		// httpCode will be negative on error 如果是负值的话就是error
		// Serial.printf("HTTP Get Code: %d\r\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    {
      String payload = https.getString();
      // Serial.println(payload);
      int pos = (payload.indexOf("sysTime1"));
      data = payload.substring(pos + 19, pos + 25); // 11 19
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
  }
  https.end();
  // Serial.printf("%s\r\n",data);
  return atol(data.c_str());
}

// 获取日期
unsigned int Network::getData(void)
{
  String data = "";
  HTTPClient https;
  https.begin("http://quan.suning.com/getSysTime.do");

  // start connection and send HTTP headerFFF
  int httpCode = https.GET();

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    		// httpCode will be negative on error 如果是负值的话就是error
		// Serial.printf("HTTP Get Code: %d\r\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    {
      String payload = https.getString();
      // Serial.println(payload);
      int pos = (payload.indexOf("sysTime1"));
      data = payload.substring(pos + 11, pos + 19); // 11 19
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
  }
  https.end();
  // Serial.printf("%s\r\n",data);
  return atol(data.c_str());
}

// 获取心知天气数据
unsigned int Network::getxinzhiweatherdata(String key, String location)
{
  String temperature = "";
  String weatherCount = "";
  HTTPClient http;
  String url = "https://api.seniverse.com/v3/weather/now.json?key=&location=&language=zh-Hans&unit=c";
  url.replace("key=", "key=" + key);
  url.replace("location=", "location=" + location);
  http.begin(url);
  // start connection and send HTTP headerFFF
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    		// httpCode will be negative on error 如果是负值的话就是error
		// Serial.printf("HTTP Get Code: %d\r\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      // Serial.println(payload);
      int pos = (payload.indexOf("temperature"));//temperature
      temperature = payload.substring(pos + 14, pos + 16);
      int pos1 = (payload.indexOf("code"));
      weatherCount = payload.substring(pos1 + 7, pos1 + 9);
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  Serial.printf("tempera=%s,weather=%s\r\n",temperature,weatherCount);
  return atol(temperature.c_str()) * 100 + atol(weatherCount.c_str());
  // Serial.printf("%s",weatherCount);
  // return atol(weatherCount.c_str());
}

void Network::setClock()
{
  struct tm time_info;
  while (!getLocalTime(&time_info))
  {
    configTime(8 * 3600, 0, NTP1, NTP2, NTP3);
    Serial.println("get time fail");      // 默认打印格式：Mon Oct 25 11:13:29 2021
  }
  Serial.println(&time_info, "%F %T %A"); // 格式化输出:2021-10-24 23:00:44 Sunday
  Serial.print(asctime(&time_info));      // 默认打印格式：Mon Oct 25 11:13:29 2021
  //   WiFi.disconnect(true);//断开网络连接，关闭网络
  ds1302_time_data.year = time_info.tm_year + 1900;
  ds1302_time_data.month = time_info.tm_mon + 1;
  ds1302_time_data.dayMonth = time_info.tm_mday;
  ds1302_time_data.dayWeek = time_info.tm_wday;
  ds1302_time_data.hour = time_info.tm_hour;
  ds1302_time_data.minute = time_info.tm_min;
  ds1302_time_data.second = time_info.tm_sec;

}
