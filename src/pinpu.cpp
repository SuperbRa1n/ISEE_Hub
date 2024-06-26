#include "pinpu.h"
#include <FastLED.h>
#include "arduinoFFT.h"

/********************************************************************
 *如果你想调节LED亮度，请Ctrl+F搜索“FastLED.setBrightness(50)”，可将50改为0~255之间的任意数，但请注意电流，过亮可能会导致你的电源供电不足引起死机.
 *如果你想调节频谱的频响曲线，请到data.h中Ctrl+F搜索“fft_freq_boost[32]”，调节数组内的倍数来调节各个频段的增益.
 *如果你想调节频谱整体的增益，请到data.h中Ctrl+F搜索“gain=40”，更改这个值来适应你的音频环境.
 *********************************************************************************************************/
/********************FFT相关定义**********************************/
#define CHANNEL 34 // 选择音频输入IO口序号为4

arduinoFFT FFT = arduinoFFT(); // 创建FFT对象

const uint16_t samples = 256;          // 采样点数，必须为2的整数次幂
const double samplingFrequency = 8000; // Hz, 声音采样频率

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[samples]; // FFT采样输入样本数组
double vImag[samples]; // FFT运算输出数组

// FFT参数，保持默认即可
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03
/********************FFT相关定义*********************************/

/*******************灯板参数定义*********************************/
#define LED_PIN 23      // 灯板输入IO口选择
#define NUM_LEDS 256    // 灯珠数量
#define BRIGHTNESS 200  // 默认背光亮度
#define LED_TYPE WS2812 // 灯珠类型
#define COLOR_ORDER GRB // 色彩顺序
#define NUM_LEDS_X 32   // 灯板X轴数量
#define NUM_LEDS_Y 8    // 灯珠Y轴数量

CRGB leds[NUM_LEDS]; // 定义LED对象

// uint8_t rgb[8][32] =
//     {
//         {255, 254, 253, 252, 251, 250, 249, 248, 191, 190, 189, 188, 187, 186, 185, 184, 127, 126, 125, 124, 123, 122, 121, 120, 63, 62, 61, 60, 59, 58, 57, 56},
//         {247, 246, 245, 244, 243, 242, 241, 240, 183, 182, 181, 180, 179, 178, 177, 176, 119, 118, 117, 116, 115, 114, 113, 112, 55, 54, 53, 52, 51, 50, 49, 48},
//         {239, 238, 237, 236, 235, 234, 233, 232, 175, 174, 173, 172, 171, 170, 169, 168, 111, 110, 109, 108, 107, 106, 105, 104, 47, 46, 45, 44, 43, 42, 41, 40},
//         {231, 230, 229, 228, 227, 226, 225, 224, 167, 166, 165, 164, 163, 162, 161, 160, 103, 102, 101, 100, 99, 98, 97, 96, 39, 38, 37, 36, 35, 34, 33, 32},
//         {223, 222, 221, 220, 219, 218, 217, 216, 159, 158, 157, 156, 155, 154, 153, 152, 95, 94, 93, 92, 91, 90, 89, 88, 31, 30, 29, 28, 27, 26, 25, 24},
//         {215, 214, 213, 212, 211, 210, 209, 208, 151, 150, 149, 148, 147, 146, 145, 144, 87, 86, 85, 84, 83, 82, 81, 80, 23, 22, 21, 20, 19, 18, 17, 16},
//         {207, 206, 205, 204, 203, 202, 201, 200, 143, 142, 141, 140, 139, 138, 137, 136, 79, 78, 77, 76, 75, 74, 73, 72, 15, 14, 13, 12, 11, 10, 9, 8},
//         {199, 198, 197, 196, 195, 194, 193, 192, 135, 134, 133, 132, 131, 130, 129, 128, 71, 70, 69, 68, 67, 66, 65, 64, 7, 6, 5, 4, 3, 2, 1, 0},
// };

