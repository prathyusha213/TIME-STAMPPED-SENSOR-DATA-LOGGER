
## LPC2148-TIME STAMPPED SENSOR DATA LOGGER


**Objective**

The objective of this project is to design and develop a Time-Stamped Sensor Data Logger using the LPC2148 ARM7 microcontroller that continuously measures temperature using the LM35 sensor, records each reading with the current date and time from the RTC, displays the information on a 16×2 LCD, and sends the logged data to a PC through UART for monitoring and storage.


---

## Features

- Real-time temperature monitoring using **LM35** sensor.
- On-chip **RTC** provides date and time for every sensor reading.
- UART communication sends timestamped sensor data to the PC.
- 16x2 LCD displays live temperature, date, and time.
- 4x4 Keypad allows editing of time, date, and temperature set point.
- Push button enters **Edit Mode**.
- LED/Buzzer alerts when temperature exceeds the set point.
- Temperature data is logged with accurate timestamps.
- Continuous real-time monitoring and data logging.

---

## Project Block Diagram

![Project Block Diagram](Screenshot%202026-09-13%20091038.png)

---

## Hardware Components Used

- LPC2148 ARM7 Microcontroller
- LM35 Temperature Sensor
- 16x2 LCD Display
- 4x4 Matrix Keypad
- Push Button
- LED
- Buzzer
- MAX232 Serial Interface
- Power Supply (3.3V / 5V)

---

## Software Used

- Keil µVision
- Flash Magic
- Embedded C Programming
- Flash Magic Serial Terminal

---

# Working of the Project

## Step 1: System Initialization

When power is supplied to the LPC2148 microcontroller, it initializes all the required peripherals and hardware modules.

- LCD is initialized to display system messages and sensor readings.
- UART0 is initialized for serial communication with the PC through MAX232.
- On-chip RTC is initialized with the default or previously updated date and time.
- ADC is initialized to read the analog output from the LM35 temperature sensor.
- GPIO pins are initialized for the keypad, push button, LED, and buzzer.

After successful initialization, the LCD displays that the system is ready for monitoring.

---

## Step 2: Temperature Measurement Using LM35

The LM35 sensor continuously measures the surrounding temperature and generates an analog voltage proportional to the temperature.

- LM35 output is connected to the ADC channel of LPC2148.
- ADC converts the analog voltage into a digital value.
- LPC2148 converts the ADC value into temperature in degrees Celsius (°C).

The temperature is updated continuously every second.

---

## Step 3: Real-Time Clock (RTC) Operation

The LPC2148 uses its internal **Real-Time Clock (RTC)** to maintain the current date and time.

- RTC starts after initialization.
- RTC continuously counts seconds, minutes, hours, days, months, and years.
- Every temperature reading is associated with the current RTC timestamp.

This provides accurate date and time information for sensor logging.

---

## Step 4: LCD Display

The 16x2 LCD continuously displays real-time system information.

The LCD shows:

- Current temperature.
- Current time.
- Current date.
- System status (Monitoring / Alert / Edit Mode).

The LCD updates every second for real-time monitoring.

---

## Step 5: Creating Timestamped Sensor Data

The LPC2148 reads both temperature and RTC values.

The controller combines these values into a formatted message containing:

- Date
- Time
- Temperature
- Status (NORMAL or ALERT)

Example:

```text
13/09/2026 10:45:00 Temp=30.5°C Status=NORMAL
```

This formatted message becomes the timestamped sensor log.

---

## Step 6: UART Data Logging

The timestamped sensor message is transmitted to the PC through UART.

- UART0 sends data to the Flash Magic Serial Terminal.
- Data is transmitted every minute.
- The PC receives temperature logs along with the corresponding timestamp.

Example Serial Terminal Output:

```text
TIME STAMPPED SENSOR DATA LOGGER

13/09/2026 10:45:00 Temp=30.5°C Status=NORMAL
13/09/2026 10:46:00 Temp=31.0°C Status=NORMAL
13/09/2026 10:47:00 Temp=45.8°C Status=ALERT
13/09/2026 10:48:00 Temp=44.2°C Status=NORMAL
```

This creates a complete temperature history with date and time.

---

## Step 7: Temperature Set Point

A predefined temperature limit is stored in the program.

Example:

```text
Temperature Set Point = 45°C
```

- LPC2148 continuously compares the measured temperature with this set point.
- The user can modify the temperature set point using the keypad during Edit Mode.

---

## Step 8: Threshold Comparison and Alert Generation

The controller continuously checks whether the measured temperature exceeds the predefined set point.

**Condition:** Temperature greater than the set point.

If TRUE:

- LED turns ON.
- Buzzer turns ON.
- LCD displays a HIGH TEMPERATURE ALERT message.
- UART transmits the log with ALERT status.

Example:

