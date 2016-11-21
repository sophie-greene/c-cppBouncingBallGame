/*Reorganise the program in 3 so that the functions become 
discreet tasks and any data passed between them is moved using 
pipes or message constructs. Add a feature which records the highest score 
in a file together with players "tag". 
Add a "gravity" feature which slows down the ball 
and reduces its reach on the playing area as time passes. 
Fully document this program, including the pipe library code.
*/
// Program By: Raphael Petta Kamgang
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <time.h> //time header file
#include <math.h> //math library contains rand() and srand()
#include <windows.h> // WinApi header 
// define the values so that they can be changed easly if needed
//since these values are used many times throughout the program
//macros
#define winWidth 800
#define winHeight 600
#define ballRaduis 10
#define NumberOfRand 2
#define Xmargin 40
#define Ymargin 40
#define btnRaduis 20
#define HoleRaduis 3
#define holesMax 2
#define holesMin 0
#define btnMax 12
#define btnMin 8
#define PI 3.14159265
#define TIME_R 400
using namespace std;

//define point structure
struct point {
  int posX;
  int posY;
} ;
//Declare structure data type
struct playerdata {
       char name[20];
       char score[4];
       }player,player1; //two copies of structure
//define functions
void button(int ,int ,int );
void hole (int,int,int);
void generateRandButtons(int,int);
void generateRandHoles(int,int);
void drawBtns();
void drawHoles();
bool collisionDetection();
bool holeDetection();
bool edgeDetection();
void moveBall();

//define global variables
int Bouncecount; //integer used to accumulate the number of rebounces 
int i;//will hold the x-axis coordinate for the center of the circule
int j;//will hold the y-axis coordinate for the center of the circule
int xdir;/*will hold the value to be added to the x-axis                coordinate of the circle */
int ydir;
int tag;//used to store the index of the button which found to be in collision with the ball
int oldi;
int oldj;
int rndNBtn;//random number of buttons to be generated at start of game btnMin-btnMax  
      //calculate xSumBtn and ySumBtn
int rndNHoles;//random number of holes to be generated at start of game holeMin-holeMax  

point * btnPosArray;
point * holePosArray;
int dist;//dist of the centre of the ball from the centre of any button
int stTime;  

int main(int argc, char *argv[])
{
     FILE *fp;
    int xSumBtn; //stores the number of buttons which fit horizantally without overlapping
    int ySumBtn; //stores the number of buttons which fit vertically without overlapping
      /* int initwindow (int width, int height, const char* title="Windows BGI",
    int left=0, int top=0, bool dbflag=FALSE, bool closeflag=TRUE);*/
    
 int wid=initwindow(winWidth,winHeight,
    "Bouncing Ball                                By: Raphael Petta Kamgang"  ,20,20);
    
    //setbkcolor(WHITE);
    Bouncecount=0;
    
    floodfill(Xmargin, Ymargin, WHITE);
    setcolor(RED);  
    rectangle (Xmargin, Ymargin, winWidth-Xmargin, winHeight-Ymargin);
    
    xSumBtn=(winWidth-2*Xmargin)/(6*btnRaduis);
    ySumBtn=(winHeight-2*Ymargin)/(6*btnRaduis);
    
    generateRandButtons(xSumBtn,ySumBtn);
    
    xSumBtn=(winWidth-2*Xmargin)/(4*HoleRaduis);
    ySumBtn=(winHeight-2*Ymargin)/(4*HoleRaduis);
    
    generateRandHoles(xSumBtn,ySumBtn);
    
    xdir=1;
    ydir=1;
    tag=-1;
    //starting position
    i=Xmargin+ballRaduis;    
    j=Ymargin+ballRaduis;
    drawBtns();
    
    stTime=(clock()/CLOCKS_PER_SEC);
    setbkcolor(WHITE);
    setcolor(BLACK);
    bgiout << "                        "<< endl;
    outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
    bgiout << "Xdir = "<< xdir << endl;
    outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
    bgiout << "                        "<< endl;
    outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
    bgiout << "Ydir = "<< ydir << endl;
    outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
          // open file for input 
    if((fp = fopen("playerlog.txt", "rb"))==NULL)
    { 
           //fclose(fp); 
           goto enf11;
           }
         
    // Read from file back into memory - empty structure called newplayer
          
    fread(&player1, sizeof (struct playerdata), 1, fp);
  
  fclose(fp);

    bgiout << "the highest score is " <<player1.score <<  " by  "<< player1.name<< endl;
    outstreamxy(Xmargin, winHeight-Ymargin/2); 
enf11:     while(1)
    {    
//if ((clock()/CLOCKS_PER_SEC)-stTime==TIME_R) break;
         oldi=i;
         oldj=j;
         if (holeDetection()==TRUE)goto cl;
         if (edgeDetection()==TRUE) 
         {
         setbkcolor(WHITE);
                setcolor(BLACK);
                bgiout << "                        "<< endl;
                outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
                bgiout << "Xdir = "<< xdir << endl;
                outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
                bgiout << "                        "<< endl;
                outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
                bgiout << "Ydir = "<< ydir << endl;
                outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
                goto m;
                                    }
         if (collisionDetection()==TRUE)
         {
             setbkcolor(WHITE);
                setcolor(BLACK);
                bgiout << "                        "<< endl;
                outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
                bgiout << "Xdir = "<< xdir << endl;
                outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
                bgiout << "                        "<< endl;
                outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
                bgiout << "Ydir = "<< ydir << endl;
                outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);                           
             }
         
      
         if (xdir==0 && ydir==0 )
         {
         
            if ((clock()/CLOCKS_PER_SEC)-stTime>5) goto cl;         
            goto enf;
         }
         m:i=i+xdir;
         j=j+ydir;
         moveBall();
         enf:;
    }
