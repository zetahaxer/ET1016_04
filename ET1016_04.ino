//Guan Wen
//2516774
//Jun Quan
//2517056
//DCEP/FT/1A/04

#include <Wire.h>

#include "RichShieldTM1637.h"
#include "RichShieldNTC.h"
#include "RichShieldPassiveBuzzer.h"

#define PassiveBuzzerPin 3 //the signal pin of the module is connected with pin 3 of OPEN-SMART UNO R3
#define LED_RED 4 //The red Led is connected to Digital Pin 4 of Arduino Uno
#define LED_GREEN 5 //The green Led is connected to Digital Pin 5 of Arduino Uno
#define LED_BLUE 6 //The blue Led is connected to Digital Pin 6 of Arduino Uno
#define BUTTONK1 8 //BUTTON K1 is connected to digital Pin 8
#define BUTTONK2 9 //BUTTON K2 is connected to digital Pin 9
#define NTC_PIN A1 //SIG pin of NTC module connect to A1 of IO Shield, that is pin A1 of OPEN-SMART UNO R3
NTC temper(NTC_PIN); 

PassiveBuzzer buz(PassiveBuzzerPin);//create PassiveBuzzer type of object and initialize its pin no.

#define CLK 10//CLK of the TM1637 IC connect to D10 of OPEN-SMART UNO R3
#define DIO 11//DIO of the TM1637 IC connect to D11 of OPEN-SMART UNO R3
TM1637 disp(CLK,DIO);

void setup()
{
  disp.init();//The initialization of the display
  delay(1000);//
	pinMode(LED_RED,OUTPUT);
	pinMode(LED_GREEN,OUTPUT);
	pinMode(LED_BLUE,OUTPUT);
	pinMode(BUTTONK1,INPUT_PULLUP); // sets pin8 as input with INPUT_PULLUP mode.
	pinMode(BUTTONK2,INPUT_PULLUP); // sets pin9 as input with INPUT_PULLUP mode.
}

char systemStart = 0;

void loop()
{
   if (!systemStart && (digitalRead(BUTTONK1) == 0 || digitalRead(BUTTONK2) == 0))
	 {
    playStartupMelody();
    systemStart = 1;
		for (int i = 0; i < 4; i++) 
		{
			disp.display(i, INDEX_BLANK);
		}

    while (digitalRead(BUTTONK1) == 0 || digitalRead(BUTTONK2) == 0); // wait for release
  }

  if (!systemStart) return 0 ; // don’t show anything until started

	if (digitalRead(BUTTONK1) == 0 && digitalRead(BUTTONK2) == 0)
	 {
  playEndMelody();
  systemStart = 0; // stop system
  for (int i = 0; i < 4; i++) 
	{
		disp.clearDisplay(); // clear display
		digitalWrite(LED_RED, LOW);
		digitalWrite(LED_GREEN, LOW);
		digitalWrite(LED_BLUE, LOW); //dont show led until started
  }
  while (digitalRead(BUTTONK1) == 0 || digitalRead(BUTTONK2) == 0); // wait release
	if (!systemStart) return 0;
}
  float celsius = temper.getTemperature(); //get temperature
	float rounded = round(celsius * 2.0) / 2.0; 
	int whole = (int)rounded; //
	int half = (int)((rounded - whole) * 10);  // will be 0 or 5  
	displayTemperature((int8_t)whole, half); //
  delay(100);//delay 100ms
}
/************************************************* *********************/
/* Function: Display temperature on 4-digit digital tube */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees celsius */
/* Return Value: void */

float M = 28.0;

void displayTemperature(int8_t temperature, int half)
{
	int tempwhole = int(temperature);                  // Get integer part
  int8_t temp[4] = {0,1,2,3};
	 temp[0] = temperature / 10;
   disp.DecPoint = 1;
	 temp[1] = temperature % 10;
	 temp[2] = half;
	 temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);

	if (digitalRead(BUTTONK1) == 0) // check if button K1 is pressed (logic 0 when pressed)
	{
		delay(0);
		buz.playTone(1500, 300);
		M -= 0.5;
		changedisp(M);
		while (digitalRead(BUTTONK1) == 0);/*Ensure the button is released (i.e. back to logic 1) before executing the next statement */
	}
	if (digitalRead(BUTTONK2) == 0) // check if button K2 is pressed (logic 0 when pressed)
	{
		delay(0);
		buz.playTone(800, 300);
		M += 0.5;
		changedisp(M);
		while (digitalRead(BUTTONK2) == 0);/*Ensure the button is released (i.e. back to logic 1) before executing the next statement */
	}


	if(temperature == M)
	{
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_BLUE, LOW);
	}
	else 
	{
		if(temperature < M)
		{
		digitalWrite(LED_RED, HIGH);
		digitalWrite(LED_GREEN, LOW);
		digitalWrite(LED_BLUE, LOW);
		}
		else
		{
		digitalWrite(LED_RED, LOW);
		digitalWrite(LED_GREEN, LOW);
		digitalWrite(LED_BLUE, HIGH);
		}
	}
}


double changedisp(double M)
	{
		disp.clearDisplay();
		float Mrounded = round(M * 2.0) / 2.0;
		int Mwhole = int(M);
		int Mhalf = (int)((Mrounded - Mwhole) * 10);
		int8_t tempset [4];
		tempset[0] = M / 10;
		disp.DecPoint = 1;
		tempset[1] = ((int)M) % 10;
		tempset[2] = (Mhalf);
		tempset[3] = INDEX_BLANK;
		disp.display(tempset);
		delay(300);
		return 0;
	}

	void playStartupMelody() 
{
  buz.playTone(523, 150);  // C5
  delay(80);
  buz.playTone(659, 150);  // E5
  delay(80);
  buz.playTone(784, 150);  // G5
  delay(80);
  buz.playTone(988, 200);  // B5
  delay(150);}

	void playEndMelody() {
  buz.playTone(988, 150);  // B5
  delay(80);
  buz.playTone(784, 150);  // G5
  delay(80);
  buz.playTone(659, 150);  // E5
  delay(80);
  buz.playTone(523, 300);  // C5
  delay(150);
}
