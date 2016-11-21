// 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
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
void button(int x,int y);
int main(int argc, char *argv[])

{
    int Bouncecount=0; //integer used to accumulate the number of rebounces 
    int i=49;//will hold the x-axis coordinate for the center of the circule
    int j=389;//will hold the y-axis coordinate for the center of the circule
    int delay;
    int xdir=1;/*will hold the value to be added to the x-axis 
                      coordinate of the circle */
    int ydir=1;
    char pattern[8] = {0x00, 0x70, 0x20, 0x27, 0x24, 0x24, 0x07, 0x00};

    int tag,oldi,oldj,k,l;
    char * m;
   /* int initwindow (int width, int height, const char* title="Windows BGI",
    int left=0, int top=0, bool dbflag=false, bool closeflag=true);*/
    initwindow(winWidth,winHeight,
    "Bouncing Ball                                By: Raphael Petta Kamgang"  ,20,20);
    tag=1;
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
            if (i>=winWidth-2*Xmargin-ballRaduis)
            {
              //xdir = -1;
               srand(time(NULL));
               xdir=-(int(rand()%NumberOfRand )+1);
               tag=-1;
               Bouncecount++;
               bgiout << "Right    " << endl;
               outstreamxy(400, 10);
            }
            //detect left edge
            if (i<=Xmargin+ballRaduis)
            {
                //xdir = 1;
                srand(time(NULL));
                xdir=int(rand()%NumberOfRand)+1;
                tag=1;
               Bouncecount++;
               bgiout << "Left    " << endl;
               outstreamxy(400, 10);
            }
            i=i+xdir;
            
            //detect bottom edge
            if (j>=winHeight-2*Ymargin-ballRaduis)
            {
               srand(time(NULL));
                ydir=-(int(rand()%NumberOfRand)+1);
                Bouncecount++;
                bgiout << "Bottom" << endl;
               outstreamxy(400, 10);
            }
            //detect top edge
            if (j<=Ymargin+ballRaduis)
            {
               srand(time(NULL));
               ydir=int(rand()%NumberOfRand)+1;
               Bouncecount++;
               bgiout << "Top      " << endl;
               outstreamxy(400, 10);
            }
            j=j+ydir;
            //output the number of bounce
            bgiout << Bouncecount << endl;
            outstreamxy(10, 10);
            
          setcolor(RED);  
            setfillstyle(SOLID_FILL ,BLUE);
            rectangle (Xmargin, Ymargin, winWidth-2*Xmargin, winHeight-2*Ymargin);
            
            setcolor(BLACK);
            setfillstyle(1,BLACK); 
            fillellipse(oldi,oldj,ballRaduis,ballRaduis);
            setfillstyle(1,WHITE);
            fillellipse(i,j,ballRaduis,ballRaduis);
     
            for(delay=0;delay<(200000*(xdir*xdir + ydir*ydir));delay++);      
      }
        closegraph();
    return 0;
}