uint8_t rgb[8][32] =
    {
        {0, 15, 16, 31, 32, 47, 48, 63, 64, 79, 80, 95, 96, 111, 112, 127, 128, 143, 144, 159, 160, 175, 176, 191, 192, 207, 208, 223, 224, 239, 240, 255},
        {1, 14, 17, 30, 33, 46, 49, 62, 65, 78, 81, 94, 97, 110, 113, 126, 129, 142, 145, 158, 161, 174, 177, 190, 193, 206, 209, 222, 225, 238, 241, 254},
        {2, 13, 18, 29, 34, 45, 50, 61, 66, 77, 82, 93, 98, 109, 114, 125, 130, 141, 146, 157, 162, 173, 178, 189, 194, 205, 210, 221, 226, 237, 242, 253},
        {3, 12, 19, 28, 35, 44, 51, 60, 67, 76, 83, 92, 99, 108, 115, 124, 131, 140, 147, 156, 163, 172, 179, 188, 195, 204, 211, 220, 227, 236, 243, 252},
        {4, 11, 20, 27, 36, 43, 52, 59, 68, 75, 84, 91, 100, 107, 116, 123, 132, 139, 148, 155, 164, 171, 180, 187, 196, 203, 212, 219, 228, 235, 244, 251},
        {5, 10, 21, 26, 37, 42, 53, 58, 69, 74, 85, 90, 101, 106, 117, 122, 133, 138, 149, 154, 165, 170, 181, 186, 197, 202, 213, 218, 229, 234, 245, 250},
        {6, 9, 22, 25, 38, 41, 54, 57, 70, 73, 86, 89, 102, 105, 118, 121, 134, 137, 150, 153, 166, 169, 182, 185, 198, 201, 214, 217, 230, 233, 246, 249},
        {7, 8, 23, 24, 39, 40, 55, 56, 71, 72, 87, 88, 103, 104, 119, 120, 135, 136, 151, 152, 167, 168, 183, 184, 199, 200, 215, 216, 231, 232, 247, 248},
};

// uint8_t rgb[8][32] =
//     {
//         {248, 247, 232, 231, 216, 215, 200, 199, 184, 183, 168, 167, 152, 151, 136, 135, 120, 119, 104, 103, 88, 87, 72, 71, 56, 55, 40, 39, 24, 23, 8, 7},
//         {249, 246, 233, 230, 217, 214, 201, 198, 185, 182, 169, 166, 153, 150, 137, 134, 121, 118, 105, 102, 89, 86, 73, 70, 57, 54, 41, 38, 25, 22, 9, 6},
//         {250, 245, 234, 229, 218, 213, 202, 197, 186, 181, 170, 165, 154, 149, 138, 133, 122, 117, 106, 101, 90, 85, 74, 69, 58, 53, 42, 37, 26, 21, 10, 5},
//         {251, 244, 235, 228, 219, 212, 203, 196, 187, 180, 171, 164, 155, 148, 139, 132, 123, 116, 107, 100, 91, 84, 75, 68, 59, 52, 43, 36, 27, 20, 11, 4},
//         {252, 243, 236, 227, 220, 211, 204, 195, 188, 179, 172, 163, 156, 147, 140, 131, 124, 115, 108, 99, 92, 83, 76, 67, 60, 51, 44, 35, 28, 19, 12, 3},
//         {253, 242, 237, 226, 221, 210, 205, 194, 189, 178, 173, 162, 157, 146, 141, 130, 125, 114, 109, 98, 93, 82, 77, 66, 61, 50, 45, 34, 29, 18, 13, 2},
//         {254, 241, 238, 225, 222, 209, 206, 193, 190, 177, 174, 161, 158, 145, 142, 129, 126, 113, 110, 97, 94, 81, 78, 65, 62, 49, 46, 33, 30, 17, 14, 1},
//         {255, 240, 239, 224, 223, 208, 207, 192, 191, 176, 175, 160, 159, 144, 143, 128, 127, 112, 111, 96, 95, 80, 79, 64, 63, 48, 47, 32, 31, 16, 15, 0},
// };
void PINPU::tft_init()
{
    sampling_period_us = round(1000000 * (1.0 / samplingFrequency)); // 计算采样频率
    pinMode(CHANNEL, INPUT);                                         // 初始化麦克风接口为输入模式，表示读取麦克风数据

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS); // 初始化LED灯带
    FastLED.setBrightness(BRIGHTNESS);                               // LED亮度设置，取值范围为0-255
}

