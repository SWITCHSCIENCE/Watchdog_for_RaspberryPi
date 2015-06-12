/*************************************
**	main.c							**
**									**
**	Created	: 2014/12/18 14:43:39	**
**	Author	: O.aoki				**
**************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "WDtimer.h"

//#define	F_CPU	8000000UL							// 8 MHz
//#include <util/delay.h>

/*---------------------------------------------------
	Global variable 
*/
	volatile unsigned char	IntFlag;
	volatile unsigned char	LowCounter;
	volatile unsigned char	HighCounter;
	volatile unsigned long	GlobalCounter;
	volatile unsigned long	WDCounter;

/*---------------------------------------------------
	Timer 
*/
void Timer_Init() {			// base CK = 8MHz
	OCR1C = 10;				// 0.0102 sec
	BitSet(TIFR, TOV1);		// clear overflow flag
	BitSet(TIMSK, TOIE1);	// Parmit overflow interrupt
	TCCR1 = 0b11001110;		// Clear timer, PWM Mode, No Output, timer-prescaler 1/8192
}

/*---------------------------------------------------
	In-port driver
*/
/* ---- Watch dog pulse ---- */
void WD_Port_Read() {
	if ((~PINB) & (1<<WDpulse)) {
		HighCounter = 0;
		if (LowCounter < 250) LowCounter++;
	} else {
		if (HighCounter < 250) HighCounter++;
		LowCounter = 0;
	}
}

unsigned char WD_Port_Rise_Edge() {
	if (HighCounter == 9) return (0xFF);
	return (0);
}

unsigned char WD_Port_Fall_Edge() {
	if (LowCounter == 9) return (0xFF);
	return (0);
}

unsigned char WD_Port_Double_Edge() {
	if (HighCounter == 9) return (0xFF);
	if (LowCounter  == 9) return (0xFF);
	return (0);
}

/* ---- Jumper switch port for LED ---- */
unsigned char JSW_Port() {
	if ((~PINB) & (1<<LEDsw)) return (0xFF);
	return (0);
}

/*---------------------------------------------------
	Main Routine
*/
int	main( void ) {
	unsigned char	WDstat;
	unsigned char	preCounter;
	unsigned char	LEDflag;

	/* ******* 初期化開始 ******* */
	cli();						// 初期処理中は割り込み禁止

	// ---- 変数初期化
	GlobalCounter = 0;
	WDstat = WD_default;
	WDCounter = 0;
	preCounter = 0;
	// ---- ポートＢ初期化
	BitSet(MCUCR, PUD);
	DDRB  = OutPortMask;		// Set to `PB4` `PB1` ouput
	PORTB = OutPortMask;		// Output default pattern
	RstLED;
	// ---- タイマー初期設定
	Timer_Init();

	sei();						// 割込み許可

	/* ******* 初期化終了 ******* */

	while (1) {	// main loop
		LEDflag = 0;
		switch (WDstat) {
			case WD_ReBoot :
					if ((GlobalCounter & 0x00000003) == 0) LEDflag = 0xFF;
					OffFET;
					if (WDCounter >= WD_RecoveryTime) {
						WDstat = WD_default;
						WDCounter = 0;
					}
					preCounter = 0;
					break;
			case WD_default :
					if ((GlobalCounter & 0x0000007F) == 0) LEDflag = 0xFF;
					OnFET;
					WDCounter = 0;
					if (WD_Port_Fall_Edge()) {
						preCounter++;
						if (preCounter > WD_StartPulseNumber) WDstat = WD_active;
					}
					break;
			case WD_active :
					if ((GlobalCounter & 0x0000000F) == 0) LEDflag = 0xFF;
					OnFET;
					if (WD_Port_Fall_Edge()) WDCounter = 0;
					if (WDCounter >= WD_TimeOut) {
						WDstat = WD_ReBoot;
						WDCounter = 0;
					}
					break;
			default:
					WDstat = WD_ReBoot;
					WDCounter = 0;
					break;
		}
		if (!JSW_Port())
			RstLED;
		else
			if (LEDflag) TglLED;
		IntFlag = 1;
		while (IntFlag == 1);
	}
}

/*------------------------------------------------------
	Timer Interrupt Sub Routines
 */
/* -=-=-=-= Timer Overflow ISR =-=-=-=- */
ISR(SIG_OVERFLOW1) {
	WD_Port_Read();
	GlobalCounter++;
	WDCounter++;
	IntFlag = 0;
}