cl:    closegraph();
    //initialize 
    strcpy(player1.score ,"0");
    sprintf(player.score ,"%d",Bouncecount); 
    //printf("%d",Bouncecount); 
    //getch();
    // open file for input 
    if((fp = fopen("playerlog.txt", "rb"))==NULL)
    { 
           //fclose(fp); 
           goto enf1;
           }
         
    // Read from file back into memory - empty structure called newplayer
          
    if(fread(&player1, sizeof (struct playerdata), 1, fp) != 1) 
    { 
  fclose(fp);
          goto enf1;
    }
           
          // Open file for output 

 

 enf1:          if (atoi(player1.score)<Bouncecount)
          {
             //Initiallise data into structure
    
  printf("You have scored the highest,Enter name\n>");
  
  scanf("%s",player.name);
  
 
                                  
//Write WHOLE of structure to a file
  if((fp = fopen("playerlog.txt", "wb"))!=NULL) {
  if(fwrite(&player, sizeof(struct playerdata), 1, fp) != 1) 
  {
    printf("Write error occurred.\n");
    getchar();
    exit(1);
  }
  
}
   }  
   fclose(fp);          
    return 0;
}

void button(int x1,int y1,int N)
{     
    setcolor(RED);
    setfillstyle(1,BLUE); 
    fillellipse(x1,y1,btnRaduis,btnRaduis);
    setfillstyle(1,RED); 
    fillellipse(x1,y1,btnRaduis/2,btnRaduis/2);
    //setbkcolor(WHITE);
//    setcolor(BLACK);
//    bgiout <<N<<endl;
//    outstreamxy (x1,y1);
 }

