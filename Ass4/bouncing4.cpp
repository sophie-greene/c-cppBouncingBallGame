/*Reorganise the program in 3 so that the functions become 
discreet tasks and any data passed between them is moved using 
pipes or message constructs. Add a feature which records the highest score 
in a file together with players "tag". 
Add a "gravity" feature which slows down the ball 
and reduces its reach on the playing area as time passes. 
Fully document this program, including the pipe library code.
*/
// Program By: Somoud Saqfelhait
//****************************************************************
//header files
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <time.h> //time header file
#include <math.h> //math library contains rand() and srand()
#include <windows.h> // WinApi header 

//****************************************************************
// define the values so that they can be changed easly if needed
//since these values are used many times throughout the program
//macros
#define winWidth 800
#define winHeight 600
#define ballRaduis 10
#define NumberOfRand 4
#define Xmargin 40
#define Ymargin 40
#define btnRaduis 20
#define HoleRaduis 8
#define holesMax 2
#define holesMin 1
#define btnMax 7
#define btnMin 5
#define PI 3.14159265
#define TIME_R 400
using namespace std;
//*************************************************************
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
 //*************************************************************      
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
//***************************************************************
//define global variables
int score; //integer used to accumulate the number of rebounces 
int i;//x-axis coordinate of the center of the circle
int j;//y-axis coordinate of the center of the circle
/*holds the value to be added to the x-axis coordinate of the circle */
int xdir;
int ydir;
int tag;//the index of the button which found to be in collision with the ball
int oldi;
int oldj;
int cTime;
//random number of buttons to be generated at start of game btnMin-btnMax  
int rndNBtn;
//random number of holes to be generated at start of game holeMin-holeMax      
int rndNHoles;
//centres of the objects and it  contains rndNBtn number of points
point * btnPosArray;
//centres of the holes and it  contains rndNHoles number of points
point * holePosArray;
//initial time in secs
int stTime; 
 
//************************************************************************
int main(int argc, char *argv[])
{
    //Create file pointer
    FILE *fp;
    //stores the number of buttons which fit horizantally without overlapping
    int xSumBtn; 
    //stores the number of buttons which fit vertically without overlapping
    int ySumBtn; 
      
      /* int initwindow (int width, int height, const char* title="Windows BGI",
    int left=0, int top=0, bool dbflag=FALSE, bool closeflag=TRUE);*/
    int wid=initwindow(winWidth,winHeight,
    "Bouncing Ball                                By: Somoud Saqfelhait"  ,20,20);
    //intialize the score counter value
    score=0;
    //change the window's backgroung color
    floodfill(Xmargin, Ymargin, WHITE);
    //change the color 
    setcolor(RED);  
    //draw a rectangle
    rectangle (Xmargin, Ymargin, winWidth-Xmargin, winHeight-Ymargin);
    //the number of objects that would fit in the rectangle without overlap
    xSumBtn=(winWidth-2*Xmargin)/(6*btnRaduis);
    ySumBtn=(winHeight-2*Ymargin)/(6*btnRaduis);
    //produce an array of points which represent the centres of the objects
    generateRandButtons(xSumBtn,ySumBtn);
    
    //same as for object but done for sink holes
    xSumBtn=(winWidth-2*Xmargin)/(4*HoleRaduis);
    ySumBtn=(winHeight-2*Ymargin)/(4*HoleRaduis);
    
    generateRandHoles(xSumBtn,ySumBtn);
    //initialize 
    xdir=1;
    ydir=1;
    tag=-1;
    //starting position
    i=Xmargin+ballRaduis+4;    
    j=Ymargin+ballRaduis+4;
    //call the function to draw the objects "buttons"
    drawBtns();
    //initialize time
    stTime=(clock()/CLOCKS_PER_SEC);
    setbkcolor(WHITE);
    setcolor(BLACK);
    
    //output intial values
    bgiout << "                        "<< endl;
    outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
    bgiout << "Xdir = "<< xdir << endl;
    outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
    bgiout << "                        "<< endl;
    outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
    bgiout << "Ydir = "<< ydir << endl;
    outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
    bgiout << "Score = "<< score << endl;
    outstreamxy(10,10);
    //read the maximum scored from file playerlog.txt
    // open file for input 
    if((fp = fopen("playerlog.txt", "rb"))==NULL)goto en;
            
    // Read from file back into memory - empty structure called newplayer
          
    fread(&player1, sizeof (struct playerdata), 1, fp);
    fclose(fp);// close the file
    //display max
    bgiout << "The highest score is " <<player1.score <<  " by  "<< player1.name<< endl;
    outstreamxy(Xmargin, winHeight-2*Ymargin/3); 
en:; 
	//cTime is used to store the current time in secs  
	cTime=clock()/CLOCKS_PER_SEC;
    while(1)
    {   
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
			bgiout << "Score = "<< score << endl;
    		outstreamxy(10,10);                         
      }
      if (xdir==0 && ydir==0 )
      {
			//give 5 sec after game is over then exit the while loop
         if ((clock()/CLOCKS_PER_SEC)-stTime>3) goto cl;         
         goto enf;
      }
      if (((clock()/CLOCKS_PER_SEC)-stTime)>=TIME_R)goto cl;
m:		i=i+xdir;
      j=j+ydir;
      
      moveBall();
   enf:;
    }//while end
