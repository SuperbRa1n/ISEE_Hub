/**
 * @file rgb_show.h
 *
 */

#ifndef RGB_SHOW_H
#define RGB_SHOW_H

/*********************
 *      INCLUDES
 *********************/
#include <Arduino.h>
#include <pinpu.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
    int year;
    int mon;
    int mday;
    int wday;
    int hour;
    int min;
    int sec;
} _calendar_obj;
extern _calendar_obj calendar; // 日历结构体

typedef struct
{
    int r_year;
    int r_mon;
    int r_mday;
    int r_wday;
    int r_hour;
    int r_min;
    int r_sec;
} _RTC_obj;
extern _RTC_obj r_calendar; // rtc时钟结构体
/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

class DPLAY
{
private:
public:
    PINPU pinpu;
    void weather_select(void);
    void ws2812_Showrain(void);
    void ws2812_Showsun(void);
    void ws2812_Showyin(void);
    void ws2812_ShowData(void);
    void Show_Timer(void);
    void Show_Year(void);
    void Show_Temp(void);
    void Show_Bfans();
    void ws281x_ShowIcon();
};

#endif /*RGB_SHOW_H*/
