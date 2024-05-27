#include <reg52.h> //�]�t���Y�ɡA�@�뱡�p���ݭn��ʡA���Y�ɥ]�t�S��\��H�s�����w�q
#include "uart.h"
typedef unsigned char byte;
typedef unsigned int  word;
unsigned int max = 20;
byte buf[20];
byte head = 0;
byte get_0d = 0;
byte rec_flag = 0;
/*------------------------------------------------
/*------------------------------------------------
                    ��f��l��
------------------------------------------------*/
void InitUART  (void)
{
    SCON  = 0x50;		        // SCON: �Ҧ� 1, 8-bit UART, �ϯ౵��  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ����
    TH1   = 0xFD;               // TH1:  ���˭� 9600 ��C�ǿ�t�v ���� 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 ���}                         
    EA    = 1;                  //���}�`���_
}
/*------------------------------------------------
                    �o�e�@�Ӧ줸��
------------------------------------------------*/
void UART_SendByte(unsigned char dat)
{
 	SBUF = dat;
 	while(!TI);
    	TI = 0;
}

/*------------------------------------------------
                    �o�e�@�Ӧr��
------------------------------------------------*/
void UART_SendStr(unsigned char *s)
{
	while(*s!='\0')// \0 ��ܦr�굲���лx�A�q�L�˴��O�_�r�꥽��
  	{
		UART_SendByte(*s);
		if(*s=='\n')	//�Y�����O����Ÿ��]���X
			break;
  		s++;
  	}
}


/*------------------------------------------------
                     ��f���_�{��
------------------------------------------------*/
void UART_SER (void) interrupt 4 //��C���_�A�ȵ{��
{
    // 0x0d:'\r' 0x0a:'\n'
    unsigned char tmp;              //�w�q�{���ܼ� 
    if (RI) {                       //�P�_�O�������_����
	  	RI=0;                      //�лx�줸�M�s
	  	tmp=SBUF;                 //Ū�J�w�İϪ���
        if (get_0d == 0) {
            if (tmp == 0x0d) get_0d = 1;
            else {
                buf[head] = tmp;
                head++;
                if (head == MAX) head = 0;
            }
        }
        else if (get_0d == 1) {
            if (tmp != 0x0a) {
                head = 0;
                get_0d = 0;
            }
            else {
                rec_flag = 1;
                get_0d = 0;
            }
        }
    }
}