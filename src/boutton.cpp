#include <Arduino.h>
#include <boutton.h>

// Setup a new OneButton on pin A1.
OneButton button1(12, HIGH);
// Setup a new OneButton on pin A2.
OneButton button2(13, HIGH);
// Setup a new OneButton on pin A1.
OneButton button3(14, HIGH);
// Setup a new OneButton on pin A2.
OneButton button4(27, HIGH);
int flagShow=0, UpdateShow=0;

// This function will be called when the button1 was pressed 1 time (and no 2. button press followed).
void click1()
{
    flagShow = 1;
    // UpdateShow = 1;
    Serial.println("Button 1 click.");
} // click1

// This function will be called when the button1 was pressed 2 times in a short timeframe.
void doubleclick1()
{
    flagShow = 2;
    // UpdateShow = 1;
    Serial.println("Button 1 doubleclick.");
} // doubleclick1

// This function will be called once, when the button1 is pressed for a long time.
void longPressStart1()
{

    Serial.println("Button 1 longPress start");
} // longPressStart1

// This function will be called often, while the button1 is pressed for a long time.
void longPress1()
{

    Serial.println("Button 1 longPress...");
} // longPress1

// This function will be called once, when the button1 is released after beeing pressed for a long time.
void longPressStop1()
{

    Serial.println("Button 1 longPress stop");
} // longPressStop1

// ... and the same for button 2:
void click2()
{
    flagShow = 3;
    // UpdateShow = 1;

    Serial.println("Button 2 click.");
} // click2

void doubleclick2()
{
    flagShow = 4;
    // UpdateShow = 1;

    Serial.println("Button 2 doubleclick.");
} // doubleclick2

void longPressStart2()
{
    Serial.println("Button 2 longPress start");
} // longPressStart2

void longPress2()
{
    Serial.println("Button 2 longPress...");
} // longPress2

void longPressStop2()
{
    Serial.println("Button 2 longPress stop");
} // longPressStop2

// ... and the same for button 3:
void click3()
{
    flagShow = 5;
    // UpdateShow = 1;

    Serial.println("Button 3 click.");
} // click3

void doubleclick3()
{
    flagShow = 6;
    // UpdateShow = 1;

    Serial.println("Button 3 doubleclick.");
} // doubleclick3

void longPressStart3()
{
    Serial.println("Button 3 longPress start");
} // longPressStart3

void longPress3()
{
    Serial.println("Button 3 longPress...");
} // longPress3

void longPressStop3()
{
    Serial.println("Button 3 longPress stop");
} // longPressStop3

// ... and the same for button 4:
void click4()
{
    flagShow = 7;
    // UpdateShow = 1;

    Serial.println("Button 4 click.");
} // click4

void doubleclick4()
{
    flagShow = 8;
    // UpdateShow = 1;

    Serial.println("Button 4 doubleclick.");
} // doubleclick4

void longPressStart4()
{
    Serial.println("Button 4 longPress start");
} // longPressStart4

void longPress4()
{
    Serial.println("Button 4 longPress...");
} // longPress4

void longPressStop4()
{
    Serial.println("Button 4 longPress stop");
} // longPressStop4

void bouton::boutton_tick()
{

    button1.tick();
    button2.tick();
    button3.tick();
    button4.tick();
} // ----- button 1 callback functions

void bouton::boutton_init()
{
    // button1.attachClick(&bouton::click1);
    button1.attachClick(click1);
    button1.attachDoubleClick(doubleclick1);
    button1.attachLongPressStart(longPressStart1);
    button1.attachLongPressStop(longPressStop1);
    button1.attachDuringLongPress(longPress1);

    // link the button 2 functions.
    button2.attachClick(click2);
    button2.attachDoubleClick(doubleclick2);
    button2.attachLongPressStart(longPressStart2);
    button2.attachLongPressStop(longPressStop2);
    button2.attachDuringLongPress(longPress2);

    // link the button 3 functions.
    button3.attachClick(click3);
    button3.attachDoubleClick(doubleclick3);
    button3.attachLongPressStart(longPressStart3);
    button3.attachLongPressStop(longPressStop3);
    button3.attachDuringLongPress(longPress3);

    // link the button 2 functions.
    button4.attachClick(click4);
    button4.attachDoubleClick(doubleclick4);
    button4.attachLongPressStart(longPressStart4);
    button4.attachLongPressStop(longPressStop4);
    button4.attachDuringLongPress(longPress4);
}
