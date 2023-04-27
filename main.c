#include <dos.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<graphics.h>

#define UIBOXWIDTH 600
#define UIBOXHEIGHT 450

int g_pointX = 0;
int g_pointY = 0;
int g_pointprevX = 0;
int g_pointprevY = 0;


union REGS reg;

void drawrectangle(int xpos, int ypos, int color) 
{
	setcolor(color);
	rectangle(xpos, ypos, UIBOXWIDTH, UIBOXHEIGHT);
}

void mousepointer()
{
	g_pointprevX = g_pointX;
	g_pointprevY = g_pointY;
	g_pointX =reg.x.cx;
	g_pointY = reg.x.dx;
	putpixel(g_pointprevX, g_pointprevY, BLACK);
	putpixel(g_pointX, g_pointY,WHITE);
}

void main(void)
{
	int gd = DETECT, gm;
	int successState = -1;

	clrscr();

	initgraph(&gd, &gm, "c:\\TC30\\bgi");
	if (graphresult() != grOk)
	{
		printf("Graphics Error!\n");
		return;
	}
	reg.x.ax = 0;
	int86(0x33,&reg,&reg);

	if(reg.x.ax == 0)//mouse fail
	{
		printf("fail mouse");
		return;
	}
	reg.x.ax = 3; // 1 : show mouse pointer
	int86(0x33,&reg,&reg);

	while (isloop)
	{
		reg.x.ax = 3;
		int86(0x33,&reg,&reg);
		gotoxy(6,1);
		drawrectangle(uiStartX, uiStartY, RED);
		mousepointer();
	}

	closegraph();
}