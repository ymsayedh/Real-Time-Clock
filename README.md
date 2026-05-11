# Real-Time Clock (RTC) — DS1307 + ATMEGA32

A full-featured embedded clock system built in C for the ATMEGA32 microcontroller. Displays live time and date on a 16×2 LCD, with alarm, stopwatch, and countdown timer functionality controlled via a 4×4 keypad and dedicated push buttons.

---

## Features

- **Live clock display** — shows current time (HH:MM:SS) and date (DD/MM/YY) on the LCD
- **Set Time / Set Date** — configure the RTC via keypad input
- **Alarm system** — set up to 4 independent alarms; buzzer rings until manually stopped
- **Stopwatch** — start, stop, and reset with dedicated buttons
- **Countdown timer** — enter hours/minutes/seconds; buzzer sounds when time is up

---

## Hardware

| Component | Details |
|---|---|
| Microcontroller | ATMEGA32 (8MHz) |
| Real-Time Clock | DS1307 (I2C, 0xD0) |
| Display | 16×2 LCD (4-bit mode) |
| Input | 4×4 Matrix Keypad |
| Buttons | 10 push buttons (active LOW, pull-up) |
| Buzzer | Active buzzer on PB1 |

### Pin Mapping

**LCD** (4-bit mode, upper nibble of PORTB)

| LCD Signal | MCU Pin |
|---|---|
| Data (D4–D7) | PB4–PB7 |
| RS | PD6 |
| EN | PD7 |

**Keypad** (PORTA)

| | |
|---|---|
| Rows (output) | PA4–PA7 |
| Columns (input) | PA0–PA3 |

**Buttons — PORTD** (active LOW, internal pull-up)

| Button | Pin |
|---|---|
| Start Stopwatch | PD0 |
| Stop Stopwatch | PD1 |
| Reset Stopwatch | PD2 |
| Exit Mode | PD3 |
| Start Countdown | PD4 |

**Buttons — PORTC** (active LOW, internal pull-up)

| Button | Pin |
|---|---|
| Set Date | PC2 |
| Set Time | PC3 |
| Set Alarm | PC4 |
| View Alarms | PC5 |
| Stop Alarm | PC6 |

**Other**

| Component | Pin |
|---|---|
| Buzzer | PB1 |
| DS1307 SDA | PC1 (TWI) |
| DS1307 SCL | PC0 (TWI) |

---

## Project Structure

```
├── main.c        — Application logic, mode switching, alarm management
├── rtc.c / .h    — DS1307 read/write (BCD conversion, I2C communication)
├── i2c.c / .h    — Low-level TWI/I2C driver
├── lcd.c / .h    — 16×2 LCD driver (4-bit mode)
├── keypad.c / .h — 4×4 matrix keypad scanner
├── timer.c / .h  — Hardware Timer1 (CTC mode) for countdown ISR
├── dio.c / .h    — Button initialization and reading
```

---

## Development Environment

This project was developed using **Eclipse IDE for C/C++ Developers** with the **AVR Eclipse Plugin** (`de.innot.avreclipse`) and the **WinAVR toolchain** (AVR-GCC on Windows).

### Opening the project in Eclipse

1. Clone or download this repository
2. Open Eclipse and go to `File → Open Projects from File System`
3. Select the project folder — Eclipse will detect it automatically from the `.project` and `.cproject` files
4. Right-click the project → `Properties → AVR → Target Hardware`
   - Set MCU: **ATmega32**
   - Set Clock: **8000000 Hz** (8 MHz) — or 16 MHz if your board uses a 16 MHz crystal (update `OCR1A` and `TWBR` accordingly)
5. Build: `Project → Build Project` (or `Ctrl+B`)
6. Flash: `AVR → Upload Project to Target Device` (requires AVRDude + programmer)

---

## How to Use

### Default screen
After power-on, the LCD shows:
```
Time: HH:MM:SS
Date: DD/MM/YY
```
The display refreshes every second.

---

### Set Time (PC3 button)
1. Press **Set Time**
2. Enter hours (00–23) on the keypad → 2 digits
3. Enter minutes (00–59) → 2 digits
4. Enter seconds (00–59) → 2 digits
5. The RTC is updated immediately and the screen returns to clock view

---

### Set Date (PC2 button)
1. Press **Set Date**
2. Enter year (00–99) → 2 digits
3. Enter month (01–12) → 2 digits
4. Enter day → 2 digits (range validated by month: Feb max 29, 30-day months capped at 30)
5. Screen returns to clock view

---

### Alarm (PC4 / PC5 / PC6 buttons)

**Add an alarm (PC4):**
1. Press **Set Alarm**
2. Enter hours (00–23) → 2 digits
3. Enter minutes (00–59) → 2 digits
4. Alarm is saved. Up to **4 alarms** can be active at once. Duplicate alarms are rejected.

**View alarms (PC5):**
- Press **View Alarms** to scroll through active alarms on the LCD (2 at a time, 2-second pause between pages)
- Shows "No Active Alarms" if none are set

**Stop a ringing alarm (PC6):**
- When an alarm triggers, the LCD shows `ALARM!!!` and the buzzer sounds
- Press **Stop Alarm** to silence it. The alarm is then deactivated.

> Alarms are stored in RAM — they reset on power-off.

---

### Stopwatch (PD0 / PD1 / PD2 / PD3 buttons)
1. Press **Start Stopwatch** (PD0) to enter stopwatch mode
2. Press **Start** (PD0) to begin counting
3. Press **Stop** (PD1) to pause
4. Press **Reset** (PD2) to zero the counter
5. Press **Exit** (PD3) to return to the clock display

---

### Countdown Timer (PD4 button)
1. Press **Start Countdown** (PD4) to enter countdown mode
2. Enter hours → minutes → seconds via keypad (2 digits each)
3. Press **Exit** (PD3) at any prompt to cancel and return to clock view
4. The LCD counts down in real time
5. When it reaches 00:00:00, the buzzer sounds for 5 seconds, then returns to clock view

---

### Exit any mode (PD3 button)
Press **Exit Mode** (PD3) at any point inside Stopwatch or Countdown modes to immediately return to the default clock display.
