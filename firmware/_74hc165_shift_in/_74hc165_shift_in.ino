/*

The purpose of this code is to get a user quickly up and running with the 
74HC165 Breakout Board.  This board and code allows a user to add more inputs
to their Arduino board using less pins.  While this particular example is meant
for one breakout board, the boards can be soldered in series (right side of one
to left side of the next) to achieve an even better input pin to Arduino pin ratio.

Example Output:
The incoming values of the shift register are: 
ABCDEFGH : 00001111


SparkFun Electronics - written by someone with no coding skill, Jordan McConnell
Date: December 14, 2012
License: Beerware. You like code. We like beer. Simple trading economics.

*/

// HARDWARE CONNECTIONS
// Connect the following pins between your Arduino and the 74HC165 Breakout Board
// Connect pins A-H to 5V or GND or switches or whatever
int data_pin = 11; // Connect Pin 11 to SER_OUT (serial data out)
int shld_pin = 8; // Connect Pin 8 to SH/!LD (shift or active low load)
int clk_pin = 12; // Connect Pin 12 to CLK (the clock that times the shifting)
int ce_pin = 9; // Connect Pin 9 to !CE (clock enable, active low)

byte incoming; // Variable to store the 8 values loaded from the shift register

// The part that runs once
void setup() 
{                
  // Initialize serial to gain the power to obtain relevant information, 9600 baud
  Serial.begin(9600);

  // Initialize each digital pin to either output or input
  // We are commanding the shift register with each pin with the exception of the serial
  // data we get back on the data_pin line.
  pinMode(shld_pin, OUTPUT);
  pinMode(ce_pin, OUTPUT);
  pinMode(clk_pin, OUTPUT);
  pinMode(data_pin, INPUT);
  
  // Required initial states of these two pins according to the datasheet timing diagram
  digitalWrite(clk_pin, HIGH);
  digitalWrite(shld_pin, HIGH);
  
}

// The part that runs to infinity and beyond
void loop() {
  
  incoming = read_shift_regs(); // Read the shift register, it likes that
  
  // Print out the values being read from the shift register
  Serial.println("\nThe incoming values of the shift register are: ");
  Serial.print("ABCDEFGH : ");
  print_byte(incoming); // Print every 1 and 0 that correlates with A through H
  //Serial.println(incoming,BIN); // This way works too but leaves out the leading zeros
  
  delay(2000); // Wait for some arbitrary amount of time
  
}

// This code is intended to trigger the shift register to grab values from it's A-H inputs
byte read_shift_regs()
{
  byte the_shifted = 0;  // An 8 bit number to carry each bit value of A-H

  // Trigger loading the state of the A-H data lines into the shift register
  digitalWrite(shld_pin, LOW);
  delayMicroseconds(5); // Requires delays so says the datasheet timing diagram
  digitalWrite(shld_pin, HIGH);
  delayMicroseconds(5);
  
  // Required initial states of these two pins according to the datasheet timing diagram
  pinMode(clk_pin, OUTPUT);
  pinMode(data_pin, INPUT);
  digitalWrite(clk_pin, HIGH);
  digitalWrite(ce_pin, LOW); // Enable the clock

  // Get the A-H values
  the_shifted = shiftIn(data_pin, clk_pin, MSBFIRST);
  digitalWrite(ce_pin, HIGH); // Disable the clock
  
  return the_shifted;

}

// A function that prints all the 1's and 0's of a byte, so 8 bits +or- 2
void print_byte(byte val)
{
    byte i;
    for(byte i=0; i<=7; i++)
    {
      Serial.print(val >> i & 1, BIN); // Magic bit shift, if you care look up the <<, >>, and & operators
    }
    Serial.print("\n"); // Go to the next line, do not receive $200
}

