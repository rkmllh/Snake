#include <Windows.h>
#include <glew.h>
#include <freeglut.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define memzero(base,size)	\
			memset(base, 0, size)

#define COLUMNS		40.0
#define ROWS		40.0

#define LEFT		0
#define UP			1
#define RIGHT		2
#define DOWN		3

#define MATRIX_SIZE		39

#define BONUS		2
#define BUFF_SIZE	256

GLdouble xSnake[MATRIX_SIZE * MATRIX_SIZE]{ 30,29,28,27,26,25,24 };
GLdouble ySnake[MATRIX_SIZE * MATRIX_SIZE]{ 30,30,30,30,30,30,30 };
GLdouble xFood = 0;
GLdouble yFood = 0;

static UINT16 snake_length = 1;
static UINT16 Speed = 260;
static UINT16 current_direction = LEFT;
static UINT64 Score = 0;

VOID display_callback(
	VOID
);

/*
*	@param Width  #of screen
*	@param Height #of screen
*/

VOID reshape_callback(
	INT Width,
	INT Height
);

/*
*	@param Key, status of pressed key
*	Please ignore other parameters
*/

VOID key_callback(
	INT Key,
	INT,
	INT
);

VOID init();

/*
*	@param x, coordinate on x axis
*	@param y, coordinate on y axis
*/

VOID draw_single_square(
	GLfloat x,
	GLfloat y
);

VOID draw_grid(VOID);
VOID draw_food(VOID);
VOID draw_snake(VOID);
VOID randomize_food(VOID);
BOOL has_touched_itself(VOID);

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

	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutTimerFunc(0, loop, 0);
	glutSpecialFunc(key_callback);

	init();
	glutMainLoop();
}

VOID display_callback(
	VOID
)
{
	char buffer[BUFF_SIZE + sizeof(char)];
	memzero(buffer, BUFF_SIZE);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	draw_grid();
	draw_snake();

	if (has_touched_itself())
	{
		snprintf(buffer, BUFF_SIZE, "Your snake is died. You lost!");

		MessageBox(NULL, buffer, "GAME OVER!", MB_OK);
		ExitProcess(EXIT_SUCCESS);
	}

	draw_food();

	glutSwapBuffers();
}

VOID reshape_callback(
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

VOID key_callback(
	INT key,
	INT,
	INT
)
{
	switch (key)
	{

	case GLUT_KEY_UP:
		if (current_direction != DOWN)
			current_direction = UP;
	break;

	case GLUT_KEY_RIGHT:
		if (current_direction != LEFT)
			current_direction = RIGHT;
	break;

	case GLUT_KEY_LEFT:
		if (current_direction != RIGHT)
			current_direction = LEFT;
	break;

	case GLUT_KEY_DOWN:
		if (current_direction != UP)
			current_direction = DOWN;
	break;

	}
}

VOID init()
{
	glClearColor((GLclampf)0.1, (GLclampf)0.1, (GLclampf)0.1, (GLclampf)1.0);
	randomize_food();
}

VOID draw_single_square(
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

VOID draw_grid(VOID)
{
	for (GLfloat i = 0; i < COLUMNS; ++i)
		for (GLfloat j = 0; j < ROWS; ++j)
			draw_single_square(i, j);
}

VOID draw_food(VOID)
{
	glColor3d((GLdouble)1.0, (GLdouble)0.3, (GLdouble)0.0);
	glRectd(xFood,yFood, xFood + 1, yFood+ 1);
}

VOID draw_snake(VOID)
{
	glColor3f((GLfloat)0.3, (GLfloat)0.8, (GLfloat)0.0);

	for (int i = snake_length; i > 0; --i)
	{
		if (!(i - 1))
			glColor3d(1.0, 1.0, 1.0);
		xSnake[i] = xSnake[i - 1];
		ySnake[i] = ySnake[i - 1];
		glRectd(xSnake[i], ySnake[i], xSnake[i] + 1, ySnake[i] + 1);
	}

	switch (current_direction)
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
		randomize_food();
		
		if (Speed - 4 >= 0)
			Speed -= 4;
		++snake_length;
	}
}

VOID randomize_food(VOID)
{
	srand((unsigned int)time(NULL));
	xFood = rand() % MATRIX_SIZE - 2;
	yFood = rand() % MATRIX_SIZE - 2;

	xFood++;
	yFood++;
}

BOOL has_touched_itself(VOID)
{
	for (int i = snake_length; i > 0 ; --i)
		if (xSnake[0] == xSnake[i] && ySnake[0] == ySnake[i])
			return TRUE;
	return FALSE;
}

VOID loop(INT)
{
	glutPostRedisplay();
	glutTimerFunc(Speed, loop, 0);
}