void drawBar(int idx, int16_t value, uint8_t *flag) // 绘制函数，按序号和幅度值绘制条形块
{
    int LED_NUM, x, y, i;

    static int16_t volume[32]; // 保存下降数据
    constrain(value, 0, 8);    // 幅度限制在0-8范围内

    if (volume[idx] < value) // 采集到的数据比之前大则更新，实现上冲效果
        volume[idx] = value;

    for (int i = 0; i < 8 - volume[idx]; i++)
    {
        x = idx;
        y = i;
        LED_NUM = rgb[y][x];
        leds[LED_NUM] = CRGB::Black;
    }

    if (*flag)
    {
        volume[idx] -= 1; // 达到时间则减小1，表示下落
        if (idx == 32)
            *flag = 0; // 第0-7列均更新完毕则清除标记
    }
}

void PINPU::tft_run()
{
    static uint32_t t = 0, dt = 70;
    static uint8_t flag = 0;

    /*采样*/
    microseconds = micros();
    for (int i = 0; i < samples; i++)
    {
        vReal[i] = analogRead(CHANNEL); // 读取模拟值，信号采样
        vImag[i] = 0;
        while (micros() - microseconds < sampling_period_us)
        {
            // empty loop
        }
        microseconds += sampling_period_us;
    }

    /*FFT运算*/
    FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD); /* Weigh data */
    FFT.Compute(vReal, vImag, samples, FFT_FORWARD);                 /* Compute FFT */
    FFT.ComplexToMagnitude(vReal, vImag, samples);                   /* Compute magnitudes */

    fill_rainbow((leds), 256 /*数量*/, 0 /*开始色值*/, 4 /*递增值*/); // 设置彩虹渐变，先填充满，然后根据取值大小填充黑色，表示熄灭灯
    for (int i = 0; i < 32; i++)
    {                                                                                                                                                                                                                                                                                                                                      // 循环遍历八列LED
        drawBar(i, (vImag[i * 3 + 2] + vImag[i * 3 + 3] + vImag[i * 3 + 4] + vImag[i * 3 + 5] + vImag[i * 3 + 6] + vImag[i * 3 + 7] + vImag[i * 3 + 8] + vImag[i * 3 + 9] + vImag[i * 3 + 10] + vImag[i * 3 + 11] + vImag[i * 3 + 12] + vImag[i * 3 + 13] + vImag[i * 3 + 14] + vImag[i * 3 + 15] + vImag[i * 3 + 16]) / 15 / 200, &flag); // 选取频谱中取平均后的8个值,传递时间标志到绘制函数
    }
    FastLED.show(); // 显示灯条

    if ((millis() - t) > dt)
    {                 // 读取时间，判断是否达到掉落时长
        flag = 1;     // 达到则标记为1
        t = millis(); // 更新时间
    }
    // Serial.println("yun xing");
}

