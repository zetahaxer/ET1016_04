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
	Serial.begin(9600);
}

void loop()
{
  float celsius;
  celsius = temper.getTemperature();//get temperature
  displayTemperature((int8_t)celsius);//
  delay(300);//delay 300ms
}
/************************************************* *********************/
/* Function: Display temperature on 4-digit digital tube */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees celsius */
/* Return Value: void */

void displayTemperature(int8_t temperature)
{
	int tempwhole = int(temperature);                  // Get integer part
  int tempdecimal = int((temperature - tempwhole) * 10); // Get first decimal digit

	disp.display(00.00);
  int8_t temp[4];
	temp[0] = temperature / 10;
	temp[1] = temperature % 10;
	temp[2] = (tempdecimal);
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);

	float L = 24.0, M = 25.0, H = 26.0;

	if (digitalRead(BUTTONK1) == 0) // check if button K1 is pressed (logic 0 when pressed)
	{
		delay(300);
		Serial.println("Button K1 is pressed");
		buz.playTone(1500, 300);
		L += 0.5;
		M += 0.5;
		H += 0.5;
		while (digitalRead(BUTTONK1) == 0);/*Ensure the button is released (i.e. back to logic 1) before executing the next statement */
	}
	if (digitalRead(BUTTONK2) == 0) // check if button K2 is pressed (logic 0 when pressed)
	{
		delay(300);
		Serial.println("Button K2 is pressed");
		buz.playTone(800, 300);
		L += 0.5;
		M += 0.5;
		H += 0.5;
		while (digitalRead(BUTTONK1) == 0);/*Ensure the button is released (i.e. back to logic 1) before executing the next statement */
	}


	while (digitalRead(BUTTONK1) == 0 || (BUTTONK2) == 0)
	{
		int8_t tempset [4];
		tempset[0] = INDEX_BLANK;
		tempset[1] = INDEX_BLANK;
		tempset[2] = M / 10;
		tempset[3] = ((int)M) % 10;
		disp.display(tempset);
		delay(2000);
		while (digitalRead(BUTTONK1) == 0 || (BUTTONK2) == 0);/*Ensure the button is released (i.e. back to logic 1) before executing the next statement */
	}


		if(temperature < L)
	{
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, LOW);
	}
		else 
		{
			if(temperature = M)
			{
			digitalWrite(LED_RED, LOW);
			digitalWrite(LED_GREEN, HIGH);
			digitalWrite(LED_BLUE, LOW);
			}
			else
			{
				if(temperature > H)
				{
					digitalWrite(LED_RED, LOW);
					digitalWrite(LED_GREEN, LOW);
					digitalWrite(LED_BLUE, HIGH);
				}
			}
		}
	}
