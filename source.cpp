#include <Windows.h>
#include <glew.h>
#include <freeglut.h>
#include <time.h>
#include <iostream>
#include <sstream>

#define COLUMNS		40.0
#define ROWS		40.0

#define LEFT		0
#define UP			1
#define RIGHT		2
#define DOWN		3

GLdouble xSnake[39*39]{ 30,29,28,27,26,25,24 };
GLdouble ySnake[39*39]{ 30,30,30,30,30,30,30 };
GLdouble xFood = 0;
GLdouble yFood = 0;

static UINT16 SnakeLength = 1;
static UINT16 Speed = 260;
static UINT16 CurrentDirection = LEFT;
static UINT64 Score = 0;

VOID DisplayCallback(
	VOID
);

/*
*	@param Width  #of screen
*	@param Height #of screen
*/

VOID ReshapeCallback(
	INT Width,
	INT Height
);

/*
*	@param Key, status of pressed key
*	Please ignore other parameters
*/

VOID KeyCallback(
	INT Key,
	INT,
	INT
);

VOID init();

/*
*	@param x, coordinate on x axis
*	@param y, coordinate on y axis
*/
VOID DrawSingleSquare(
	GLfloat x,
	GLfloat y
);

VOID DrawGrid(VOID);
VOID DrawFood(VOID);
VOID DrawSnake(VOID);
VOID RandomizeCoordinateOfFood(VOID);
BOOL HasTouchtItSelf(VOID);

VOID loop(
	INT
);

int main(
	int argc,
	char** argv
)
{
	FreeConsole();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(DisplayCallback);
	glutReshapeFunc(ReshapeCallback);
	glutTimerFunc(0, loop, 0);
	glutSpecialFunc(KeyCallback);

	init();
	glutMainLoop();
}

VOID DisplayCallback(
	VOID
)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	DrawGrid();
	DrawSnake();

	if (HasTouchtItSelf())
	{
		std::ostringstream text;
		text << "Score: " << SnakeLength * 2;

		MessageBox(NULL, text.str().c_str(), "GAME OVER!", MB_OK);
		ExitProcess(EXIT_SUCCESS);
	}

	DrawFood();

	glutSwapBuffers();
}

VOID ReshapeCallback(
	INT Width,
	INT Height
)
{
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

VOID KeyCallback(
	INT key,
	INT,
	INT
)
{
	switch (key)
	{

	case GLUT_KEY_UP:
		if (CurrentDirection != DOWN)
			CurrentDirection = UP;
	break;

	case GLUT_KEY_RIGHT:
		if (CurrentDirection != LEFT)
			CurrentDirection = RIGHT;
	break;

	case GLUT_KEY_LEFT:
		if (CurrentDirection != RIGHT)
			CurrentDirection = LEFT;
	break;

	case GLUT_KEY_DOWN:
		if (CurrentDirection != UP)
			CurrentDirection = DOWN;
	break;

	}
}

VOID init()
{
	glClearColor((GLclampf)0.1, (GLclampf)0.1, (GLclampf)0.1, (GLclampf)1.0);
	RandomizeCoordinateOfFood();
}

VOID DrawSingleSquare(
	GLfloat x,
	GLfloat y
)
{
	glLineWidth((GLfloat)2.6);

	if ((!x) || x == COLUMNS - 1 || (!y) || y == ROWS - 1)
		glColor3f((GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);
	else
		glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0);

	glBegin(GL_LINE_LOOP);

	glVertex2f(x, y);
	glVertex2f(x, y + 1);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x + 1, y);

	glEnd();
}

VOID DrawGrid(VOID)
{
	for (GLfloat i = 0; i < COLUMNS; ++i)
		for (GLfloat j = 0; j < ROWS; ++j)
			DrawSingleSquare(i, j);
}

VOID DrawFood(VOID)
{
	glColor3d((GLdouble)1.0, (GLdouble)0.3, (GLdouble)0.0);
	glRectd(xFood,yFood, xFood + 1, yFood+ 1);
}

VOID DrawSnake(VOID)
{
	glColor3f((GLfloat)0.3, (GLfloat)0.8, (GLfloat)0.0);

	for (int i = SnakeLength; i > 0; --i)
	{
		if (!(i - 1))
			glColor3d(1.0, 1.0, 1.0);
		xSnake[i] = xSnake[i - 1];
		ySnake[i] = ySnake[i - 1];
		glRectd(xSnake[i], ySnake[i], xSnake[i] + 1, ySnake[i] + 1);
	}

	switch (CurrentDirection)
	{
	case LEFT:	--xSnake[0];	break;
	case UP:	++ySnake[0];	break;
	case RIGHT: ++xSnake[0];	break;
	case DOWN:	--ySnake[0];	break;
	}
	
	if (xSnake[0] >= COLUMNS - 1)
		xSnake[0] = 1;
	else if (xSnake[0] <= 0)
		xSnake[0] = COLUMNS - 2;
	if (ySnake[0] >= ROWS - 1)
		ySnake[0] = 1;
	else if (ySnake[0] <= 0)
		ySnake[0] = ROWS - 2;
	
	if (ySnake[0] == yFood && xSnake[0] == xFood)
	{
		RandomizeCoordinateOfFood();
		
		if (Speed - 4 >= 0)
			Speed -= 4;
		++SnakeLength;
	}
}

VOID RandomizeCoordinateOfFood(VOID)
{
	srand((unsigned int)time(NULL));
	xFood = rand() % 37;
	yFood = rand() % 37;

	xFood++;
	yFood++;
}

BOOL HasTouchtItSelf(VOID)
{
	for (int i = SnakeLength; i > 0 ; --i)
		if (xSnake[0] == xSnake[i] && ySnake[0] == ySnake[i])
			return TRUE;
	return FALSE;
}

VOID loop(INT)
{
	glutPostRedisplay();
	glutTimerFunc(Speed, loop, 0);
}