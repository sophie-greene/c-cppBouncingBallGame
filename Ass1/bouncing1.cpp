/*A program is to be written in ANSI C that moves a "ball" across the screen. 
If this ball hits any of the edges of a box in which it is contained, 
it must bounce off in a randomised fashion. 
At each hit it causes a sound to occur.*/
// Program By: Raphael Petta Kamgang
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <math.h> //math library contains rand() and srand()
#include <windows.h> // WinApi header 
// define the values so that they can be changed easly if needed
//since these values are used many times throuout the program
#define winWidth 800
#define winHeight 600
#define ballRaduis 10
#define NumberOfRand 5
#define Xmargin 40
#define Ymargin 40
using namespace std;
int main(int argc, char *argv[])

{
    int Bouncecount=0; //integer used to accumulate the number of rebounces 
    int i=49;//will hold the x-axis coordinate for the center of the circule
    int j=389;//will hold the y-axis coordinate for the center of the circule
    int delay;
    int xdir=1;/*will hold the value to be added to the x-axis 
                      coordinate of the circle */
    int ydir=1;
    int oldi;
    int oldj;
    
   /* int initwindow (int width, int height, const char* title="Windows BGI",
    int left=0, int top=0, bool dbflag=false, bool closeflag=true);*/
    initwindow(winWidth,winHeight,
    "Bouncing Ball                                              By: Raphael Petta Kamgang"  ,20,20);
    floodfill(Xmargin, Ymargin, WHITE);
    setcolor(RED);
    while(1)
    {
            setcolor(YELLOW);
            oldi=i;
            oldj=j;
            /*detect the edges of the window
            subtract the ball raduis from the width or length
            this is to acheieve bouncing before any part of the ball get 
            out of the window*/
            //detect Right Edge
            if (i>=winWidth-Xmargin-ballRaduis)
            {
               srand(time(NULL));// seed the rand operation using the system clock
               xdir=-(int(rand()%NumberOfRand )+1);
               Bouncecount++;
               printf("\a");
            }
            //detect left edge
            if (i<=Xmargin+ballRaduis)
            {
               srand(time(NULL));
               xdir=int(rand()%NumberOfRand)+1;
               Bouncecount++;
               printf("\a");
            }
            i=i+xdir;
            
            //detect bottom edge
            if (j>=winHeight-Ymargin-ballRaduis)
            {
               srand(time(NULL));
                ydir=-(int(rand()%NumberOfRand)+1);
                Bouncecount++;
                printf("\a");
            }
            //detect top edge
            if (j<=Ymargin+ballRaduis)
            {
               srand(time(NULL));
               ydir=int(rand()%NumberOfRand)+1;
               Bouncecount++;
               printf("\a"); //create beep sound
            }
            j=j+ydir;
            //draw the box
            setcolor(BLUE);  
            rectangle (Xmargin, Ymargin, winWidth-Xmargin, winHeight-Ymargin);
            setcolor(BLACK); 
            setfillstyle(1,BLACK); 
            fillellipse(oldi,oldj,ballRaduis,ballRaduis);
            setfillstyle(1,BLACK);
            fillellipse(i,j,ballRaduis,ballRaduis);
            //the delay is directly proportional to the x and y axis displacement
            for(delay=0;delay<(100000*((abs(xdir )+ abs(ydir))*(abs(xdir )+ abs(ydir))));delay++);      
      }
        closegraph();
    return 0;
}

