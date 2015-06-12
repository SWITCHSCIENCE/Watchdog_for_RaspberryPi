/*************************************
**	main.c							**
**									**
**	Created	: 2014/12/18 14:43:39	**
**	Author	: O.aoki				**
**************************************/

#define __ATtiny85__		1						// チップ環境を"ATtiny85"としてコンパイルする

/* ---- 定数 ---- */
#define	OutPortMask			(1 << FET) + (1 << LED)	// ポート出力レジスタのマスクパターン
#define WD_TimeOut			5*100					// ウォッチドッグのタイムアウト[*10mS]
#define WD_RecoveryTime		10*100					// Raspberry Piのリブート（電源off）時間[*10mS]
#define WD_StartPulseNumber	5						// ウォッチドッグ開始シグナルのエッジ検出数

/* ---- ウォッチドッグステータス ---- */
#define	WD_ReBoot			0
#define	WD_default			1
#define	WD_active			2

/* -=-=-= Inport =-=-=- */
/* ---- Watch dog pulse ---- */
#define	WDpulse				PINB0					// Global : Watch dog pulse port
/* ---- Jumper switch port for LED ---- */
#define	LEDsw				PINB3					// Global : jumper sw port

/* -=-=-= Outport =-=-=- */
/* ---- ビット操作 ---- */
#define BitRst(addr, bit)	addr &= ~(1<<bit)		// Global : addrのbit番目をLowに
#define BitSet(addr, bit)	addr |=  (1<<bit)		// Global : addrのbit番目をHighに
#define BitTgl(addr, bit)	addr ^=  (1<<bit)		// Global : addrのbit番目を反転

/* ---- FET操作 ---- */
#define	FET					PB4
#define	OnFET				BitRst(PORTB, FET)		// Global : FETを繋げる
#define	OffFET				BitSet(PORTB, FET)		// Global : FETを切る

/* ---- LED操作 ---- */
#define	LED					PB1
#define	SetLED				BitRst(PORTB, LED)		// Global : LEDを点ける
#define	RstLED				BitSet(PORTB, LED)		// Global : LEDを消す
#define	TglLED				BitTgl(PORTB, LED)		// Global : LEDを反転

/* -=-=-= Debug用 =-=-=- */
/* ---- Debug用LED ---- */
#define	LED1				1
#define	SetLED1				BitRst(PORTB, LED1)		// Global : LED1を点ける
#define	RstLED1				BitSet(PORTB, LED1)		// Global : LED1を消す
#define	TglLED1				BitTgl(PORTB, LED1)		// Global : LED1を反転
