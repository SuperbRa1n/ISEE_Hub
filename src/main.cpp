#include <Arduino.h>
#include <DS1302.h>
#include <FastLED.h>
#include <WebServer.h>
#include <boutton.h>
#include <network.h>
#include <pinpu.h>
#include <rgb_show.h>

String device_id = "HUB_01";

String ssid = "zzy";
String password = "asdfghjkl";

#define TZ 8    // 中国时区为8
#define DST_MN 0// 默认为0

#define TZ_MN ((TZ) * 60)// 时间换算
#define TZ_SEC ((TZ) * 3600)
#define DST_SEC ((DST_MN) * 60)

WebServer server(80);

String B_fans = "259287435";
hw_timer_t *timer = nullptr;
String key = "Sh9GQnxRpGVHKYPHo";
String location = "hangzhou";

DPLAY SHOW;
DS1302_Dev ds1302_dev;
DS1302_DateTime ds1302_time_data;
_RTC_obj r_calendar;
Network wifi;
bouton btoun;
PINPU FT;

int tem_data;

extern int temperture, weather, Bfans;

void IRAM_ATTR TimerEvent() {
    btoun.boutton_tick();
}


void handleRoot() {
    String html = "<form action='/connect' method='post'>Enter your information: <br><input type='text' name='B_fans' value='your bilibili uid'><br><input type='text' name='key' value='your API_KEY'><br><input type='text' name='location' value='your location(such as hangzhou)'><br><input type='submit' value='submit'></form>";
    server.send(200, "text/html", "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body>" + html + "</body></html>");
}

void handleConnect() {
    B_fans = server.arg("B_fans");
    key = server.arg("key");
    location = server.arg("location");
    unsigned long timeout = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - timeout < 10000) {
        delay(1000);
    }
    if (WiFi.status() == WL_CONNECTED) {
        server.send(200, "text/html", "<p>The information has been submitted./p>");
        Serial.println("成功提交表单");
    } else {
        server.send(200, "text/html", "<p>Submition failed.</p>");
        Serial.println("失败提交表单");
    }
    wifi.setClock();// 访问时间并赋值给rtc时钟

    tem_data = wifi.getxinzhiweatherdata(key, location);// 获取天气数据
    temperture = tem_data / 100;
    weather = tem_data % 100;
    Serial.printf("%d,%d\r\n", temperture, weather);

    Bfans = wifi.getBilibiliFans(B_fans);// 获取B站粉丝数
    Serial.printf("B粉丝=%d\r\n", Bfans);

    DS1302_setDateTime(&ds1302_dev, &ds1302_time_data);
}

void setup() {

    Serial.begin(115200); /* prepare for possible serial debug */
    btoun.boutton_init();
    FT.tft_init();
    DS1302_begin(&ds1302_dev, 5, 18, 19);
    // wifi.SmartConfig();          // wifi智能配网
    wifi.init(ssid.c_str(), password.c_str());// wifi密码连接
    WiFi.mode(WIFI_AP_STA);                   // enable Station and AP mode
    WiFi.softAP(device_id);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    server.on("/", handleRoot);
    server.on("/connect", HTTP_POST, handleConnect);
    server.begin();
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &TimerEvent, true);
    timerAlarmWrite(timer, 100, true);
    timerAlarmEnable(timer);// 使能定时器
}

void loop() {
    server.handleClient();
    DS1302_setDateTime(&ds1302_dev, &ds1302_time_data);
    if (flagShow != 0) {
        UpdateShow = flagShow;
        FT.ws281x_closeAll();
        wifi.setClock();
        flagShow = 0;
        delay(100);
        Serial.println("flagShow");
        // 	printf("%d\r\n", UpdateShow);
        // 	printf("%d\r\n", flagShow);
    }

    if (UpdateShow == 1) {
        wifi.setClock();
        SHOW.Show_Timer();
    }
    if (UpdateShow == 2) {
        SHOW.Show_Year();
    }

    if (UpdateShow == 3) {
        SHOW.Show_Temp();
    }

    if (UpdateShow == 4) {
    }
    if (UpdateShow == 5) {
        FT.tft_run();
    }
    if (UpdateShow == 6)// 更新数据
    {
        wifi.setClock();// 访问时间并赋值给rtc时钟

        tem_data = wifi.getxinzhiweatherdata(key, location);// 获取天气数据
        temperture = tem_data / 100;
        weather = tem_data % 100;
        Serial.printf("%d,%d\r\n", temperture, weather);

        Bfans = wifi.getBilibiliFans(B_fans);// 获取B站粉丝数
        Serial.printf("B粉丝=%d\r\n", Bfans);
    }
    if (UpdateShow == 7) {
        SHOW.Show_Bfans();
        // SHOW.ws281x_ShowIcon();
    }
    if (UpdateShow == 8)// 关闭显示
    {
        FT.ws281x_closeAll();
    }
    SHOW.Show_Temp();
    FastLED.show();
}
