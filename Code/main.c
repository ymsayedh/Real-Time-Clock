#include <avr/io.h>
#include <util/delay.h>
#include "dio.h"
#include "lcd.h"
#include "rtc.h"
#include "timer.h"
#include "keypad.h"


uint8_t Exit_Mode() {
    if (Button_Read('D', EXIT_MODE)) {  // Specify PORTD
        _delay_ms(50); // Debounce delay
        while (Button_Read('D', EXIT_MODE));  // Wait for button release
        return 1;  // Exit signal
    }
    return 0;
}

uint8_t Get_Valid_Input(uint8_t min, uint8_t max) {
    uint8_t value = 0;
    char key;
    uint8_t count = 0;

    while (count < 2) {
        key = Keypad_GetKey();  // Wait for a key press
        if (key >= '0' && key <= '9') {
            value = (value * 10) + (key - '0'); // Convert ASCII to number
            LCD_Char(key); // Display number
            count++;

            // Wait until key is released to prevent multiple inputs
            while (Keypad_GetKey() == key);
            _delay_ms(200);  // Debounce delay
        }
    }

    // Ensure value is within valid range
    if (value < min || value > max) {
        LCD_Clear();
        LCD_String("Invalid!   ");
        _delay_ms(1000);
        return Get_Valid_Input(min, max);  // Retry until valid input
    }

    return value;
}

void Display_Time_Date() {
    uint8_t hours, minutes, seconds;
    uint8_t day, month, year;

    RTC_GetTime(&hours, &minutes, &seconds);
    RTC_GetDate(&day, &month, &year);

    LCD_Clear();
    LCD_String("Time: ");
    LCD_Char((hours / 10) + '0');
    LCD_Char((hours % 10) + '0');
    LCD_Char(':');
    LCD_Char((minutes / 10) + '0');
    LCD_Char((minutes % 10) + '0');
    LCD_Char(':');
    LCD_Char((seconds / 10) + '0');
    LCD_Char((seconds % 10) + '0');

    LCD_Command(0xC0);
    LCD_String("Date: ");
    LCD_Char((day / 10) + '0');
    LCD_Char((day % 10) + '0');
    LCD_Char('/');
    LCD_Char((month / 10) + '0');
    LCD_Char((month % 10) + '0');
    LCD_Char('/');
    LCD_Char((year / 10) + '0');
    LCD_Char((year % 10) + '0');
}

void Exit_To_Default() {
    LCD_Clear();
    Display_Time_Date();
    _delay_ms(1000);  // Short delay before resuming loop
}

void Countdown_Mode() {
    LCD_Clear();
    LCD_String("Set Timer!!");
    _delay_ms(1000);

    uint8_t h = 0, m = 0, s = 0;
    char key;
    uint8_t count = 0;

    LCD_Clear();
    LCD_String("Hours:");
    while (count < 2) {
        if (Exit_Mode()) { Exit_To_Default(); return; }
        key = Keypad_GetKey();
        if (key >= '0' && key <= '9') {
            h = (h * 10) + (key - '0');
            LCD_Char(key);
            count++;
        }
    }

    LCD_Clear();
    LCD_String("Minutes:");
    count = 0;
    while (count < 2) {
        if (Exit_Mode()) { Exit_To_Default(); return; }
        key = Keypad_GetKey();
        if (key >= '0' && key <= '9') {
            m = (m * 10) + (key - '0');
            LCD_Char(key);
            count++;
        }
    }

    LCD_Clear();
    LCD_String("Seconds:");
    count = 0;
    while (count < 2) {
        if (Exit_Mode()) { Exit_To_Default(); return; }
        key = Keypad_GetKey();
        if (key >= '0' && key <= '9') {
            s = (s * 10) + (key - '0');
            LCD_Char(key);
            count++;
        }
    }

    Timer_Set(h, m, s);
    Timer_Start();

    LCD_Clear();
    LCD_String("Time Left");

    while (1) {
        if (Exit_Mode()) { Exit_To_Default(); return; }

        LCD_Command(0xC0);
        LCD_Char((h / 10) + '0');
        LCD_Char((h % 10) + '0');
        LCD_Char(':');
        LCD_Char((m / 10) + '0');
        LCD_Char((m % 10) + '0');
        LCD_Char(':');
        LCD_Char((s / 10) + '0');
        LCD_Char((s % 10) + '0');

        if (h == 0 && m == 0 && s == 0) break;
        _delay_ms(1000);

        if (s > 0) s--;
        else if (m > 0) { m--; s = 59; }
        else if (h > 0) { h--; m = 59; s = 59; }
    }

    LCD_Clear();
    LCD_String("Time's Up!");
    PORTB |= (1 << BUZZER_PIN);
    _delay_ms(5000);
    PORTB &= ~(1 << BUZZER_PIN);

    Exit_To_Default();  // Return to date & time display
}