void generateRandButtons(int xNoBtn,int yNoBtn)
{
     point ** posMatrix;//dynamic array to store all possible positions of buttons
     int x,y;
     int cnt;
     int randomNoX,randomNoY;
     
    /*allocate the space for dynamic two dimentional array to 
    hold all the possible positions of the buttons*/
    posMatrix = new point*[xNoBtn];
    for ( cnt = 0; cnt < xNoBtn; ++cnt) 
    posMatrix[cnt] = new point [yNoBtn];
    
    //generate the all possible positions and store in posMatrix
    for (x=0;x<xNoBtn-1;x++)
    {
        for (y=0;y<yNoBtn-1;y++)
        {
            posMatrix[x][y].posX=Xmargin+6*(x+1)*(btnRaduis);
            posMatrix[x][y].posY=Ymargin+6*(y+1)*(btnRaduis);
        }
    }
    srand(time(NULL)); //seed the randomization process using the system clock    
    rndNBtn=(rand()%(btnMax-btnMin))+btnMin+1;
    btnPosArray= new point [rndNBtn];
    for (cnt=0;cnt<rndNBtn;cnt++)
    {
       srand(time(NULL)); //seed the randomization process using the system clock
gen:   randomNoX=rand()% (xNoBtn-1) ; 
       randomNoY=rand()%(yNoBtn-1);
    //insure no position will be selected more than once
        if (posMatrix[randomNoX][randomNoY].posX==-1)goto gen;
        btnPosArray[cnt].posX=posMatrix[randomNoX][randomNoY].posX;
        posMatrix[randomNoX][randomNoY].posX=-1;
        btnPosArray[cnt].posY=posMatrix[randomNoX][randomNoY].posY;
        button(btnPosArray[cnt].posX,btnPosArray[cnt].posY,cnt);
    //give chance for the randomization seed to change by applying delay
       //for(del=0;del<99999999;del++);
    }          
    delete [] posMatrix ; //free memory
    
 }
 void drawBtns()
 { 
      int cnt; 
      for (cnt=0;cnt<rndNBtn;cnt++)
         {
                button(btnPosArray[cnt].posX,btnPosArray[cnt].posY,cnt);  
         }

 }
 
bool edgeDetection()
 {
      

        /*detect the edges of the window
        subtract the ball raduis from the width or length
        this is to acheieve bouncing before any part of the ball get 
        out of the window*/
        //detect Right Edge
        //xdir is positive
        if (i>=(winWidth-Xmargin)-ballRaduis-abs(xdir))
        {
           srand(time(NULL));
           xdir=-((rand()%NumberOfRand )+1);
           if (ydir!=0)
            {
            ydir=((abs(ydir)/ydir)*(rand()%NumberOfRand ))+1;
            }
            else ydir=(rand()%NumberOfRand )+1;
           return TRUE;
        }
        //detect left edge
        //xdir is negative
        else if (i<Xmargin+ballRaduis+abs(xdir))
        {
            //xdir = 1;
            srand(time(NULL));
            xdir=(rand()%NumberOfRand)+1;
            if (ydir!=0)
            {
            ydir=((abs(ydir)/ydir)*(rand()%NumberOfRand ))+1;
            }
            else ydir=(rand()%NumberOfRand )+1;
            return TRUE;
        }
      

        //detect bottom edge
        //ydir is positive
        else if (j>(winHeight-Ymargin)-ballRaduis-abs(ydir))
        {
           srand(time(NULL));
           if (xdir!=0) 
           {
           xdir=((abs(xdir)/xdir)*(rand()%NumberOfRand ))+1;
           }
           else  xdir=(rand()%NumberOfRand )+1;
           ydir=-((rand()%NumberOfRand)+1);
           return TRUE;
        }
        //detect top edge
        //ydir is negative
        else if (j<=Ymargin+ballRaduis+abs(ydir))
        {
           srand(time(NULL));//seed the randomization process using the system clock
           if (xdir!=0) 
           {
           xdir=(abs(xdir)/xdir)*(rand()%NumberOfRand )+1;
           }
           else xdir=(rand()%NumberOfRand )+1;
           ydir=(rand()%NumberOfRand)+1;
           return TRUE;
        }
        else return FALSE;
      
 }
 
