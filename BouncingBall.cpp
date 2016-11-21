/*
Bouncing Ball Demo cpp file
Chris Tuminello (with thanks to an unknown java programmer)
www.PlanetCpp.com

Note* There were certain problems in the java version
that even with a limited math background and no physics
i attempted to fix. main problem is that at most settings
the ball doesn't seem to stop bouncing and will keep the same
kenetic loss forever once the ball gets low. If the ball has
the right settings or hits at the right time it will stop and
the velocity will drop to 0. Another problem is that if the 
kenetic loss is set too low for the gravity setting the ball 
doesnt seem to lose energy at all for the whole duration.
I think that is a problem of the gravity being high so it forces
the energy to also be high, and since the loss is low you won't see
change.
If anyone can fix the formula's problems i'd be interested in the results
*/
#include "bouncingball.h"	//include all the functions
LRESULT CALLBACK SettingsProc(HWND shWnd,UINT siMsg,WPARAM swParam,LPARAM slParam)
{
	INITCOMMONCONTROLSEX icex;	//for common controls
	HINSTANCE shinst;
	int kpos,gpos;	//for trackbar values

	switch(siMsg)
	{
	case WM_CREATE:
		shinst = (HINSTANCE)GetWindowLong(shWnd,GWL_HINSTANCE);
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_BAR_CLASSES;
		InitCommonControlsEx(&icex);	//load the trackbar class
		//create font for the settings windows
		bold = CreateFont(NTXFONTHEIGHT(10,GetDC(shWnd)),0,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,0,0,PROOF_QUALITY,DEFAULT_PITCH,"Tahoma");
		//for coloring the setting window and statics
		logbrush.lbColor = RGB(150,150,255);
		logbrush.lbHatch = 0;
		logbrush.lbStyle = BS_SOLID;
		shbrush = CreateBrushIndirect(&logbrush);	//creates the brush
		//set up the windows
			gravityslide = CreateWindow(TRACKBAR_CLASS,"",WS_CHILD|TBS_AUTOTICKS|TBS_VERT|TBS_RIGHT|WS_VISIBLE,
										30,30,30,150,shWnd,NULL,shinst,NULL);
			SendMessage(gravityslide,TBM_SETRANGE,true,MAKELONG(0,251));
			keneticslide = CreateWindow(TRACKBAR_CLASS,"",WS_CHILD|TBS_AUTOTICKS|TBS_VERT|TBS_LEFT|WS_VISIBLE,
										110,30,30,150,shWnd,NULL,shinst,NULL);
			SendMessage(keneticslide,TBM_SETRANGE,true,MAKELONG(1,100));
			SendMessage(keneticslide,TBM_SETTICFREQ,2,0);	//now theres 50 ticks each valued at 2+
		gstat = CreateWindow("static","Gravity",WS_CHILD|WS_VISIBLE|SS_CENTER,0,0,100,29,shWnd,NULL,shinst,NULL);
		kstat = CreateWindow("static","Kenetic Loss",WS_CHILD|WS_VISIBLE|SS_CENTER,100,0,50,29,shWnd,NULL,shinst,NULL);
			SendMessage(gstat,WM_SETFONT,(WPARAM)bold,MAKELPARAM(true,0));
				SendMessage(kstat,WM_SETFONT,(WPARAM)bold,MAKELPARAM(true,0));
		showkenetic = CreateWindow("static","",WS_CHILD|WS_VISIBLE|SS_LEFT,150,0,50,29,shWnd,NULL,shinst,NULL);
				SendMessage(showkenetic,WM_SETFONT,(WPARAM)bold,MAKELPARAM(true,0));

		SendMessage(keneticslide,TBM_SETPOS,true,30);//starting values
		SendMessage(gravityslide,TBM_SETPOS,true,88);//starting values
		break;
	case WM_PAINT:
		shdc = BeginPaint(shWnd,&sps);
			//update the static controls
			kpos = SendMessage(keneticslide,TBM_GETPOS,0,0);	//get trackbar values
			gpos = SendMessage(gravityslide,TBM_GETPOS,0,0);
			
			UpdateSettings(showkenetic,"",(double)kpos);	//update the statics
			UpdateSettings(gstat,"Gravity",(double)1.0+(double)gpos/10);
		EndPaint(shWnd,&sps);
		break;
	case WM_VSCROLL:
		/*to simplify this im just going to handle the updates in wm_vscroll
		this window is not complicated so no need to make sure its a trackbar control
		no real need to check notification code also i will just update through all
		you can get the value through notification but since i already have update code with
		tbm_getpos in wm_paint ill just redirect to there*/
			SendMessage(shWnd,WM_PAINT,0,0);
		break;
	case WM_DESTROY:	//cleanup time :o(
		DeleteObject(bold);
		DeleteObject(shbrush);
		break;
	case WM_CTLCOLORSTATIC:	//here we color the controls
		SetTextColor((HDC)swParam,RGB(0,0,255));
		SetBkMode((HDC)swParam,TRANSPARENT);	//so we dont have to worry about the text's background color
		return (long)shbrush;
		break;
	};
	return DefWindowProc(shWnd,siMsg,swParam,slParam);
}
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	int gravityval = 0;
	char gchar[10] = {""},kchar[10] = {""};
	char wintitle[50] = {""};

	switch (iMsg)
	{
	case WM_LBUTTONUP:
		if(thrown==true)	//if the ball was already thrown
		{
			BallX=0;		//reset coordinates
			BallY=0;
			thrown=false;	//tell timer that its not to be moved
			SendMessage(hWnd,WM_PAINT,0,0);	//update
		}
		else
		{
			//reset values
			BallDirectionX = RIGHT;BallDirectionY = FALLING;
			gravityval = SendMessage(gravityslide,TBM_GETPOS,0,0);
			//set gravity and kenetic loss to new value
			g = (double)1.0+(double)gravityval/10;
			s = 0;Vold = 0;	//set velocity to 0
			bounce = (int)SendMessage(keneticslide,TBM_GETPOS,0,0);

			_gcvt(g,4,gchar);	//_gcvt is to convert double to string
			_gcvt(bounce,4,kchar);
			strcpy(wintitle,"Bouncing Ball -> Gravity: ");//change window title to reflect values
			strcat(wintitle,gchar);//attach gravity value
			//check if 0 needed
			if(gchar[strlen(gchar)-1]=='.')
				strcat(wintitle,"0");
			strcat(wintitle," Kenetic Loss: ");
			strcat(wintitle,kchar);
			if(kchar[strlen(kchar)-1]=='.')
				strcat(wintitle,"0");
			SendMessage(hWnd,WM_SETTEXT,0,(LPARAM)(LPCTSTR)wintitle);	//set title
			thrown=true;	//tell timer to throw ball
		}
		break;
	case WM_CREATE:
		uitimer = SetTimer(hWnd,TIMER,50,NULL);	//timer that will control ball updating
		hbrush = CreateSolidBrush(RGB(0,0,255));	//ball color
		blackbrush = CreateSolidBrush(RGB(0,0,0));	//background brush
		hpen = CreatePen(1,1,RGB(0,0,255));			//ball pen
		hdc = GetDC(hWnd);
			SelectObject(hdc,hbrush);	//set gdi objects to hwnd
			SelectObject(hdc,hpen);
		ReleaseDC(hWnd,hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);	//tell windows were gonna paint
		hdc = GetDC(hWnd);
			SelectObject(hdc,blackbrush);	//set brush for background painting
			Rectangle(hdc,0,0,390,410);		//overwrite the background
			SelectObject(hdc,hbrush);		//set brush for ball painting
			if(flatten==true)	//did ball hit ground?
			{
				Ellipse(hdc,BallX,BallY+2,BallX+12,BallY+10);
				flatten=false;
			}
			else	//just draw round ball
				Ellipse(hdc,BallX,BallY,BallX+10,BallY+10);
			if(thrown==false)	//if were waiting for user to throw ball
				TextOut(hdc,73,180,ClickToStart,strlen(ClickToStart));	//textout message
		ReleaseDC(hWnd,hdc);
		EndPaint(hWnd,&ps);
		break;
	case WM_DESTROY:	//cleanup time :o(
			DeleteObject(hbrush);
			DeleteObject(blackbrush);
			DeleteObject(hpen);
			KillTimer(hWnd,uitimer);
			PostQuitMessage(0);
			return 0;
		break;
	case WM_TIMER:
			UpdatePosition(hWnd);	//does what it says
		break;
	}
	return DefWindowProc(hWnd,iMsg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR szCmdLine, int iCmdShow)
{
	RECT sysrect;
	int left = 0,top = 0;
	const char *Main = "Bouncing_Ball";
	const char *Setting = "BB_Settings";
	WNDCLASSEX mclass,setting;
	MSG msg;
	
	RegisterWindow(Main,mclass,(long)WndProc,hInstance,BLACK_BRUSH);
	RegisterWindow(Setting,setting,(long)SettingsProc,hInstance,NULL,RGB(150,150,255));
	SystemParametersInfo(SPI_GETWORKAREA,0,&sysrect,0);	//get screen metrics
		left = ((sysrect.right/2)-(390/2))+100;//center window then add 100 since both windows start next to each other, centers whole layout
		top = (sysrect.bottom/2)-(410/2);
	main = CreateWindow(Main, "Bouncing Ball", WS_VISIBLE|WS_BORDER|WS_MINIMIZEBOX|WS_SYSMENU, left, top,
						390, 410,NULL, NULL, hInstance, NULL);
	settings = CreateWindow(Setting,"Settings",WS_VISIBLE|WS_OVERLAPPED,left-200,top,200,200,NULL,NULL,hInstance,NULL);
while(GetMessage(&msg,NULL,0,0))
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}
	return msg.wParam;
}

