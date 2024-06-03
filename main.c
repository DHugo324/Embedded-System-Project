#include <reg52.h> //�]�t���Y�ɡA�@�뱡�p���ݭn��ʡA���Y�ɥ]�t�S��\��H�s�����w�q 
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "delay.h"
#include "keyscan.h"
typedef unsigned char byte;
typedef unsigned int  word;

#define DataPort P0 //�w�q��ư� 
#define	KeyPort	P1	//�w�q��L��
sbit k1 = P3 ^ 0;
sbit k2 = P3 ^ 1;
sbit SPK = P3 ^ 5;  //�w�q���ֿ�X��
sbit LATCH1 = P3 ^ 7;	//�w�q��s�ϯ�� �q��s
sbit LATCH2 = P3 ^ 6;	//               ����s

unsigned char code dofly_DuanMa[] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
                                     0x77,0x7c,0x39,0x5e,0x79,0x71 };      // �q�X0~F
unsigned char code dofly_WeiMa[] = { 0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f };//��X
byte TempData[10];
word mynum[4];
bit catchable=0;

// state
#define WAIT 0
#define PREPARE 1
#define GUESS 2
#define END 3

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
char* itos(word x){
	if(x==10)return 'A';
	else return x+'0';
}
void main(void) {
    word i, j;
	word state;
	byte key;
	word guess_cnt=0, guess_num=0;
    InitUART();
    Init_Timer0();
    ES = 1;                  //���}��f���_
    while (1) {
		key = KeyPro();
        if (rec_flag == 1) {// ��UART��J
            for (i = 0;i < 8;j++) TempData[i] = 0; //�M��
            buf[head] = '\0';
			if(buf[0]=='W'){// �q��
				//���True
				state=PREPARE;
			}
			else if(buf[0]=='L'){// �q��
				//���False
				state=PREPARE;
			}
			else if(buf[0]=='G'){// �C������
				//��ܵ��G
				state=END;
			}
			else if(buf[0]>='0' && buf[0]<='6'){//�o��ڪ��Ʀr
				//��ܧڪ��Ʀr
				state=WAIT;
			}
			else if(buf[0]=='T'){// ����ڲq
				//���guess
				state=GUESS;
			}
			else if(buf[0]=='O'){// �������q
				catchable=1;
				state=GUESS;
			}
            rec_flag = 0;
            head = 0;
        }
		if(key!=0xff){// ����L��J
			if(state==GUESS){// �q��
				if(key>=1 && key<=10){
					while(key!=15){
						if(key>=1 && key<=10) guess_cnt = key;
						key = KeyPro();
						while(key==0xff)key = KeyPro();
					}
					key = KeyPro();
					while(~(key>=1 && key<=6))key = KeyPro();
					while(key!=15){
						if(key>=1 && key<=6) guess_num = key;
						key = KeyPro();
						while(key==0xff)key = KeyPro();
					}
					UART_SendStr(strcat(itos(guess_cnt),itos(guess_num)));//�ǰe�q��
					state=WAIT;
				}
				else if(key==15 && catchable){// ��
					UART_SendStr("STOP");
					catchable = 0;
					state=WAIT;
				}
			}
			else if(state==PREPARE){
				if(key==15){
					UART_SendStr("READY");//�ǳƧ���
					state=WAIT;
				}
			}
		}
		if(state==END)break;
    }
}