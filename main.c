#include <reg52.h> //�]�t���Y�ɡA�@�뱡�p���ݭn��ʡA���Y�ɥ]�t�S��\��H�s�����w�q 
#include "uart.h"
#include "delay.h"
#include "keyscan.h"
typedef unsigned char byte;
typedef unsigned int  word;

#define DataPort P0 //�w�q��ư� 
#define	KeyPort	P1	//�w�q��L��
sbit k1 = P2 ^ 0;
sbit k2 = P2 ^ 1;
sbit SPK = P2 ^ 2;  //�w�q���ֿ�X��
sbit LATCH1 = P2 ^ 7;	//�w�q��s�ϯ�� �q��s
sbit LATCH2 = P2 ^ 6;	//               ����s

unsigned char code dofly_DuanMa[] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
                                     0x77,0x7c,0x39,0x5e,0x79,0x71 };      // �q�X0~F
unsigned char code dofly_WeiMa[] = { 0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f };//��X
byte TempData[10];
/*------------------------------------------------
                    �p�ɾ���l�ưƵ{��
------------------------------------------------*/
void Init_Timer0(void) {
    TMOD |= 0x01;//�ϥμҦ�1�A16�줸�p�ɾ��A�ϥ�"|"�Ÿ��i�H�b�ϥΦh�ӭp�ɾ��ɤ����v�T
    EA = 1;      //�`���_���}
    ET0 = 1;     //�p�ɾ����_���}
    TR0 = 1;     //�p�ɾ��}�����}
}
/*------------------------------------------------
 ��ܨ�ơA�Ω�ʺA���˼ƽX��
------------------------------------------------*/
void Display(unsigned char FirstBit, unsigned char Num) {
    static unsigned char i = 0;

    DataPort = 0;   //�M�Ÿ�ơA���������v
    LATCH1 = 1;     //�q��s
    LATCH1 = 0;

    DataPort = dofly_WeiMa[i + FirstBit]; //����X 
    LATCH2 = 1;     //����s
    LATCH2 = 0;

    DataPort = TempData[i]; //����ܸ�ơA�q�X
    LATCH1 = 1;     //�q��s
    LATCH1 = 0;

    i++;
    if (i == Num)
        i = 0;
}
/*------------------------------------------------
                 �p�ɾ����_�Ƶ{��
------------------------------------------------*/
void Timer0_isr(void) interrupt 1
{
    TH0 = (65536 - 2000) / 256;		  //���s��� 2ms
    TL0 = (65536 - 2000) % 256;
    Display(0, 8);       // �եμƽX�ޱ���
    TF1 = 0;
}

void main(void) {
    word i, j;
    InitUART();
    Init_Timer0();
    ES = 1;                  //���}��f���_
    while (1) {
        if (rec_flag == 1) {
            for (j = 0;j < 8;j++) TempData[j] = 0; //�M��
            buf[head] = '\0';

            rec_flag = 0;
            head = 0;
        }
    }
}