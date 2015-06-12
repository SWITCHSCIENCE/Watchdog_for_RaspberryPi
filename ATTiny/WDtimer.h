/*************************************
**	main.c							**
**									**
**	Created	: 2014/12/18 14:43:39	**
**	Author	: O.aoki				**
**************************************/

#define __ATtiny85__		1						// �`�b�v����"ATtiny85"�Ƃ��ăR���p�C������

/* ---- �萔 ---- */
#define	OutPortMask			(1 << FET) + (1 << LED)	// �|�[�g�o�̓��W�X�^�̃}�X�N�p�^�[��
#define WD_TimeOut			5*100					// �E�H�b�`�h�b�O�̃^�C���A�E�g[*10mS]
#define WD_RecoveryTime		10*100					// Raspberry Pi�̃��u�[�g�i�d��off�j����[*10mS]
#define WD_StartPulseNumber	5						// �E�H�b�`�h�b�O�J�n�V�O�i���̃G�b�W���o��

/* ---- �E�H�b�`�h�b�O�X�e�[�^�X ---- */
#define	WD_ReBoot			0
#define	WD_default			1
#define	WD_active			2

/* -=-=-= Inport =-=-=- */
/* ---- Watch dog pulse ---- */
#define	WDpulse				PINB0					// Global : Watch dog pulse port
/* ---- Jumper switch port for LED ---- */
#define	LEDsw				PINB3					// Global : jumper sw port

/* -=-=-= Outport =-=-=- */
/* ---- �r�b�g���� ---- */
#define BitRst(addr, bit)	addr &= ~(1<<bit)		// Global : addr��bit�Ԗڂ�Low��
#define BitSet(addr, bit)	addr |=  (1<<bit)		// Global : addr��bit�Ԗڂ�High��
#define BitTgl(addr, bit)	addr ^=  (1<<bit)		// Global : addr��bit�Ԗڂ𔽓]

/* ---- FET���� ---- */
#define	FET					PB4
#define	OnFET				BitRst(PORTB, FET)		// Global : FET���q����
#define	OffFET				BitSet(PORTB, FET)		// Global : FET��؂�

/* ---- LED���� ---- */
#define	LED					PB1
#define	SetLED				BitRst(PORTB, LED)		// Global : LED��_����
#define	RstLED				BitSet(PORTB, LED)		// Global : LED������
#define	TglLED				BitTgl(PORTB, LED)		// Global : LED�𔽓]

/* -=-=-= Debug�p =-=-=- */
/* ---- Debug�pLED ---- */
#define	LED1				1
#define	SetLED1				BitRst(PORTB, LED1)		// Global : LED1��_����
#define	RstLED1				BitSet(PORTB, LED1)		// Global : LED1������
#define	TglLED1				BitTgl(PORTB, LED1)		// Global : LED1�𔽓]
