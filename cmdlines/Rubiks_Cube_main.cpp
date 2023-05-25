#include "surface.h"
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#define DISPLAY 1
using std::cout;

void display(RubiksCube& c);
int userOperating(RubiksCube &c);
char checkInput();
int askIfQuit();
bool askIfRufresh();
char gloBuf[2048];

int main(){
    RubiksCube r;
MAIN_BEGIN:
    r.init(rawCube);
    r.display();
    if (userOperating(r)==1)	goto MAIN_BEGIN;
    return 0;
}

void display(RubiksCube& c){
	#if DISPLAY
    cout << "--------------------------------------\n";

	c.display();
	// std::iostream outBuf;
	// std::stringstream out;
    // c.display(out);
	// std::cout << out.str();
	#endif
}
int userOperating(RubiksCube& c){
    char input;
	bool changed;
    while (true){
        input = checkInput();
		changed=true;
        switch(input){
			case -1:
			case 'q':
			case 'Q':
				askIfQuit();	changed = true;	break;
			case 'r':
			case 'R':
				if( askIfRufresh())	return 1;
				break;
            case 'u':
                c.U(); 	cout<<"U";	  	break;
            case 'U':
                c._U(); cout << "U\'";	break;
			case 'j':
				c.D(); 	cout << "D";	break;
			case 'J':
				c._D(); cout << "D\'";	break;
			case 'i':
				c.R(); 	cout << "R";	break;
			case 'I':
				c._R(); cout << "R\'";	break;
			case 'k':
				c.L(); 	cout << "L";	break;
			case 'K':
				c._L(); cout << "L\'";	break;
			case 'o':
				c.F(); 	cout << "F";	break;
			case 'O':
				c._F(); cout << "F\'";	break;
			case 'l':
				c.B(); 	cout << "B";	break;
			case 'L':
				c._B(); cout << "B\'";	break;
			
			case 'm':
				c.MU();	cout << "MU";	break;
			case 'M':
				c._MU();cout << "MU\'";	break;
			case ',':
				c.ML(); cout << "ML";	break;
			case '<':
				c._ML();cout << "ML\'";	break;
			case '.':
				c.MF(); cout << "MF";	break;
			case '>':
				c._MF();cout << "MF\'";	break;
			
			case 4:
				c.turnToUp(); 	cout << "CL";	break;
			case 5:
				c.turnToDown(); cout << "CL\'";	break;
			case 6:
				c.turnToLeft(); cout << "CU\'";	break;
			case 7:
				c.turnToRight();cout << "CU";	break;
			
			case 8:
				c.turnClockwise();	cout << "CF";	 break;
			case 9:
				c.turnCounterClockwise();	cout << "CF\'";	 break;

			default:
				changed = false;
				break;
        }
		if (changed)
			display(c);
    }
	return 0;
}
char checkInput(){
	char tmp;	//读取键值，或过滤功能键的第一个返回值
	char tmp2;	//接受功能键
	int a;
	// while (false){
	// 	a = _getch();
	// 	printf("%d\n",a);
	// }
	while (true)
	{
		tmp = _getch();
		
		if (tmp == 0 || tmp == -32)//表示读取的是功能键或者方向键，丢掉第一个返回值，读取第二个返回值
		{
			switch (tmp2 = _getch())	//接収功能键返回值
			{
			case 72://上 
				return 4;
                // printf("This is up\n");
				// break;
            case 80:
                return 5;
                // printf("This is down\n");
                // break;
			case 73:
                return 8;
				// printf("This is pageup\n");
				// break;
			case 81:
                return 9;
				// printf("This is pagedown\n");
				// break;
            case 75:
                return 6;
                // printf("this is left\n");
                // break;
            case 77:
                return 7;
                // printf("this is right\n");
                // break;
			case 59://F1
				// printf("This is F1\n");
				return 0;
                // break;
			default:
				return 0;
                // break;
			}
		}
		else			//普通按键，如字母、数字、space，Esc等按键
		{
			switch (tmp)
			{
			case 32://空格
                return ' ';
			case 27://Esc
				return -1;
			default:
                return (tmp>=0x20&&tmp<=0x7e)?tmp:0;
			}
		}
		
	}
	return 0;
}
int askIfQuit(){
	fflush(stdin);
    cout << "quit the program? y/n  ";
    char a=_getch();
    if (a=='y' || a=='Y')exit(0);
	cout << std::endl;
    return 0;
}
bool askIfRufresh(){
	cout << "refresh? y/n  ";
	char a=_getch();
	cout << std::endl;
	if (a=='y'||a=='Y'){
		std::cout << "\n\n\n\n\n\n\n\n\n\n";
		return true;
	}
	return false;
}