#include <rgb_show.h>
#include <DS1302.h>
#include <FastLED.h>

_calendar_obj calendar; // 时钟结构体

int temperture, weather, Bfans;
int b_FAN = 999999;
//~~~~~~~~~~~~~~~~~show weather~~~~~~~~~~~~~~~~~~~//
void DPLAY::ws2812_Showrain()
{
    int i;
    for (i = 3; i < 6; i++)
    {
        pinpu.ws2812_Showpice(2, i, 140, 120, 120);
    }
    for (i = 2; i < 7; i++)
    {
        pinpu.ws2812_Showpice(3, i, 140, 120, 120);
    }
    for (i = 1; i < 8; i++)
    {
        pinpu.ws2812_Showpice(4, i, 140, 120, 120);
    }
    pinpu.ws2812_Showpice(5, 2, 160, 240, 240);
    pinpu.ws2812_Showpice(5, 4, 160, 240, 240);
    pinpu.ws2812_Showpice(5, 6, 160, 240, 240);
    pinpu.ws2812_Showpice(6, 1, 160, 240, 240);
    pinpu.ws2812_Showpice(6, 3, 160, 240, 240);
    pinpu.ws2812_Showpice(6, 5, 160, 240, 240);
}
void DPLAY::ws2812_Showsun()
{
    int i;
    for (i = 2; i < 6; i++)
    {
        pinpu.ws2812_Showpice(1, i, 64, 200, 240);
        pinpu.ws2812_Showpice(6, i, 64, 200, 240);
    }
    for (i = 1; i < 7; i++)
    {
        pinpu.ws2812_Showpice(2, i, 64, 255, 240);
        pinpu.ws2812_Showpice(5, i, 64, 255, 240);
    }
    for (i = 1; i < 3; i++)
    {
        pinpu.ws2812_Showpice(3, i, 64, 255, 240);
        pinpu.ws2812_Showpice(4, i, 64, 255, 240);
    }
    for (i = 5; i < 7; i++)
    {
        pinpu.ws2812_Showpice(3, i, 64, 255, 240);
        pinpu.ws2812_Showpice(4, i, 64, 255, 240);
    }
    for (i = 3; i < 5; i++)
    {
        pinpu.ws2812_Showpice(3, i, 0, 255, 240);
        pinpu.ws2812_Showpice(4, i, 0, 255, 240);
    }
}

void DPLAY::ws2812_Showyin()
{
    int i;
    for (i = 3; i < 6; i++)
    {
        pinpu.ws2812_Showpice(2, i, 140, 120, 120);
    }
    for (i = 2; i < 7; i++)
    {
        pinpu.ws2812_Showpice(3, i, 140, 120, 120);
    }
    for (i = 1; i < 8; i++)
    {
        pinpu.ws2812_Showpice(4, i, 140, 120, 120);
    }
}
void DPLAY::weather_select()
{
    switch (weather)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        DPLAY::ws2812_Showsun();
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        DPLAY::ws2812_Showyin();
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
        DPLAY::ws2812_Showrain();
        break;
    default:
        break;
    }
}

void DPLAY::ws2812_ShowData()
{
    pinpu.ws2812_ShowNum(0, 1, 25, 2, 1, 6);
    FastLED.show(); // 显示灯条
    delay(1);
}

void DPLAY::Show_Timer()
{
    int i;
    pinpu.ws281x_DrawPoint(2, 16, 2);
    pinpu.ws281x_DrawPoint(4, 16, 2);
    pinpu.ws281x_DrawPoint(2, 24, 2);
    pinpu.ws281x_DrawPoint(4, 24, 2);
    pinpu.ws2812_ShowNum(0, 9, ds1302_time_data.hour, 2, 1, 6);
    pinpu.ws2812_ShowNum(0, 17, ds1302_time_data.minute, 2, 1, 6);
    pinpu.ws2812_ShowNum(0, 25, ds1302_time_data.second, 2, 1, 6);

    for (i = 9; i < 16; i++)
    {
        pinpu.ws2812_Showpice(7, i, 224, 120, 240);
    }

    for (i = 17; i < 24; i++)
    {
        pinpu.ws2812_Showpice(7, i, 224, 120, 240);
    }

    for (i = 25; i < 32; i++)
    {
        pinpu.ws2812_Showpice(7, i, 224, 120, 240);
    }
    DPLAY::weather_select();
    FastLED.show(); // 显示灯条
    delay(1);
}