/********************时钟显示相关定义**********************************/
// uint8_t rgb[8][32] =
//     {
//         {0, 15, 16, 31, 32, 47, 48, 63, 64, 79, 80, 95, 96, 111, 112, 127, 128, 143, 144, 159, 160, 175, 176, 191, 192, 207, 208, 223, 224, 239, 240, 255},
//         {1, 14, 17, 30, 33, 46, 49, 62, 65, 78, 81, 94, 97, 110, 113, 126, 129, 142, 145, 158, 161, 174, 177, 190, 193, 206, 209, 222, 225, 238, 241, 254},
//         {2, 13, 18, 29, 34, 45, 50, 61, 66, 77, 82, 93, 98, 109, 114, 125, 130, 141, 146, 157, 162, 173, 178, 189, 194, 205, 210, 221, 226, 237, 242, 253},
//         {3, 12, 19, 28, 35, 44, 51, 60, 67, 76, 83, 92, 99, 108, 115, 124, 131, 140, 147, 156, 163, 172, 179, 188, 195, 204, 211, 220, 227, 236, 243, 252},
//         {4, 11, 20, 27, 36, 43, 52, 59, 68, 75, 84, 91, 100, 107, 116, 123, 132, 139, 148, 155, 164, 171, 180, 187, 196, 203, 212, 219, 228, 235, 244, 251},
//         {5, 10, 21, 26, 37, 42, 53, 58, 69, 74, 85, 90, 101, 106, 117, 122, 133, 138, 149, 154, 165, 170, 181, 186, 197, 202, 213, 218, 229, 234, 245, 250},
//         {6, 9, 22, 25, 38, 41, 54, 57, 70, 73, 86, 89, 102, 105, 118, 121, 134, 137, 150, 153, 166, 169, 182, 185, 198, 201, 214, 217, 230, 233, 246, 249},
//         {7, 8, 23, 24, 39, 40, 55, 56, 71, 72, 87, 88, 103, 104, 119, 120, 135, 136, 151, 152, 167, 168, 183, 184, 199, 200, 215, 216, 231, 232, 247, 248},
// };
// uint8_t rgb[8][32] =
//     {
//         {248, 247, 232, 231, 216, 215, 200, 199, 184, 183, 168, 167, 152, 151, 136, 135, 120, 119, 104, 103, 88, 87, 72, 71, 56, 55, 40, 39, 24, 23, 8, 7},
//         {249, 246, 233, 230, 217, 214, 201, 198, 185, 182, 169, 166, 153, 150, 137, 134, 121, 118, 105, 102, 89, 86, 73, 70, 57, 54, 41, 38, 25, 22, 9, 6},
//         {250, 245, 234, 229, 218, 213, 202, 197, 186, 181, 170, 165, 154, 149, 138, 133, 122, 117, 106, 101, 90, 85, 74, 69, 58, 53, 42, 37, 26, 21, 10, 5},
//         {251, 244, 235, 228, 219, 212, 203, 196, 187, 180, 171, 164, 155, 148, 139, 132, 123, 116, 107, 100, 91, 84, 75, 68, 59, 52, 43, 36, 27, 20, 11, 4},
//         {252, 243, 236, 227, 220, 211, 204, 195, 188, 179, 172, 163, 156, 147, 140, 131, 124, 115, 108, 99, 92, 83, 76, 67, 60, 51, 44, 35, 28, 19, 12, 3},
//         {253, 242, 237, 226, 221, 210, 205, 194, 189, 178, 173, 162, 157, 146, 141, 130, 125, 114, 109, 98, 93, 82, 77, 66, 61, 50, 45, 34, 29, 18, 13, 2},
//         {254, 241, 238, 225, 222, 209, 206, 193, 190, 177, 174, 161, 158, 145, 142, 129, 126, 113, 110, 97, 94, 81, 78, 65, 62, 49, 46, 33, 30, 17, 14, 1},
//         {255, 240, 239, 224, 223, 208, 207, 192, 191, 176, 175, 160, 159, 144, 143, 128, 127, 112, 111, 96, 95, 80, 79, 64, 63, 48, 47, 32, 31, 16, 15, 0},
// };
// uint8_t rgb[8][32] =
//     {
//         {255, 247, 239, 231, 223, 215, 207, 199, 191, 183, 175, 167, 159, 151, 143, 135, 127, 119, 111, 103, 95, 87, 79, 71, 63, 55, 47, 39, 31, 23, 15, 7},
//         {254, 246, 238, 230, 222, 214, 206, 198, 190, 182, 174, 166, 158, 150, 142, 134, 126, 118, 110, 102, 94, 86, 78, 70, 62, 54, 46, 38, 30, 22, 14, 6},
//         {253, 245, 237, 229, 221, 213, 205, 197, 189, 181, 173, 165, 157, 149, 141, 133, 125, 117, 109, 101, 93, 85, 77, 69, 61, 53, 45, 37, 29, 21, 13, 5},
//         {252, 244, 236, 228, 220, 212, 204, 196, 188, 180, 172, 164, 156, 148, 140, 132, 124, 116, 108, 100, 92, 84, 76, 68, 60, 52, 44, 36, 28, 20, 12, 4},
//         {251, 243, 235, 227, 219, 211, 203, 195, 187, 179, 171, 163, 155, 147, 139, 131, 123, 115, 107, 99,  91, 83, 75, 67, 59, 51, 43, 35, 27, 19, 11, 3},
//         {250, 242, 234, 226, 218, 210, 202, 194, 186, 178, 170, 162, 154, 146, 138, 130, 122, 114, 106, 98,  90, 82, 74, 66, 58, 50, 42, 34, 26, 18, 10, 2},
//         {249, 241, 233, 225, 217, 209, 201, 193, 185, 177, 169, 161, 153, 145, 137, 129, 121, 113, 105, 97,  89, 81, 73, 65, 57, 49, 41, 33, 25, 17, 9,  1},
//         {248, 240, 232, 224, 216, 208, 200, 192, 184, 176, 168, 160, 152, 144, 136, 128, 120, 112, 104, 96,  88, 80, 72, 64, 56, 48, 40, 32, 24, 16, 8,  0},
// };
// uint8_t rgb[8][32] =
//     {
//         {0, 15, 23, 31, 39, 47, 55, 63, 71, 79, 87, 95, 103, 111, 119, 127, 135, 143, 151, 159, 167, 175, 183, 191, 199, 207, 215, 223, 231, 239, 247, 255},
//         {1, 14, 22, 30, 38, 46, 54, 62, 70, 78, 86, 94, 102, 110, 118, 126, 134, 142, 150, 158, 166, 174, 182, 190, 198, 206, 214, 222, 230, 238, 246, 254},
//         {2, 13, 21, 29, 37, 45, 53, 61, 69, 77, 85, 93, 101, 109, 117, 125, 133, 141, 149, 157, 165, 173, 181, 189, 197, 205, 213, 221, 229, 237, 245, 253},
//         {3, 12, 20, 28, 36, 44, 52, 60, 68, 76, 84, 92, 100, 108, 116, 124, 132, 140, 148, 156, 164, 172, 180, 188, 196, 204, 212, 220, 228, 236, 244, 252},
//         {4, 11, 19, 27, 35, 43, 51, 59, 67, 75, 83, 91, 99,  107, 115, 123, 131, 139, 147, 155, 163, 171, 179, 187, 195, 203, 211, 219, 227, 235, 243, 251},
//         {5, 10, 18, 26, 34, 42, 50, 58, 66, 74, 82, 90, 98,  106, 114, 122, 130, 138, 146, 154, 162, 170, 178, 186, 194, 202, 210, 218, 226, 234, 242, 250},
//         {6, 9,  17, 25, 33, 41, 49, 57, 65, 73, 81, 89, 97,  105, 113, 121, 129, 137, 145, 153, 161, 169, 177, 185, 193, 201, 209, 217, 225, 233, 241, 249},
//         {7, 8,  16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96,  104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248},
// };