// Updated Stopwatch Mode
void Stopwatch_Mode() {
    uint8_t h = 0, m = 0, s = 0;
    uint8_t running = 0;

    LCD_Clear();
    LCD_String("Stopwatch Mode!!");
    _delay_ms(1000);

    while (1) {
        if (Exit_Mode()) { Exit_To_Default(); return; }

        if (Button_Read('D', START_STOPWATCH)) running = 1;
        if (Button_Read('D', STOP_STOPWATCH)) running = 0;
        if (Button_Read('D', RESET_STOPWATCH)) h = m = s = 0;


        if (running) {
            s++;
            if (s == 60) { s = 0; m++; }
            if (m == 60) { m = 0; h++; }
        }

        LCD_Clear();
        LCD_Char((h / 10) + '0');
        LCD_Char((h % 10) + '0');
        LCD_Char(':');
        LCD_Char((m / 10) + '0');
        LCD_Char((m % 10) + '0');
        LCD_Char(':');
        LCD_Char((s / 10) + '0');
        LCD_Char((s % 10) + '0');
        _delay_ms(1000);
    }

    Exit_To_Default();  // Return to date & time display
}

// Updated Set Time Function
void Set_Time() {
    LCD_Clear();
    LCD_String("Set Time!!");
    _delay_ms(1000);

    uint8_t h = 0, m = 0, s = 0;

    LCD_Clear();
    LCD_String("Set Hours: ");
    h = Get_Valid_Input(0, 23);  // Allow only 0-23

    LCD_Clear();
    LCD_String("Set Minutes: ");
    m = Get_Valid_Input(0, 59);  // Allow only 0-59

    LCD_Clear();
    LCD_String("Set Seconds: ");
    s = Get_Valid_Input(0, 59);  // Allow only 0-59

    RTC_SetTime(h, m, s);

    LCD_Clear();
    LCD_String("Time Set!");
    _delay_ms(1000);

    Exit_To_Default();  // Return to date & time display
}


void Set_Date() {
    LCD_Clear();
    LCD_String("Set Date!!");
    _delay_ms(1000);

    uint8_t d = 0, m = 0, y = 0;

    LCD_Clear();
    LCD_String("Set Year: ");
    y = Get_Valid_Input(0, 99);  // Accepts 00-99

    LCD_Clear();
    LCD_String("Set Month: ");
    m = Get_Valid_Input(1, 12);  // Only allows 01-12

    LCD_Clear();
    LCD_String("Set Day: ");
    if (m == 2) {
        d = Get_Valid_Input(1, 29);  // February max 29
    } else if (m == 4 || m == 6 || m == 9 || m == 11) {
        d = Get_Valid_Input(1, 30);  // 30-day months
    } else {
        d = Get_Valid_Input(1, 31);  // Other months max 31
    }

    RTC_SetDate(d, m, y);

    LCD_Clear();
    LCD_String("Date Set!");
    _delay_ms(1000);

    Exit_To_Default();  // Return to default mode (showing date/time)
}



#define MAX_ALARMS 4

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t active;
} Alarm;

Alarm alarms[MAX_ALARMS];
uint8_t alarm_count = 0;

