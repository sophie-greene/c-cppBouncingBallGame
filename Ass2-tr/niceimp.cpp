//Rem
// Title: Billiards Style Collision Physics
// Description: 2D circle collision response which
// can be used where 2 objects collide in 2 dimensions
//
// 2D COLLISION CODE
//
// By Joseph 'Phish' Humfrey
// This Type of collision response isn't just useful For pool and billiard style
// games, And in fact, I didn't write it for that reason at all. I wrote it because I
// needed To have collision for the space ships in my game 'Unity'.
// When their shields are up, they use this relatively simple method.
// It can be used For almost anything - player character bouncing off enemies in
// a platform game, to space ship collision, to a game which does actually involve
// balls. The basic algorithm which I used would work for both 2D and 3D, so If you
// would like to see it, email me at phish@...
//End Rem

#define sw 800 
#define sh=600

#define FRICTION	0.982
# define BALL_MASS 160.0
#define BALL_RADIUS#15.0

SetGraphicsDriver GLMax2DDriver()
Graphics sw,sh,0,60
SetClsColor 16,172,78

SeedRnd MilliSecs()

Global dx#=-1.0 , dy#=0 , power#=0.0

Global ballList:TList=New TList
Type ballType
	Field x#, y#     ' position
	Field dx#, dy#   ' x and y speeds
	Field radius#    ' radius of ball
	Field mass#      ' mass of ball
	Field r%,g%,b%   ' ball color
End Type

SetupTriangle
Global cueball:ballType=New ballType
SetupCueBall


' #############################################################

' MAIN LOOP
Repeat
	Global mx%,my%
	
	SetClsColor 60,140,80
	Cls
	UpdateBallPhysics
	RenderBalls

	' shoot
	If MouseDown(1)
		power=power+0.3+(power*0.04)
		If power>50.0 power=50.0
	Else
		If cueball.dx=0 And cueball.dy=0 ' ball stopped
			FlushMouse
			ang=angle(cueball.x,cueball.y,mx,my)
			cueball.dx=Sin(ang)*power ;	cueball.dy=-Cos(ang)*power
			power=0
		EndIf
	EndIf

	' reset
	If MouseHit(2)
		ballList.Clear
		SetupTriangle
		SetupCueBall
	End If

	' show shoot line when balls are stationary
	' (currently, only checks for cue ball stopped)
	If cueball.dx=0 And cueball.dy=0
		SetColor 180,180,180
		SetAlpha 0.4 ; SetBlend ALPHABLEND
		mx%=MouseX() ; my%=MouseY()
		dx=(mx-cueball.x)/20 ; dy=(my-cueball.y)/20
		DrawLine cueball.x,cueball.y,mx,my
		DrawOval mx-cueball.radius, my-cueball.radius, cueball.radius*2, cueball.radius*2
		DrawOval mx-cueball.radius+1, my-cueball.radius+1, cueball.radius*2-2, cueball.radius*2-2
		SetAlpha 1.0
	EndIf

	' draw power bar
	SetColor 0,0,0 ; SetAlpha 0.7
	DrawRect sw*0.3-1,sh-26,50*4+2,18
	SetColor 170,251,206
	DrawRect sw*0.3,sh-25,power*4,16

	SetAlpha 1.0 ; SetBlend MASKBLEND
	SetColor 200,200,200
	DrawText "[LMB] Shoot   [RMB] Reset    [Esc] Exit",10,10
	
	Flip
Until KeyHit(KEY_ESCAPE)

End

' #############################################################


Function SetupTriangle()
	Const ballTriangleSize%=5
	For Local xloop% = ballTriangleSize To 1 Step -1
		For Local yloop% = 1 To xloop
			Local c:ballType = New ballType
			ballList.AddLast c
			c.radius = BALL_RADIUS
			c.mass = BALL_MASS
			c.x = (5-xloop)*27 + 200
			c.y = (yloop*31) - (xloop*31)/2.0 + (sh/2)
			c.r=Rand(255) ; c.g=Rand(255) ; c.b=Rand(255)
		Next
	Next
End Function