uint8_t asc2_0603[95][3] =
    {

        {0x7C, 0x44, 0x7C}, /*"0",0*/
        {0x00, 0x7C, 0x00}, /*"1",1*/
        {0x5C, 0x54, 0x74}, /*"2",2*/
        {0x54, 0x54, 0x7C}, /*"3",3*/
        {0x70, 0x10, 0x7C}, /*"4",4*/
        {0x74, 0x54, 0x5C}, /*"5",5*/
        {0x7C, 0x54, 0x5C}, /*"6",6*/
        {0x40, 0x40, 0x7C}, /*"7",7*/
        {0x7C, 0x54, 0x7C}, /*"8",8*/
        {0x74, 0x54, 0x7C}, /*"9",9*/

        // {0x7F, 0x41, 0x7F}, /*"0",0*/
        // {0x00, 0x7F, 0x00}, /*"1",1*/
        // {0x4F, 0x49, 0x79}, /*"2",2*/
        // {0x49, 0x49, 0x7F}, /*"3",3*/
        // {0x78, 0x08, 0x7F}, /*"4",4*/
        // {0x79, 0x49, 0x4F}, /*"5",5*/
        // {0x7F, 0x49, 0x4F}, /*"6",6*/
        // {0x40, 0x40, 0x7F}, /*"7",7*/
        // {0x7F, 0x49, 0x7F}, /*"8",8*/
        // {0x79, 0x49, 0x7F}, /*"9",9*/
};

uint8_t asc2_0804[10][4] =
    {

        {0x5E, 0x21, 0x21, 0x21}, /*"摄氏度图标",0*/

};

uint8_t Temp[10][8] =
    {

        {0x00, 0x00, 0x3E, 0x2A, 0x2A, 0x14, 0x00, 0x00}, /*"B币",0*/
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x6B, 0x3E, 0x00}, /*"B币边框",1*/
        {0xFF, 0xF1, 0xF1, 0xE1, 0x81, 0x81, 0xE1, 0xE1}, /*"点赞",2*/
        {0xF7, 0xE0, 0xC1, 0x83, 0xC1, 0xE0, 0xF7, 0xFF}, /*"收藏",3*/

};