bool collisionDetection()
 {
    int cnt;
    double collNormalAngle;
    
    /*detect possible collision with buttons by calculating the 
    the distance of the ball centre from each of the buttons centres
    using Pythagoras' theorem */
    
  for(cnt=0;cnt<rndNBtn;cnt++)
        {      
               srand(time(NULL)); //seed the randomization process using the system clock          
           //Use Pythagorus to find distance to each object's centre
           dist=(int)sqrt((float)pow((btnPosArray[cnt].posX-i),2)+
           pow(btnPosArray[cnt].posY-j,2));
           
           if((dist<(btnRaduis+ballRaduis+sqrt(pow(xdir,2)+pow(ydir,2)))) && tag==-1)
           {
               tag=cnt;
               collNormalAngle=atan2(j-btnPosArray[cnt].posY, i-btnPosArray[cnt].posX);
               if ((btnPosArray[cnt].posX-i)>0 && (btnPosArray[cnt].posY-j)>0)
               {
                   xdir=abs(xdir)*cos (collNormalAngle) ;
                   ydir=abs(ydir)*sin (collNormalAngle);
               }
               else if ((btnPosArray[cnt].posX-i)>0 && (btnPosArray[cnt].posY-j)<0)
               {
                   xdir=abs(xdir)*cos (collNormalAngle) ;
                   ydir=abs(ydir)*sin (PI-collNormalAngle);
               }
               else if ((btnPosArray[cnt].posX-i)<0 && (btnPosArray[cnt].posY-j)>0)
               {
                   xdir=abs(xdir)*cos (PI-collNormalAngle) ;
                   ydir=abs(ydir)*sin (collNormalAngle);
               }
               else if ((btnPosArray[cnt].posX-i)<0 && (btnPosArray[cnt].posY-j)<0)
               {
                   xdir=abs(xdir)*cos (PI-collNormalAngle) ;
                   ydir=abs(ydir)*sin (PI-collNormalAngle);
               }
               if (xdir==0)xdir=((rand()%NumberOfRand)+1)*cos (collNormalAngle) /fabs(cos (collNormalAngle));
               if (ydir==0)ydir=((rand()%NumberOfRand)+1)*sin (collNormalAngle) /fabs(sin (collNormalAngle));
               Bouncecount++;
               setbkcolor(WHITE);
               setcolor(BLACK);
               bgiout << "Score = "<< Bouncecount << endl;
               outstreamxy(10,10);
               return TRUE;
               goto endof;
           }
           if ((dist >=(btnRaduis+ballRaduis+sqrt(pow(xdir,2)+pow(ydir,2))))&&(tag ==cnt)) tag = -1;
         
         }   

    return FALSE;
endof:;
 }
                     
 void moveBall()
 {
    int del;
    
             
         setbkcolor(WHITE);
         setcolor(BLACK);
         settextstyle(4 , HORIZ_DIR,12);
         bgiout << "Time Remaining "<< TIME_R-((clock()/CLOCKS_PER_SEC)-stTime)<<"  secs      " <<endl;
         outstreamxy(400,570);
    setcolor(WHITE);
    setfillstyle(1,WHITE); 
    fillellipse(oldi,oldj,ballRaduis,ballRaduis);
    setfillstyle(1,RED);
    // putpixel( oldi+2*ballRaduis, oldj, RED );
    //fillellipse(oldi,oldj,ballRaduis+1,ballRaduis+1);
    fillellipse(i,j,ballRaduis,ballRaduis);
    
    // for(del=0;del<200;del++);
    
    //delay is directly proportional to the x and y displacement
    for(del=0;del<5*(((clock()/CLOCKS_PER_SEC)-stTime)*(pow(xdir,2) + pow(ydir,2)-abs(ydir*xdir)));del++); 
    //getch();
 }    
