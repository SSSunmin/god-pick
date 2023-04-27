#include <dos.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<graphics.h>

#define WIDTH 640
#define HEIGHT 480
#define UIBOXWIDTH 600
#define UIBOXHEIGHT 450
#define STARCOUNT 100
struct  Star
{
	float ab_slope;
	float slope;
	int start_x;
	int start_y;
	int prev_x;
	int prev_y;
	int now_x;
	int now_y;
	int direction;
	int ismove;
	int nowrad;
	int delaycount;
	int isend;
};


int key = 1;
int isloop = 1;
int stage = 0;
int isdrawcircle = 0;

int uiStartX = (WIDTH - UIBOXWIDTH) ;
int uiStartY = (HEIGHT - UIBOXHEIGHT)+5;

int g_pointX = 0;
int g_pointY = 0;
int g_pointprevX = 0;
int g_pointprevY = 0;
int g_circle_xpos = 0;
int g_circle_ypos = 0;

int clickCount = 0;
int g_nowrad = 0;
int maxrad = 150;

int countEndStar = 0;
int delaycount = 0;

int center_x = WIDTH / 2;
int center_y = HEIGHT / 2;
int stagestarcount[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

union REGS reg;

struct Star star[STARCOUNT];

void GetDirection(int index);
void ReverseDirection(int index);
void Moveline(int index, int color);
void drawrectangle(int xpos, int ypos, int color);
void mousepointer();
void BiggerCircle(int xpos, int ypos, int color);
void StarBiggerCircle(int xpos, int ypos, int color, int pointerindex);
void CheckHitPoint(int xpos, int ypos,int nowrad);

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

void ReverseDirection(int index)
{
	star[index].start_x = star[index].now_x;
	star[index].start_y = star[index].now_y;
	star[index].prev_x = star[index].start_x;
	star[index].prev_y = star[index].start_y;
	star[index].slope = star[index].slope * -1;
	star[index].ab_slope = fabs(star[index].slope);
}

void GetDirection(int index)
{
	if (star[index].slope >= 0 && star[index].ab_slope < 1)
	{
		if (star[index].start_x < WIDTH - uiStartX && star[index].start_y < HEIGHT - uiStartY)
		{
			star[index].direction = 1;
		}
		else
		{
			star[index].direction = 7;
		}
	}
	else if (star[index].slope >= 0 && star[index].ab_slope >= 1)
	{

		if (star[index].start_y < HEIGHT - uiStartY && star[index].start_x < WIDTH-uiStartX)
		{
			star[index].direction = 2;
		}
		else
		{
			star[index].direction = 8;
		}
	}
	else if (star[index].slope < 0 && star[index].ab_slope < 1)
	{
		if (star[index].start_x < WIDTH - uiStartX && star[index].start_y > uiStartY)
		{
			star[index].direction = 3;
		}
		else
		{
			star[index].direction = 5;
		}
	}
	else if (star[index].slope < 0 && star[index].ab_slope >= 1)
	{
		if (star[index].start_y > uiStartY && star[index].start_x < WIDTH - uiStartX)
		{
			star[index].direction = 4;
		}
		else
		{
			star[index].direction = 6;
		}
	}
}

void Moveline(int index, int color)
{
	double tmp = 0;
	if(star[index].ismove == 0)
	{
		putpixel(star[index].now_x, star[index].now_y, 0);
		return;
	}

	switch (star[index].direction)
	{
	case 1:
		if (star[index].now_x < WIDTH - uiStartX && star[index].now_y < HEIGHT-uiStartY)
		{
			tmp = floor((star[index].slope * (star[index].now_x - star[index].start_x) + (star[index].start_y)) + 0.5f);
			star[index].prev_x = star[index].now_x;
			star[index].prev_y = star[index].now_y;
			star[index].now_y = tmp;
			star[index].now_x++;
			putpixel(star[index].now_x, star[index].now_y, color);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
		}
		else
		{
			putpixel(star[index].now_x, star[index].now_y, 0);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
			ReverseDirection(index);
			GetDirection(index);
		}
		break;
	case 2:
		if (star[index].now_y < HEIGHT-uiStartY && star[index].now_x < WIDTH-uiStartX)
		{
			tmp = floor(((star[index].now_y - star[index].start_y) / star[index].slope + star[index].start_x) + 0.5f);
			star[index].prev_x = star[index].now_x;
			star[index].prev_y = star[index].now_y;
			star[index].now_x = tmp;
			star[index].now_y++;
			putpixel(star[index].now_x, star[index].now_y, color);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
		}
		else
		{
			putpixel(star[index].now_x, star[index].now_y, 0);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
			ReverseDirection(index);
			GetDirection(index);
		}
		break;
	case 3:

		if (star[index].now_x < WIDTH-uiStartX && star[index].now_y > uiStartY)
		{
			tmp = floor((star[index].slope  * (star[index].now_x - star[index].start_x) - (star[index].start_y)*-1) + 0.5f);
			star[index].prev_x = star[index].now_x;
			star[index].prev_y = star[index].now_y;
			star[index].now_y = tmp;
			star[index].now_x++;
			putpixel(star[index].now_x, star[index].now_y, color);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
		}
		else
		{
			putpixel(star[index].now_x, star[index].now_y, 0);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
			ReverseDirection(index);
			GetDirection(index);
		}
		break;
	case 4:
		if (star[index].now_y > uiStartY && star[index].now_x < WIDTH-uiStartX)
		{
			tmp = floor((((star[index].now_y - star[index].start_y) / star[index].slope + star[index].start_x)) + 0.5f);
			star[index].prev_x = star[index].now_x;
			star[index].prev_y = star[index].now_y;
			star[index].now_x = tmp;
			star[index].now_y--;
			putpixel(star[index].now_x, star[index].now_y, color);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
		}
		else
		{
			putpixel(star[index].now_x, star[index].now_y, 0);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
			ReverseDirection(index);
			GetDirection(index);
		}
		break;
	case 5:
		if (star[index].now_x > uiStartX && star[index].now_y < HEIGHT-uiStartY)
		{
			tmp = floor(((star[index].slope  * (star[index].now_x - star[index].start_x) + (star[index].start_y))) + 0.5f);
			star[index].prev_x = star[index].now_x;
			star[index].prev_y = star[index].now_y;
			star[index].now_y = tmp;
			star[index].now_x--;
			putpixel(star[index].now_x, star[index].now_y, color);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
		}
		else
		{
			putpixel(star[index].now_x, star[index].now_y, 0);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
			ReverseDirection(index);
			GetDirection(index);
		}
		break;
	case 6:
		if (star[index].now_y < HEIGHT-uiStartY && star[index].now_x > uiStartX)
		{
			tmp = floor(((star[index].now_y - star[index].start_y) / star[index].slope + star[index].start_x) + 0.5f);
			star[index].prev_x = star[index].now_x;
			star[index].prev_y = star[index].now_y;
			star[index].now_x = tmp;
			star[index].now_y++;
			putpixel(star[index].now_x, star[index].now_y, color);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
		}
		else
		{
			putpixel(star[index].now_x, star[index].now_y, 0);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
			ReverseDirection(index);
			GetDirection(index);
		}
		break;
	case 7:
		if (star[index].now_x > uiStartX && star[index].now_y > uiStartY)
		{
			tmp = floor((star[index].slope  * (star[index].now_x - star[index].start_x) + (star[index].start_y)) + 0.5f);
			star[index].prev_x = star[index].now_x;
			star[index].prev_y = star[index].now_y;
			star[index].now_y = tmp;
			star[index].now_x--;
			putpixel(star[index].now_x, star[index].now_y, color);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
		}
		else
		{
			putpixel(star[index].now_x, star[index].now_y, 0);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
			ReverseDirection(index);
			GetDirection(index);
		}
		break;
	case 8:
		if (star[index].now_y > uiStartY && star[index].now_x > uiStartX)
		{
			tmp = floor(((star[index].now_y - star[index].start_y) / star[index].slope + star[index].start_x) + 0.5f);
			star[index].prev_x = star[index].now_x;
			star[index].prev_y = star[index].now_y;
			star[index].now_x = tmp;
			star[index].now_y--;
			putpixel(star[index].now_x, star[index].now_y, color);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
		}
		else
		{
			putpixel(star[index].now_x, star[index].now_y, 0);
			putpixel(star[index].prev_x, star[index].prev_y, 0);
			ReverseDirection(index);
			GetDirection(index);
		}
		break;
	}
}

void PingPong()
{
	int index;
	index = 0;
	while (index < stagestarcount[stage] && isloop)
	{
		Moveline(index, YELLOW);

		if(kbhit())
		{
			key = getch();
			if (key == 'q' || key == 'Q')
			{
				isloop = 0;
				return;
			}
		}
		index++;

	}
}

void CheckHitPoint(int xpos, int ypos,int nowrad)
{
	double pointrad = 0;
	double rad = nowrad * nowrad;
	int index = 0;

	for(index = 0; index <stagestarcount[stage]; index++)
	{
		if(star[index].ismove == 1)
		{
			pointrad = ((star[index].now_x - (double)xpos) *(star[index].now_x - (double)xpos)) + ((star[index].now_y - (double)ypos)*(star[index].now_y - (double)ypos));
			if (pointrad > 0 && pointrad < rad)
			{
				star[index].ismove = 0;
			}
		}
	}
}


void drawcircle(int xpos, int ypos, int rad, int color)
{
	setcolor(color);
	circle(xpos, ypos, rad);
}

void BiggerCircle(int xpos, int ypos,int color)
{
	int prevrad = g_nowrad -1;

	if(prevrad > 0)
	{
		drawcircle(xpos, ypos, prevrad, BLACK);
	}
	if (g_nowrad < maxrad)
	{
		drawcircle(xpos, ypos, g_nowrad, color);
		CheckHitPoint(xpos,ypos, g_nowrad++);
	}
	else
	{
		if (delaycount < 30)
		{
			drawcircle(xpos, ypos, g_nowrad,color);
			delaycount++;
		}
		else
		{
			drawcircle(xpos, ypos, g_nowrad, BLACK);
			isdrawcircle = 0;
			g_nowrad = 0;
		}
	}
}


void MakeCircle()
{
	int index = 0;

	for (index = 0; index < stagestarcount[stage]; index++)
	{

		if (star[index].ismove == 0 && star[index].isend == 0)
		{
			StarBiggerCircle(star[index].now_x, star[index].now_y, YELLOW, index);
		}
	}
}

void StarBiggerCircle(int xpos, int ypos, int color, int pointerindex)
{
	int prevrad = star[pointerindex].nowrad-1;

	if(prevrad > 0)
	{
		drawcircle(xpos, ypos, prevrad, BLACK);
	}
	if (star[pointerindex].nowrad <maxrad)
	{
		drawcircle(xpos, ypos, star[pointerindex].nowrad, color);
		CheckHitPoint(xpos,ypos,star[pointerindex].nowrad++);
	}
	else
	{
		if (star[pointerindex].delaycount < 30)
		{
			drawcircle(xpos, ypos, star[pointerindex].nowrad, color);
			star[pointerindex].delaycount++;

		}
		else
		{
			drawcircle(xpos, ypos, star[pointerindex].nowrad, BLACK);
			star[pointerindex].nowrad = maxrad;
			star[pointerindex].isend = 1;
			countEndStar++;
		}
	}
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
        PingPong();
        if(reg.x.bx & 1 && isdrawcircle == 0)
		{
			if (clickCount < 1)
			{
				isdrawcircle = 1;
				g_circle_xpos = g_pointX;
				g_circle_ypos = g_pointY;
				clickCount++;
			}
		}
		if(isdrawcircle)
		{
			BiggerCircle(g_circle_xpos, g_circle_ypos, YELLOW);
		}
        	MakeCircle();
	}

	closegraph();
}