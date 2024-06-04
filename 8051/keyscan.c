#include <reg52.h> // �]�t���Y�ɡA�@�뱡�p���ݭn��ʡA���Y�ɥ]�t�S��\��H�s�����w�q
#include "delay.h"
#include "keyscan.h"

#define KeyPort P1 // �w�q��L��

unsigned char KeyScan(void)  // ��L���˨�ơA�ϥΦ�C�v�ű��˪k
{
    unsigned char Val;
    KeyPort = 0xf0; // ���|��m���A�C�|��ԧC
    if (KeyPort != 0xf0) // ��ܦ�������U
    {
        DelayMs(10); // �h��
        if (KeyPort != 0xf0) { // ��ܦ�������U
            KeyPort = 0xfe; // �˴��Ĥ@��
            if (KeyPort != 0xfe) {
                Val = KeyPort & 0xf0;
                Val += 0x0e;
                while (KeyPort != 0xfe);
                DelayMs(10); // �h��
                while (KeyPort != 0xfe);
                return Val;
            }
            KeyPort = 0xfd; // �˴��ĤG��
            if (KeyPort != 0xfd) {
                Val = KeyPort & 0xf0;
                Val += 0x0d;
                while (KeyPort != 0xfd);
                DelayMs(10); // �h��
                while (KeyPort != 0xfd);
                return Val;
            }
            KeyPort = 0xfb; // �˴��ĤT��
            if (KeyPort != 0xfb) {
                Val = KeyPort & 0xf0;
                Val += 0x0b;
                while (KeyPort != 0xfb);
                DelayMs(10); // �h��
                while (KeyPort != 0xfb);
                return Val;
            }
            KeyPort = 0xf7; // �˴��ĥ|��
            if (KeyPort != 0xf7) {
                Val = KeyPort & 0xf0;
                Val += 0x07;
                while (KeyPort != 0xf7);
                DelayMs(10); // �h��
                while (KeyPort != 0xf7);
                return Val;
            }
        }
    }
    return 0xff;
}

unsigned char KeyPro(void) {
    switch (KeyScan()) {
    case 0x7e:return 0; break; // 0 ���U����������ܬ۹������X��
    case 0x7d:return 1; break; // 1
    case 0x7b:return 2; break; // 2
    case 0x77:return 3; break; // 3
    case 0xbe:return 4; break; // 4
    case 0xbd:return 5; break; // 5
    case 0xbb:return 6; break; // 6
    case 0xb7:return 7; break; // 7
    case 0xde:return 8; break; // 8
    case 0xdd:return 9; break; // 9
    case 0xdb:return 10; break; // a
    case 0xd7:return 11; break; // 
    case 0xee:return 12; break; // 
    case 0xed:return 13; break; // 
    case 0xeb:return 14; break; // 
    case 0xe7:return 15; break; // �ǳ�
    default:return 0xff; break;
    }
}