// 设置rgb显示的颜色
void set_rgb_color(uint8_t x, uint8_t y)
{
    int LED_NUM;
    LED_NUM = rgb[x][y];
    CHSV hsv;
    hsv.hue = LED_NUM;
    hsv.val = 255;
    hsv.sat = 240;

    leds[LED_NUM] = hsv;
}

//点亮单个点 
//设置rgb显示的颜色
// E:色调 S:饱和度（调节饱和度可调节颜色的深浅） L：亮度
void set_ryrgb_color(uint8_t x, uint8_t y, uint8_t e, uint8_t s, uint8_t l)
{
    int LED_NUM;
    LED_NUM = rgb[x][y];
    CHSV hsv;
    hsv.hue = e;
    hsv.sat = s;
    hsv.val = l;

    leds[LED_NUM] = hsv;
}

// 画点
// x:0~32
// y:0~8
// t:颜色选择
// GRBcolor: 颜色设置
void PINPU::ws281x_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    int LED_NUM;
    if (t == 1)

    {
        set_ryrgb_color(x, y, 20, 240, 240);
    }
    else if (t == 2)

    {
        set_ryrgb_color(x, y, 20, 160, 240);
    }
    else if (t == 3)

    {
        set_ryrgb_color(x, y, 120, 120, 240);
    }
    else if (t == 4)

    {
        set_ryrgb_color(x, y, 180, 120, 240);
    }

    else
    {
        LED_NUM = rgb[x][y];
        leds[LED_NUM] = CRGB::Black;
    }
}

void PINPU::ws281x_closeAll(void) // 关闭全部的灯
{
    uint16_t i;
    uint8_t j;
    int LED_NUM;
    for (i = 0; i < NUM_LEDS_Y; i++)
    {
        for (j = 0; j < NUM_LEDS_X; j++)
        {
            LED_NUM = rgb[i][j];
            leds[LED_NUM] = CRGB::Black;
        }
    }
    FastLED.show(); // 显示灯条
    delay(10);
}

// 在指定位置显示一个字符,包括部分字符
// x:0~8
// y:0~32
// mode:0,反白显示;1,正常显示
// size:选择字体 8
void PINPU::ws281x_Showchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t t, uint8_t size)
{
    uint8_t temp, s, t1;
    uint8_t x0 = x;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); // 得到字体一个字符对应点阵集所占的字节数
    for (s = 0; s < csize; s++)
    {
        if (csize == 3)
            temp = asc2_0603[chr][s];
        else
            temp = asc2_0804[chr][s];
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
            {
                ws281x_DrawPoint(x, y, t);
            }
            else
                ws281x_DrawPoint(x, y, !t);
            temp <<= 1;
            x++;
            if ((x - x0) == 8)
            {
                x = x0;
                y++;
                break;
            }
        }
    }
}

// m^n函数
uint32_t PINPU::mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}

// 显示数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// num:数值(0~4294967295);
void PINPU::ws2812_ShowNum(uint8_t x, uint8_t y, uint64_t num, uint8_t len, uint8_t corol, uint8_t size)
{
    uint8_t t, temp;
    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;

        ws281x_Showchar(x, y + (size / 2 + 1) * t, temp, corol, size);
    }
}

void PINPU::ws2812_Showpice(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
    set_ryrgb_color(x, y, r, g, b);
}

// 在指定位置显示图标
// x:0~8
// y:0~32
// chr:图标序号
// t:图标颜色
// size:图标字符长度
void PINPU::ws281x_ShowTemp(uint8_t x, uint8_t y, uint8_t chr, uint8_t t, uint8_t size)
{
    uint8_t temp, s, t1;
    uint8_t x0 = x;

    for (s = 0; s < size; s++)
    {
        if (size == 8)
            temp = Temp[chr][s];
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
            {
                ws281x_DrawPoint(x, y, t);
            }
            else
                ws281x_DrawPoint(x, y, !t);
            temp <<= 1;
            x++;
            if ((x - x0) == 8)
            {
                x = x0;
                y++;
                break;
            }
        }
    }
}