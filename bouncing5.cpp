#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <math.h>
#include <windows.h> // WinApi header 

using namespace std;
void button(int x,int y);
int main(int argc, char *argv[])

{
    int i=49,j=389,delay,xdir=1,ydir=1;
    int oldi,k;
    initwindow(450,450);
    while(1)
    {
            setcolor(YELLOW);
            oldi=i;
            if (i>=440)
            {
                srand(time(NULL));
                xdir=-(int(rand()%50 )+1);
            }
            if (i<=10)
            {
                srand(time(NULL));
                xdir=int(rand()%7)+1;
            }
            i=i+xdir;
            if (j>=440)
            {
               srand(time(NULL));
                ydir=-(int(rand()%10)+1);
            }
            if (j<=10)
            {
               srand(time(NULL));
               ydir=int(rand()%2)+1;
            }
            j=j+ydir;//there should be a loop here somewhere to draw the ball for xdir number of times
            //for (int)
           // fillellipse(i-xdir,j,10,10);
//            setcolor(BLACK);
//            fillellipse(i-xdir,j,10,10);
  cleardevice();
           fillellipse(i,j,10,10);
            for(delay=0;delay<500000;delay++);  
           // setcolor(BLACK);
           // fillellipse(i,j,10,10);
    
      }
        closegraph();
    return 0;
}

