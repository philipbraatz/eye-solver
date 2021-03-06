#pragma once

//get position
///GetCursorPos(&p)

enum press
{
	leftdown,leftup,rightdown,rightup
};

int AsciiToScan(char input);
char* encode(const wchar_t* wstr, auto codePage);
wchar_t* decode(const char* encodedStr, auto codePage);
void mousectrl(int flag, int x = 0, int y = 0);
void keypress(char key, int hold, bool stay = false);
void keyinput(int press, char button);