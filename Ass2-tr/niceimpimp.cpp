# define Gravity 0.98
void updateBallPhysics(int x,int y, int dx,int dy)
{
     x=x+dx
     y=y+dy
     
     //gradually slow down
     dx=(int )dx*Gravity
	 dy=(int )dy*Gravity
	 //stop completely when below a certain speed
		If Abs(dx)<0.068 Then c.dx=0
		If Abs(dy)<0.068 Then c.dy=0
	 
 }