void DPLAY::Show_Year()
{
    int i;
    pinpu.ws281x_DrawPoint(5, 16, 2);
    pinpu.ws281x_DrawPoint(5, 24, 2);
    pinpu.ws2812_ShowNum(0, 9, ds1302_time_data.year % 100, 2, 1, 6);
    pinpu.ws2812_ShowNum(0, 17, ds1302_time_data.month, 2, 1, 6);
    pinpu.ws2812_ShowNum(0, 25, ds1302_time_data.dayMonth, 2, 1, 6);

    for (i = 9; i < 16; i++)
    {
        pinpu.ws2812_Showpice(7, i, 224, 120, 240);
    }

    for (i = 17; i < 24; i++)
    {
        pinpu.ws2812_Showpice(7, i, 224, 120, 240);
    }

    for (i = 25; i < 32; i++)
    {
        pinpu.ws2812_Showpice(7, i, 224, 120, 240);
    }
    DPLAY::weather_select();
    FastLED.show(); // 显示灯条
    delay(1);
}

void DPLAY::Show_Temp() // 温度
{
    int i;
    pinpu.ws2812_ShowNum(0, 18, temperture, 2, 3, 6);
    DPLAY::weather_select();

    //****************摄氏图标*****************//
    for (i = 1; i < 6; i++)
    {
        pinpu.ws2812_Showpice(i, 10, 200, 120, 240);
        pinpu.ws2812_Showpice(i, 12, 200, 120, 240);
    }
    for (i = 9; i < 14; i++)
    {
        pinpu.ws2812_Showpice(6, i, 200, 120, 240);
    }
    for (i = 2; i < 5; i++)
    {
        pinpu.ws2812_Showpice(i, 11, 220, 240, 240);
    }
    pinpu.ws2812_Showpice(1, 11, 200, 120, 240);
    pinpu.ws2812_Showpice(5, 11, 200, 120, 240);
    //******************摄氏度图标******************//
    pinpu.ws2812_Showpice(1, 25, 20, 240, 240);
    for (i = 26; i < 29; i++)
    {
        pinpu.ws2812_Showpice(2, i, 20, 240, 240);
        pinpu.ws2812_Showpice(5, i, 20, 240, 240);
    }
    pinpu.ws2812_Showpice(3, 26, 20, 240, 240);
    pinpu.ws2812_Showpice(4, 26, 20, 240, 240);

    //******************温度图标边框***************//
    // for (i = 0; i < 8; i++)
    // {
    //     pinpu.ws2812_Showpice(i, 9, 220, 120, 240);
    // }
    for (i = 9; i < 32; i++)
    {
        pinpu.ws2812_Showpice(7, i, 220, 120, 240);
    }


    //*******************天气图标边框***************//
    // for (i = 0; i < 8; i++)
    // {
    //     pinpu.ws2812_Showpice(i, 0, 180, 120, 240);
    //     pinpu.ws2812_Showpice(i, 7, 180, 120, 240);
    // }
    // for (i = 0; i < 8; i++)
    // {
    //     pinpu.ws2812_Showpice(7, i, 180, 120, 240);
    //     pinpu.ws2812_Showpice(0, i, 180, 120, 240);
    // }
    FastLED.show(); // 显示灯条
    delay(1);
}

void DPLAY::Show_Bfans()
{
    int i;
    pinpu.ws2812_ShowNum(0, 9, Bfans, 6, 4, 6);
    for (i = 2; i < 7; i++)
    {
        pinpu.ws2812_Showpice(2, i, 120, 120, 240);
        pinpu.ws2812_Showpice(7, i, 120, 120, 240);
    }
    for (i = 3; i < 7; i++)
    {
        pinpu.ws2812_Showpice(i, 1, 120, 120, 240);
        pinpu.ws2812_Showpice(i, 7, 120, 120, 240);
    }
    pinpu.ws2812_Showpice(0, 2, 120, 120, 240);
    pinpu.ws2812_Showpice(1, 3, 120, 120, 240);
    pinpu.ws2812_Showpice(0, 6, 120, 120, 240);
    pinpu.ws2812_Showpice(1, 5, 120, 120, 240);

    pinpu.ws2812_Showpice(4, 3, 120, 120, 240);
    pinpu.ws2812_Showpice(4, 5, 120, 120, 240);

    for (i = 9; i < 32; i++)
    {
        pinpu.ws2812_Showpice(7, i, 224, 120, 240);
    }

    FastLED.show(); // 显示灯条
    delay(1);
}

void DPLAY::ws281x_ShowIcon() // 一键三连
{
    pinpu.ws281x_ShowTemp(0, 0, 2, 1, 8);
    pinpu.ws281x_ShowTemp(0, 11, 0, 1, 8);
    pinpu.ws281x_ShowTemp(0, 11, 1, 3, 8);
    pinpu.ws281x_ShowTemp(0, 22, 3, 1, 8);
    FastLED.show(); // 显示灯条
    delay(1);
}