void Add_Alarm() {
    if (alarm_count >= MAX_ALARMS) {
        LCD_Clear();
        LCD_String("Alarm Full!");
        _delay_ms(1000);
        return;
    }

    LCD_Clear();
    LCD_String("Set Alarm!");
    _delay_ms(1000);

    uint8_t h, m;

    LCD_Clear();
    LCD_String("Set Hours:");
    h = Get_Valid_Input(0, 23);

    LCD_Clear();
    LCD_String("Set Minutes:");
    m = Get_Valid_Input(0, 59);

    // Check for duplicate alarm
    for (uint8_t i = 0; i < alarm_count; i++) {
        if (alarms[i].hours == h && alarms[i].minutes == m) {
            LCD_Clear();
            LCD_String("Alarm Exists!");
            _delay_ms(1000);
            return;
        }
    }

    // Add new alarm
    alarms[alarm_count].hours = h;
    alarms[alarm_count].minutes = m;
    alarms[alarm_count].active = 1;
    alarm_count++;

    LCD_Clear();
    LCD_String("Alarm Set!");
    _delay_ms(1000);
}


void Check_Alarms() {
    uint8_t h, m, s;
    RTC_GetTime(&h, &m , &s);  // Get only hours and minutes

    for (uint8_t i = 0; i < alarm_count; i++) {
        if (alarms[i].active && alarms[i].hours == h && alarms[i].minutes == m) {
            LCD_Clear();
            LCD_String("ALARM!!!");
            PORTB |= (1 << BUZZER_PIN);  // Turn on buzzer

            while (Button_Read('C', STOP_ALARM) == 0) {
                _delay_ms(100);  // Wait until user presses STOP_ALARM (PC6)
            }

            PORTB &= ~(1 << BUZZER_PIN);  // Turn off buzzer
            alarms[i].active = 0;  // Disable alarm after ringing

            LCD_Clear();
            LCD_String("Alarm Stopped");
            _delay_ms(1000);
        }
    }
}

void Display_Alarms() {
    if (!Button_Read('C', VIEW_ALARMS)) return;  // Show only when PC5 is pressed

    LCD_Clear();
    uint8_t displayed = 0;

    for (uint8_t i = 0; i < alarm_count && displayed < MAX_ALARMS; i++) {
        if (alarms[i].active) {
            if (displayed == 2) {
                _delay_ms(2000);  // Pause before showing next set
                LCD_Clear();
            }

            uint8_t row = (displayed < 2) ? 0x80 : 0xC0;  // First or second row
            LCD_Command(row + (displayed % 2) * 8);  // Positioning

            // Display alarm time HH:MM
            LCD_Char((alarms[i].hours / 10) + '0');
            LCD_Char((alarms[i].hours % 10) + '0');
            LCD_Char(':');
            LCD_Char((alarms[i].minutes / 10) + '0');
            LCD_Char((alarms[i].minutes % 10) + '0');

            displayed++;
        }
    }

    if (displayed == 0) {  // No active alarms
        LCD_Clear();
        LCD_String("No Active Alarms");
    }

    _delay_ms(3000);  // Hold display before clearing
    LCD_Clear();
}


int main() {
    DIO_Init();
    LCD_Init();
    RTC_Init();
    Timer_Init();
    Keypad_Init();

    LCD_String("RTC Timer");
    _delay_ms(1000);

    while (1) {
        if (Button_Read('D', START_STOPWATCH)) Stopwatch_Mode();
        if (Button_Read('D', START_COUNTDOWN)) Countdown_Mode();
        if (Button_Read('C', SET_TIME)) Set_Time();
        if (Button_Read('C', SET_DATE)) Set_Date();
        if (Button_Read('C', SET_ALARM)) Add_Alarm();
        if (Button_Read('C', VIEW_ALARMS)) Display_Alarms();
        if (Button_Read('D', EXIT_MODE)) Exit_Mode();

        Check_Alarms();
        Display_Time_Date();
        _delay_ms(1000);
    }
}
