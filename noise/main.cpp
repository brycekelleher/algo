#include <stdio.h>
#include "GL/freeglut.h"
#include "perlin.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int windowWidth;
int windowHeight;
int redrawDelay = 1000 / 60;

int g_nNoiseTexture = 1;
float fNoiseTime;

// Debugging function for noise
void WriteNoiseRawFile()
{
	static unsigned char data[65536];
	int x, y;

	for(y = 0; y < 256; y++)
	{
		for(x = 0; x < 256; x++)
		{
			float n = 0.5f * pnoise((float)x / 16.0f, (float)y, 0) + 0.5f;
			data[y * 256 + x] = (unsigned char)(n * 255.0f);
		}
	}

	FILE* fp = fopen("noise.raw", "wb");
	fwrite(data, 65536, sizeof(unsigned char), fp);
	fclose(fp);
}

void DoNoiseTexture()
{
	static unsigned char s_data[65536];
	int x, y;

	// Generate the noise data
	for(y = 0; y < 64; y++)
	{
		for(x = 0; x < 64; x++)
		{
			float n = 0.5f * pnoise((float)x / 16.0f, (float)y / 16.0f, fNoiseTime) + 0.5f;
			s_data[y * 64 + x] = (unsigned char)(n * 255.0f);
		}
	}

	glBindTexture(GL_TEXTURE_2D, g_nNoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 64, 64, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, s_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void DrawQuad()
{
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_POLYGON);
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f( 1.0f, -1.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f( 1.0f,  1.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f,  1.0f);
	}
	glEnd();
}

void OnDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	DoNoiseTexture();
	DrawQuad();
	
	glutSwapBuffers();
}

void OnResize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void OnMouseButton(int button, int state, int xPos, int yPos)
{}

void OnMouseMove(int x, int y)
{}

void OnTick(int timerId)
{
	fNoiseTime += 0.05f;
	glutPostRedisplay();
	glutTimerFunc(redrawDelay, OnTick, 0);
}

void OnKeyChar(unsigned char key, int x, int y)
{}

int main(int argc, const char *argv[])
{
    /* glut init */
    glutInit(&argc, (char **) argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Draw");

    /* gl init */
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    OnResize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* register glut event callbacks */
    glutReshapeFunc(OnResize);
    glutDisplayFunc(OnDisplay);
    glutKeyboardFunc(OnKeyChar);
    glutMouseFunc(OnMouseButton);
    glutMotionFunc(OnMouseMove);
    glutPassiveMotionFunc(OnMouseMove);
    glutTimerFunc(redrawDelay, OnTick, 0);

    /* begin event loop */
    glutMainLoop();

    return 0;
}