```text
13/09/2026 10:47:00 Temp=46.8°C Status=ALERT
```

If FALSE:

- LED remains OFF.
- Buzzer remains OFF.
- System continues normal monitoring.

---

## Step 9: Push Button Detection

The LPC2148 continuously monitors the push button connected to a GPIO pin.

- Push button status is checked inside the main loop.
- When the button is pressed, the controller sets an Edit Mode flag.

Example:

```c
if(switch_state == PRESSED)
{
    edit_flag = 1;
}
```

The system then enters Edit Mode.

---

## Step 10: Entering Edit Mode

When Edit Mode is activated, the LCD displays the editing menu.

Example LCD Menu:

```text
EDIT MODE
1.TIME 2.DATE
```

The keypad is used to select the field that needs modification.

The user can edit:

- Hour
- Minute
- Second
- Day
- Month
- Year
- Temperature Set Point

---

## Step 11: Editing Time and Date Using Keypad

The keypad allows the user to enter new values.

Working:

- Select the required field.
- Enter the new value.
- LPC2148 validates the entered value.
- The value is written into the corresponding RTC register.

Example:

```text
Enter Hour:
14
```

LCD displays:

```text
TIME UPDATED
SUCCESS
```

The RTC immediately starts running with the updated time.

---

## Step 12: Editing Temperature Set Point

The user selects the Temperature Set Point option.

Example:

```text
Current Set Point = 45°C
Enter New Value:
50
```

Working:

- User enters a new temperature limit.
- LPC2148 validates the value.
- New set point is stored in memory.
- Future comparisons use the updated temperature limit.

---

## Step 13: Exit Edit Mode

After completing all modifications, the user selects **Exit**.

Working:

- Edit flag is cleared.
- LCD returns to monitoring screen.
- RTC continues with updated values.
- Temperature monitoring resumes automatically.

---

## Step 14: Continuous Monitoring Loop

The complete system operates continuously inside an infinite loop.

The monitoring cycle performs the following operations repeatedly:

1. Read temperature from LM35.
2. Read current time and date from RTC.
3. Display temperature and timestamp on LCD every second.
4. Generate timestamped sensor log.
5. Send log to Flash Magic Serial Terminal every minute.
6. Compare temperature with the set point.
7. Activate LED/Buzzer if temperature exceeds the threshold.
8. Check push button for Edit Mode.
9. Repeat the monitoring process continuously.

---

# Project Flow

- Initialize LCD.
- Initialize UART.
- Initialize RTC.
- Initialize ADC.
- Initialize GPIO devices.
- Read LM35 Temperature.
- Read Current Time and Date from RTC.
- Display Temperature and Timestamp on LCD.
- Generate Timestamped Sensor Log.
- Send Data to UART Serial Terminal.
- Compare Temperature with Set Point.
- If Temperature Exceeds Set Point:
  - Turn ON LED.
  - Turn ON Buzzer.
  - Send ALERT through UART.
- Check Push Button.
- Enter Edit Mode if Button is Pressed.
- Edit Time, Date, or Temperature Set Point using Keypad.
- Exit Edit Mode.
- Repeat Continuous Monitoring.

---

## Project Structure

```text
TIME_STAMPED_SENSOR_DATA_LOGGER/
├── main.c
├── lcd.c
├── lcd.h
├── uart.c
├── uart.h
├── rtc.c
├── rtc.h
├── lm35_adc.c
├── adc.h
├── keypad.c
├── keypad.h
├── delay.c
├── delay.h
├── Mini_Defines.h
└── README.md
```

--

## Hardware Output

![Project Output](IMG20260925173631.heic).
## Hyper Terminal Output

![Project Block Diagram](Screenshot%202026-09-18%20085627.png)


## Project Output

The system provides the following outputs:

- Real-time temperature monitoring.
- Real-time date and time display.
- Timestamped sensor data logging through UART.
- Temperature alert using LED/Buzzer.
- User-editable RTC date and time.
- User-editable temperature set point.
- Continuous serial logging for monitoring and record keeping.

## Applications

- Temperature Data Logging.
- Industrial Temperature Monitoring.
- Laboratory Data Recording.
- Embedded Sensor Monitoring System.
- Time-Based Environmental Monitoring.
- Real-Time Temperature Logging.
- Educational Embedded Systems Projects.

---

## Future Improvements

- Add EEPROM to store historical sensor logs.
- Add SD Card for offline data storage.
- Add ESP-01 Wi-Fi module for cloud logging.
- Add ThingSpeak cloud integration.
- Generate Excel reports automatically from logged data.
- Add humidity sensor (DHT11/DHT22).
- Add GSM or mobile notification alerts.

---

## Author

**M. Harikrishna**

Embedded Systems Project using **LPC2148 ARM7 Microcontroller**.