Function SetupCueBall()
	cueball.x = sw-BALL_RADIUS-8
	cueball.y = sh/2
	cueball.dx=0.0 ; cueball.dy=0.0
	cueball.radius = BALL_RADIUS
	cueball.mass = BALL_MASS
	cueball.r=255 ; cueball.g=255 ; cueball.b=255
	ballList.Addlast cueball
End Function

Function UpdateBallPhysics()
' This is the main physics function for the
' balls. It contains the very basic movement
' physics as well as the collision response code
	For Local c:ballType = EachIn ballList
		' update positions
		c.x=c.x+c.dx
		c.y=c.y+c.dy
		' gradually slow down
		c.dx=c.dx*FRICTION
		c.dy=c.dy*FRICTION
		' stop completely when below a certain speed
		If Abs(c.dx)<0.068 Then c.dx=0
		If Abs(c.dy)<0.068 Then c.dy=0
		' COLLISION CHECKING
		' Check each ball in the loop against
		' every other (c against c2)
		For Local c2:ballType = EachIn ballList
			Local collisionDistance# = c.radius+c2.radius
			Local actualDistance# = Sqr((c2.x-c.x)^2+(c2.y-c.y)^2)
			' collided or not?
			If actualDistance<collisionDistance Then
				Local collNormalAngle#=ATan2(c2.y-c.y, c2.x-c.x)
				' position exactly touching, no intersection
				Local moveDist1#=(collisionDistance-actualDistance)*(c2.mass/Float((c.mass+c2.mass)))
				Local moveDist2#=(collisionDistance-actualDistance)*(c.mass/Float((c.mass+c2.mass)))
				c.x=c.x + moveDist1*Cos(collNormalAngle+180)
				c.y=c.y + moveDist1*Sin(collNormalAngle+180)
				c2.x=c2.x + moveDist2*Cos(collNormalAngle)
				c2.y=c2.y + moveDist2*Sin(collNormalAngle)
				' COLLISION RESPONSE
				' n = vector connecting the centers of the balls.
				' we are finding the components of the normalised vector n
				Local nX#=Cos(collNormalAngle)
				Local nY#=Sin(collNormalAngle)
				' now find the length of the components of each movement vectors
				' along n, by using dot product.
				Local a1# = c.dx*nX  +  c.dy*nY
				Local a2# = c2.dx*nX +  c2.dy*nY
				' optimisedP = 2(a1 - a2)
				'             ----------
				'              m1 + m2
				Local optimisedP# = (2.0 * (a1-a2)) / (c.mass + c2.mass)
				' now find out the resultant vectors
				'' Local r1% = c1.v - optimisedP * mass2 * n
				c.dx = c.dx - (optimisedP*c2.mass*nX)
				c.dy = c.dy - (optimisedP*c2.mass*nY)
				'' Local r2% = c2.v - optimisedP * mass1 * n
				c2.dx = c2.dx + (optimisedP*c.mass*nX)
				c2.dy = c2.dy + (optimisedP*c.mass*nY)
			End If
		Next
		' Simple bouncing off walls.
		If c.x<c.radius
			c.x=c.radius
			c.dx=c.dx*-0.9
		End If
		If c.x>GraphicsWidth()-c.radius
			c.x=GraphicsWidth()-c.radius
			c.dx=c.dx*-0.9
		End If
		If c.y<c.radius
			c.y=c.radius
			c.dy=c.dy*-0.9
		End If
		If c.y>GraphicsHeight()-c.radius
			c.y=GraphicsHeight()-c.radius
			c.dy=c.dy*-0.9
		End If
	Next
End Function

Function RenderBalls()
	For Local c:ballType = EachIn ballList
		SetColor c.r,c.g,c.b
		DrawOval c.x-c.radius, c.y-c.radius, c.radius*2, c.radius*2
	Next
End Function

Function dist%(x1#, y1#, x2#, y2#)
	Local xd% = Abs(x1# - x2#) , yd% = Abs(y1# - y2#)
	Return Sqr(xd * xd + yd * yd)
End Function

Function angle%(x1#,y1#,x2#,y2#)
	Local angle%=ATan2(x2-x1,y2-y1)
	Return 180-angle
End Function
