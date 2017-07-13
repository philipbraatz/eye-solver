#include <Windows.h>


enum trigger
{
	LongPress,Tap,Hold,Release
};

// only takes some values
// can be expanded to more
//w,a,s,d,q,e,o,p,i,t,1,2,3,4,5,6,7,8,9,shift(z),enter(c),f3(v),f5(),ctrl(m),space( )
int AsciiToScan(char input)
{
	///esc,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,prt,paus,del,hone,pgup,pgdn,end,`,1,2,3,4,5,6,7,8,9,0,-,=,back,tab,q,w,e,r,t,y,u,i,o,p,[,],\,cap,a,s,d,f,g,h,j,k,l,;,',ent,sh,z,x,c,v,b,n,m,,,.,,/,crtl,alt,space,left,up,right,down,num,n7,n8,n9,n4,n5,n6,n1,n2,n3,n0

	switch (input)
	{
	case 'w':
		return 0x11;
	case 'a':
		return 0x1E;
	case 's':
		return 0x1F;
	case 'd':
		return 0x20;
	case 'q':
		return 0x10;
	case 'e':
		return 0x12;
	case 'o':
		return 0x18;
	case 'p':
		return 0x19;
	case 'i':
		return 0x17;
	case 't':
		return 0x14;
	case '1':
		return 0x2;
	case '2':
		return 0x3;
	case '3':
		return 0x4;
	case '4':
		return 0x5;
	case '5':
		return 0x6;
	case '6':
		return 0x7;
	case '7':
		return 0x8;
	case '8':
		return 0x9;
	case '9':
		return 0xA;
	case 'z'://shift
		return 0x2A;
	case 'x'://esc
		return 0x1;
	case 'c'://enter
		return 0x1c;
	case 'v'://f3
		return 0x3D;
	case 'b'://f5
		return 0x3F;
	case 'n'://f4
		return 0x3E;
	case 'm'://ctrl
		return 0x1D;
	case ' '://space
		return 0x39;
	default:
		return 0x35;
	}
}

//move mouse
///SetCursorPos(x,y)

//			MOUSEEVENTF_
//call presses and depresses
void mouseClick(DWORD click) {
	INPUT ip;
	ip.type = INPUT_MOUSE;
	ip.mi.mouseData = 0;

	ip.mi.dwFlags = click;

	SendInput(1, &ip, sizeof(INPUT));//press button
}

//hold down the key in milliseconds
void keypress(char key, trigger t, int time =0)
{
	const int WAIT = 10;

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wScan = AsciiToScan(key); // Which keypress to simulate / hardware scan code for key
	ip.ki.wVk = 0; // virtual-key code for the key
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_SCANCODE;

	switch (t)
	{
	case LongPress:

		break;
	case Tap:
		SendInput(1, &ip, sizeof(INPUT));//down
		Sleep(WAIT);

		ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;//release
		SendInput(1, &ip, sizeof(INPUT));//up
		break;
	case Hold:
		SendInput(1, &ip, sizeof(INPUT));//down
		Sleep(time);

		ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;//release
		SendInput(1, &ip, sizeof(INPUT));//up

		break;
	case Release:
		ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;//release
		SendInput(1, &ip, sizeof(INPUT));//up
		break;
	default:
		ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;//release
		SendInput(1, &ip, sizeof(INPUT));//up
		break;
	}
}