bool holeDetection()
{
     int cnt;
    double collNormalAngle;
     //Create file pointer
 

    /*detect possible collision with holes by calculating the 
    the distance of the ball centre from each of the holes centres
    using Pythagoras' theorem */
    
  for(cnt=0;cnt<rndNHoles;cnt++)
        {      
               srand(time(NULL)); //seed the randomization process using the system clock          
           //Use Pythagorus to find distance to each object's centre
           dist=(int)sqrt((float)pow((holePosArray[cnt].posX-i),2)+
           pow(holePosArray[cnt].posY-j,2));
           
           if((dist<(HoleRaduis+ballRaduis)) )
           {
               xdir=0;
               ydir=0;
                setbkcolor(WHITE);
                setcolor(BLACK);
                bgiout << "                        "<< endl;
                outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
                bgiout << "Xdir = "<< xdir << endl;
                outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
                bgiout << "                        "<< endl;
                outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
                bgiout << "Ydir = "<< ydir << endl;
                outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
                setcolor(WHITE);
                setfillstyle(1,WHITE); 
                fillellipse(i,j,ballRaduis,ballRaduis);
                setfillstyle(1,RED);
                fillellipse(holePosArray[cnt].posX,holePosArray[cnt].posY,ballRaduis,ballRaduis);
               setbkcolor(WHITE);
               setcolor(RED);
               settextstyle(4 , HORIZ_DIR,72);
               bgiout << " GAME OVER" << endl;
               outstreamxy((winWidth-2*Xmargin)/3,(winHeight-2*Ymargin)/2);
               stTime=clock()/CLOCKS_PER_SEC;
  
   while(clock()/CLOCKS_PER_SEC-stTime<3)
               {
                setbkcolor(WHITE);
                setcolor(BLACK);
                settextstyle(4 , HORIZ_DIR,12);
                bgiout << "Time Remaining "<< 3-((clock()/CLOCKS_PER_SEC)-stTime)<<"  secs         " <<endl;
                outstreamxy(400,570);
               }
                
      
              

               return TRUE;
               goto endff;
           }
         
          
         }   
return FALSE;
  endff:;    
}
void generateRandHoles(int xNoHoles,int yNoHoles)
{
      point ** posMatrix;//dynamic array to store all possible positions of holes
     int x,y;
     int cnt;
     int i;
     int randomNoX,randomNoY;
     
    /*allocate the space for dynamic two dimentional array to 
    hold all the possible positions of the holes*/
    posMatrix = new point*[xNoHoles];
    for ( cnt = 0; cnt < xNoHoles; ++cnt) 
    posMatrix[cnt] = new point [yNoHoles];
    
    //generate the all possible positions and store in posMatrix
    for (x=0;x<xNoHoles-1;x++)
    {
        for (y=0;y<yNoHoles-1;y++)
        {
            posMatrix[x][y].posX=Xmargin+4*(x+1)*(HoleRaduis);
            posMatrix[x][y].posY=Ymargin+4*(y+1)*(HoleRaduis);
        }
    }
    srand(time(NULL)); //seed the randomization process using the system clock    
    rndNHoles=(rand()%(holesMax-holesMin))+holesMin+1;
    holePosArray= new point [rndNHoles];
    for (cnt=0;cnt<rndNHoles;cnt++)
    {
       srand(time(NULL)); //seed the randomization process using the system clock
gen:   randomNoX=rand()% (xNoHoles-1) ; 
       randomNoY=rand()%(yNoHoles-1);
    //insure no position will be selected more than once
        if (posMatrix[randomNoX][randomNoY].posX==-1)goto gen;
        //check if hole intersect with a button
        for (i=0;i<rndNBtn;i++)
        {
            if (sqrt(pow(btnPosArray[i].posX-posMatrix[randomNoX][randomNoY].posX,2)+
            pow(btnPosArray[i].posY-posMatrix[randomNoX][randomNoY].posY,2))
            <btnRaduis+HoleRaduis) 
            {
            posMatrix[randomNoX][randomNoY].posX=-1;
                goto gen;
                }
        }
        holePosArray[cnt].posX=posMatrix[randomNoX][randomNoY].posX;
        posMatrix[randomNoX][randomNoY].posX=-1;
        holePosArray[cnt].posY=posMatrix[randomNoX][randomNoY].posY;
        hole(holePosArray[cnt].posX,holePosArray[cnt].posY,cnt);
    //give chance for the randomization seed to change by applying delay
       //for(del=0;del<99999999;del++);
    }          
    delete [] posMatrix ; //free memory
    
}
void hole (int x2,int y2,int N1 )
{
     //setcolor(RED);
    setfillstyle(1,BLACK); 
    fillellipse(x2,y2,HoleRaduis,HoleRaduis);
        setbkcolor(WHITE);
  // setcolor(BLACK);
//   bgiout <<N1<<endl;
//   outstreamxy (x2,y2);
   
}
void drawHoles()
{
      int cnt; 
      for (cnt=0;cnt<rndNHoles;cnt++)
         {
                hole(holePosArray[cnt].posX,holePosArray[cnt].posY,cnt);  
         }
}