cl: closegraph();
   //initialize score
   strcpy(player1.score ,"0");
   sprintf(player.score ,"%d",score); 
   // open file for input 
    if((fp = fopen("playerlog.txt", "rb"))==NULL)goto enf1;    
    // Read from file back into memory - empty structure called newplayer         
    if(fread(&player1, sizeof (struct playerdata), 1, fp) != 1) 
    { 
		//if file failed to be opened
  		fclose(fp);
      goto enf1;
    } 
enf1:;// Open file for output 
	if (atoi(player1.score)<score)//atoi convert string to integer
   {
    	printf("You have scored the highest,Enter name\n>");
  		scanf("%s",player.name);
      //Write WHOLE of structure to a file
  		if((fp = fopen("playerlog.txt", "wb"))!=NULL) 
		{
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
//********************************end of main**********************************
void button(int x1,int y1,int N)
{  
	//draw the object   
   setcolor(CYAN);
   setfillstyle(1,CYAN); 
   fillellipse(x1,y1,btnRaduis,btnRaduis);
   setfillstyle(1,LIGHTRED); 
   fillellipse(x1,y1,3*btnRaduis/4,3*btnRaduis/4);
	setbkcolor(LIGHTRED);
   setcolor(CYAN);
   //output the object number
   settextstyle(1 , HORIZ_DIR,11);
   bgiout <<N<<endl;
   outstreamxy (x1-btnRaduis/4,y1-btnRaduis/2);
}
//**********************************end of button******************************

void generateRandButtons(int xNoBtn,int yNoBtn)
{
	//dynamic array to store all possible positions of buttons
   point ** posMatrix;
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
gen:  randomNoX=rand()% (xNoBtn-1) ; 
    	randomNoY=rand()%(yNoBtn-1);
	 	//insure no position will be selected more than once
	   if (posMatrix[randomNoX][randomNoY].posX==-1)goto gen;
	   btnPosArray[cnt].posX=posMatrix[randomNoX][randomNoY].posX;
	   posMatrix[randomNoX][randomNoY].posX=-1;//indicate the cell is taken
	   btnPosArray[cnt].posY=posMatrix[randomNoX][randomNoY].posY;
	   button(btnPosArray[cnt].posX,btnPosArray[cnt].posY,cnt);
 	
 	}          
 	delete [] posMatrix ; //free memory
 }
//**************end of generateRandButtons******************************

void drawBtns()
{ 
	//get all buttons positions and draw them
   int cnt; 
   for (cnt=0;cnt<rndNBtn;cnt++)
   {
      button(btnPosArray[cnt].posX,btnPosArray[cnt].posY,cnt);  
   }
}
//**************end of generateRandButtons******************************
 
  /*this function detects the edges of the window
	subtract the ball raduis from the width or length
	this is to acheieve bouncing before any part of the ball get 
	out of the window*/
bool edgeDetection()
{
	//detect Right Edge
	//xdir is positive
	if (i>(winWidth-Xmargin)-ballRaduis-abs(xdir))
	{
		srand(time(NULL));
		xdir=-((rand()%NumberOfRand )+1);
		if (ydir!=0)
		{
			//to get the sign of ydir divide the absolute value of ydir over ydir
			ydir=((abs(ydir)/ydir)*((rand()%NumberOfRand) +1));
		}
		else ydir=(rand()%NumberOfRand +1);
		return TRUE;
	}
	//detect left edge
	//xdir is negative
	else if (i<=Xmargin+ballRaduis+abs(xdir))
	{
		srand(time(NULL));
		xdir=(rand()%NumberOfRand)+1;
		if (ydir!=0)
		{
			ydir=((abs(ydir)/ydir)*(rand()%NumberOfRand +1));
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
			xdir=((abs(xdir)/xdir)*(rand()%NumberOfRand )+1);
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
			xdir=-1*(abs(xdir)/xdir)*((rand()%NumberOfRand)+1 );
		}
		else xdir=(rand()%NumberOfRand )+1;
		ydir=(rand()%NumberOfRand)+1;
		return TRUE;
	}
	else return FALSE;

 }
 //**************end of edgeDetection******************************
bool collisionDetection()
 {
	int cnt;
	double collNormalAngle;
	int dist;//dist of the centre of the ball from the centre of any button
	/*detect possible collision with buttons by calculating the 
	the distance of the ball centre from each of the buttons centres
	using Pythagoras' theorem */

  	for(cnt=0;cnt<rndNBtn;cnt++)
   {      
		   srand(time(NULL)); //seed the randomization process using the system clock          
		  
		  //Use Pythagorus to find distance to each object's centre
		  dist=(int)sqrt((float)pow((btnPosArray[cnt].posX-i),2)+
		  pow(btnPosArray[cnt].posY-j,2));
		  //calculate the angle between the button and the ball and 
		  //calculate the xdir and ydir after collision
		  if((dist<(btnRaduis+ballRaduis+ sqrt(pow(xdir,2)+pow(ydir,2))-2)) && tag==-1)
      	{
            tag=cnt;
            collNormalAngle=atan2(j-btnPosArray[cnt].posY, i-btnPosArray[cnt].posX);
            //ball approaches button from the right bottom
           // if ((btnPosArray[cnt].posX>i) && (btnPosArray[cnt].posY)>j)
//            {
//                xdir=abs(xdir)*cos (collNormalAngle) ;
//                ydir=abs(ydir)*sin (collNormalAngle);
//                printf("%f",sin (collNormalAngle));
//            }
//            else if ((btnPosArray[cnt].posX)>i && (btnPosArray[cnt].posY)<j)
//            {
//                xdir=abs(xdir)*cos (collNormalAngle) ;
//                ydir=abs(ydir)*sin (PI-collNormalAngle);
//            }
//            else if ((btnPosArray[cnt].posX)<i && (btnPosArray[cnt].posY)>j)
//            {
//                xdir=abs(xdir)*cos (2*PI/3-collNormalAngle) ;
//                ydir=abs(ydir)*sin (collNormalAngle);
//            }
//            else if ((btnPosArray[cnt].posX)<i && (btnPosArray[cnt].posY)<j)
//            {
//                xdir=abs(xdir)*cos (2*PI-collNormalAngle) ;
//                ydir=abs(ydir)*sin (2*PI-collNormalAngle);
//            }
//            else if ((btnPosArray[cnt].posX)==i )
//            {
//                xdir=-1*abs(xdir)*cos (collNormalAngle) ;
//                
//            }
//				else if ((btnPosArray[cnt].posY)==j)
//				{
//					ydir=-1*abs(ydir)*sin (collNormalAngle);
//				}
        //   
        xdir=(-1*xdir *cos (collNormalAngle) ) +(xdir *sin (collNormalAngle) );
			ydir=(-1*ydir *cos (collNormalAngle)) +(ydir *sin (collNormalAngle));

		  if (xdir==0)xdir=((rand()%NumberOfRand)+1)*cos (collNormalAngle) /fabs(cos (collNormalAngle));            
			  if (ydir==0)ydir=((rand()%NumberOfRand)+1)*sin (collNormalAngle) /fabs(sin (collNormalAngle));
            score++;
            setbkcolor(WHITE);
            setcolor(BLACK);
           // getch();
            return TRUE;
            goto endof;
           }
           
           if ((dist >(btnRaduis+ballRaduis))&&(tag ==cnt)) tag = -1;
         }   

    return FALSE;
endof:;
 }
                     
void moveBall()
{
	int del;
	setbkcolor(WHITE);
	setcolor(BLACK);
	settextstyle(1 , HORIZ_DIR,11);
	bgiout << "Time Remaining "<< TIME_R-((clock()/CLOCKS_PER_SEC)-stTime)<<"  secs      " <<endl;
	outstreamxy(400,570);
	setcolor(WHITE);
	setfillstyle(1,WHITE); 
	fillellipse(oldi,oldj,ballRaduis,ballRaduis);
	setfillstyle(1,RED);


	fillellipse(i,j,ballRaduis,ballRaduis);
	//show indication of speed
	/*speed or velocity is the change in distance 
	divided by the current time*/
	//update cTime
	if (cTime!=clock()/CLOCKS_PER_SEC)
	{
		cTime= clock()/CLOCKS_PER_SEC;
		setcolor(BLACK);	
		bgiout<<"speed=                                                               ";
		outstreamxy(500,10);
		bgiout<<"speed= "<<(100*sqrt(2)/cTime)<<"   pixel/sec";
		outstreamxy(500,10);
	}	
	for(del=0;del<1000*(((clock()/CLOCKS_PER_SEC)-stTime))*sqrt(pow(xdir,2)+pow(ydir,2)/TIME_R);del++); 
//delay is directly proportional to the x and y displacement
	//getch();
 }    
bool holeDetection()
{
	int cnt;
	double collNormalAngle;
	int dist;//dist of the centre of the ball from the centre of any button
	
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
			settextstyle(1 , HORIZ_DIR,11);
			bgiout << "Time Remaining "<< 3-((clock()/CLOCKS_PER_SEC)-stTime)
			<<"  secs         " <<endl;
			outstreamxy(400,570);
			}
			return TRUE;
			goto endff;
		}//if
	} //for  
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
	}          
	delete [] posMatrix ; //free memory
}
void hole (int x2,int y2,int N1 )
{
	setcolor(RED);
	setfillstyle(1,7); 
	fillellipse(x2,y2,HoleRaduis,HoleRaduis);
	setbkcolor(WHITE);
}
void drawHoles()
{
	int cnt; 
	for (cnt=0;cnt<rndNHoles;cnt++)
	{
		hole(holePosArray[cnt].posX,holePosArray[cnt].posY,cnt);  
	}
}
