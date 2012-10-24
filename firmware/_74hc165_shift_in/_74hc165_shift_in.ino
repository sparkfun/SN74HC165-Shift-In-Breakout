/*
 * SparkFun SN74HC165 Input Shift Register Example
 * Based on Arduino Playground example, http://www.arduino.cc/playground/Code/ShiftRegSN74HC165N
 * Modified by Jordan McConnell at SparkFun Electronics
 *
 *
 * Program to shift in the bit values from the SN74HC165N 8-bit
 * parallel-in/serial-out shift register chips/breakout boards.
 *
 * This sketch demonstrates reading in digital states from one
 * or more daisy-chained SN74HC165N shift register breakout
 * boards while using only 4 digital pins on the Arduino.
 *
 * You can daisy-chain these boards by soldering the left/right
 * sides of the boards together and by connecting the serial-out
 * (Q7 pin) of the last shift register to the specified dataPin
 * of the Arduino.
 *
 * As is, this code supports 1-4 Shift registers, however, you
 * of course can daisy chain as many as you like while still
 * using only 4 Arduino pins if you process them 4 at a time
 * into separate unsigned long variables.
*/


// MAKE SURE THESE ARE CORRECT FOR YOUR SETUP
// Number of shift register chips that are daisy-chained.
#define NUMBER_OF_SHIFT_CHIPS   1
// Change the "int" to "long" if NUMBER_OF_SHIFT_CHIPS is > 2.
#define BYTES_VAL_T unsigned int


// Width of data, 8 data bits per shift register.
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8
// Width of pulse to trigger the shift register to read and latch.
#define PULSE_WIDTH_USEC   5
// Delay between shift register reads.
#define POLL_DELAY_MSEC   1000


// HARDWARE PINS, CHANGE IF YOUR SETUP IS DIFFERENT
                          // Signal Name, SF Board label
int ploadPin        = 8;  // Parallel load, SH/!LD
int clockEnablePin  = 9;  // Clock Enable, /CE
int dataPin         = 11; // Q7, SER_OUT of the last shift register in the chain
int clockPin        = 12; // Clock, CLK


// Read the state of all shift registers.
// Return state of state of register pins in an unsigned integer (or long).
BYTES_VAL_T read_shift_regs()
{
    byte bitVal;
    BYTES_VAL_T bytesVal = 0;

    // Trigger a parallel Load to latch the state of the data lines.
    digitalWrite(clockEnablePin, HIGH);
    digitalWrite(ploadPin, LOW);
    delay(PULSE_WIDTH_USEC);
    digitalWrite(ploadPin, HIGH);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(clockEnablePin, LOW);

    // Loop to read each bit value from the serial out line
    // of the SN74HC165. Store bits in variable bytesVal.
    for(int i = 0; i < DATA_WIDTH; i++)
    {
        // get next data bit from shift registers
        bitVal = digitalRead(dataPin);

        // Set the corresponding bit in bytesVal.
        bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

        // Pulse the Clock (rising edge shifts the next bit).
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(PULSE_WIDTH_USEC);
        digitalWrite(clockPin, LOW);
    }

    return(bytesVal);
}

// Display state of all shift registers.
void display_shift_data(BYTES_VAL_T pinValues)
{
    Serial.print("\r\nPin States:\r\n");

    for(int i = 0; i < DATA_WIDTH; i++)
    {
        Serial.print("  Pin-");
        Serial.print(i);
        Serial.print(": ");

        if((pinValues >> i) & 1)
            Serial.println("HIGH");
        else
            Serial.println("LOW");
    }
}

// Initialize hardware pins for reading shift registers and printing serial.
void setup()
{
    Serial.begin(9600);

    // Initialize each digital pin
    pinMode(ploadPin, OUTPUT);
    pinMode(clockEnablePin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);

    digitalWrite(clockPin, LOW);
    digitalWrite(ploadPin, HIGH);
}

// Read and display shift register data, wait, repeat.
void loop()
{
    BYTES_VAL_T data = 0; // Stores input pin data from shift registers

    // Read the state of all shift registers.
    data = read_shift_regs();

    // Display state of all shift registers.
    display_shift_data(data);

    // Wait a sec
    delay(POLL_DELAY_MSEC);
}
