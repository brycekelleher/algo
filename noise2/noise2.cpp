#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "GL/freeglut.h"

#define 	PI   3.14159265358979323846	/* pi */

#if 0
#define 	PI_2   1.57079632679489661923	/* pi/2 */
#define 	PI_4   0.78539816339744830962	/* pi/4 */
#define 	1_PI   0.31830988618379067154	/* 1/pi */
#define 	2_PI   0.63661977236758134308	/* 2/pi */
#endif

#include "functions.cpp"

// perlin noise function
float pnoise(float x, float y, float z);

#define WINDOW_WIDTH	512
#define WINDOW_HEIGHT	512

static int windowwidth		= WINDOW_WIDTH;
static int windowheight		= WINDOW_HEIGHT;
static float worldx;
static float worldy;

static int tickmsec = 16;
static int currenttime = 0;
static int animtime = 0;
GLuint glnoisetexture = 0;
GLuint glflowtexture = 0;
bool drawnoise = false;
bool drawcurlfield = true;
bool drawnodes = true;

static void hsv_to_rgb(float rgb[3], float h, float s, float v)
{
	float r, g, b;

	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	switch(i % 6)
	{
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}

	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

static float StepAt(float x, float min, float max)
{
	return Step(x - min) - Step(x - max);
}

static float StepMap(float x)
{
	return 
		(0.0 * StepAt(x, 0.0f, 0.25f)) + 
		(0.25f * StepAt(x, 0.25f, 0.5f)) +
		(0.75f * StepAt(x, 0.5f, 0.75f)) +
		(1.0f * StepAt(x, 0.75f, 1.0f));
}

#if 1
static float Noise(float x, float y)
{
	//currenttime = 0;
	float n = pnoise(x, y, currenttime / 1000.0f);

	return n;
}
#endif

#if 0
// Torodial noise
static float Noise(float x, float y)
{
	float xx, yy, zz;

	xx = (1.0f + 1.0f * cos(x)) * cos(y);
	yy = (1.0f + 1.0f * cos(x)) * sin(y);
	zz = 1.0f * sin(x);

	return pnoise(xx, yy, zz);
}
#endif

static float FBM(float x, float y)
{
	float frequency = 1.0f;
	float amplitude = 1.0f;
	float lacunarity = 2.0f;
	float gain = 0.5f;
	int octaves = 4;

	float sum = 0.0f;
	for(int i = 0; i < octaves; i++)
	{
		sum += amplitude * Noise(frequency * x, frequency * y);

		frequency *= lacunarity;
		amplitude *= gain;
	}

	return sum;
}


// 'World' code
static float xscale = 8.0f;
static float yscale = 8.0f;
static float tscale = 1.0f;

static float StepMap2(float x)
{
	return 
		(0.0 * StepAt(x, 0.0f, 0.25f)) + 
		(0.25f * StepAt(x, 0.25f, 0.5f)) +
		(0.75f * StepAt(x, 0.5f, 0.75f)) +
		(1.0f * StepAt(x, 0.75f, 1.0f));
}

static float Potential(float x, float y)
{
	//x += FBM(3.81 * x, 2.72 * y);
	//y += FBM(2.45 * x, 3.12 * y);

	float n = Noise(xscale * x, yscale * y);
	// scale and bias
	n = 0.5 * n + 0.5f;

	//n = 1.0f;

	//n = SmootherStep(n);

	// apply the distance window
	n += Clamp((2.0f * CosWindow(x - 0.5f, 0.5f) * CosWindow(y - 0.5f, 0.5f)), 0, 1);

	// apply the timing
	//n *= Clamp(0.5f + 0.5f * sin(0.5f * 3.14 * currenttime / 1000.0f), 0.0f, 1.0f);

	return n;
}

// calculate the gradient by forward differencing
static CVec2 PotentialGrad(float x, float y)
{
	float step = 0.001f;
	float p = Potential(x, y);
	float xx = Potential(x + step, y);
	float yy = Potential(x, y + step);

	return CVec2((xx - p) / step, (yy - p) / step);
}

static CVec2 PotentialCurl(float x, float y)
{
	CVec2 curl = PotentialGrad(x, y);
	curl = CVec2(curl.y, -curl.x);

	return curl;
}

float frand(float fmin, float fmax)
{
	return Lerp((rand() / (float)RAND_MAX), fmin, fmax);
}

CVec2 vrand()
{
	CVec2 v;
	v.x = ((float)rand() / (float)RAND_MAX) * WINDOW_WIDTH - (WINDOW_WIDTH / 2);
	v.y = ((float)rand() / (float)RAND_MAX) * WINDOW_HEIGHT - (WINDOW_HEIGHT / 2);

	return v;
}

// -----------------------------------
// node simulation

typedef struct node_s
{
	CVec2 xy;

} node_t;

#define MAX_NODES 4096

static node_t nodes[MAX_NODES];
static int numnodes = 0;

static void DrawNode(node_t *n)
{
	glColor3f(0, 0, 1);

	glBegin(GL_LINES);
	glVertex2f(n->xy.x - 0.01f, n->xy.y);
	glVertex2f(n->xy.x + 0.01f, n->xy.y);
	glVertex2f(n->xy.x, n->xy.y - 0.01f);
	glVertex2f(n->xy.x, n->xy.y + 0.01f);
	glEnd();
}

static void NodeApply(void (*fn)(node_t *n))
{
	for(int i = 0; i < numnodes; i++)
	{
		fn(nodes + i);
	}
}

static void NodeSpawn()
{
	node_t* n = nodes + numnodes;
	numnodes++;

	n->xy.x = frand(0, 1);
	n->xy.y = frand(0, 1);
	n->xy.x = 0.01f;
}

static void NodeRun(node_t *n)
{
	// get the node velocity from the Potential field
	CVec2 grad = PotentialGrad(n->xy.x, n->xy.y);

	CVec2 curl = PotentialCurl(n->xy.x, n->xy.y);

	CVec2 v = CVec2(0, 0);

	// constant velocity in the x direction
	v += CVec2(0.1f, 0.0f);

	v += 0.1f * curl;

	// Euler time step
	float dt = 1 / 60.0f;
	n->xy += dt * v;

	if(n->xy.x > 1.0f)
		n->xy.x -= 1.0f;
	if(n->xy.y > 1.0f)
		n->xy.y -= 1.0f;

	if(n->xy.x < 0.0f)
		n->xy.x += 1.0f;
	if(n->xy.y < 0.0f)
		n->xy.y += 1.0f;
}

static void SpawnNodes()
{
	for(int i = 0; i < 800; i++)
	{
		NodeSpawn();
	}
}

static void SimulateNodes()
{
	NodeApply(NodeRun);
}

// -----------------------------------
// rendering stuff

static void DrawQuad()
{
	glBegin(GL_POLYGON);
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f( 1.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f( 1.0f,  1.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f,  1.0f);
	}
	glEnd();
}

static void DoNoiseTexture()
{
	#define NOISE_TEXTURE_SIZE	128

	static unsigned char data[NOISE_TEXTURE_SIZE * NOISE_TEXTURE_SIZE];
	int x, y;

	// Generate the noise data
	for(y = 0; y < NOISE_TEXTURE_SIZE; y++)
	{
		for(x = 0; x < NOISE_TEXTURE_SIZE; x++)
		{
			// transform the texel location to world space
			float xx = (x / (float)NOISE_TEXTURE_SIZE) + (0.5f / NOISE_TEXTURE_SIZE);
			float yy = (y / (float)NOISE_TEXTURE_SIZE) + (0.5f / NOISE_TEXTURE_SIZE);

			//float n = Clamp(0.5f * Potential(xx, yy) + 0.5f, 0, 1);
			float n = Clamp(Potential(xx, yy), 0, 1);
			data[y * NOISE_TEXTURE_SIZE + x] = (unsigned char)(n * 255.0f);
		}
	}

	if(!glnoisetexture)
	{
		glGenTextures(1, &glnoisetexture);
	}

	glBindTexture(GL_TEXTURE_2D, glnoisetexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, NOISE_TEXTURE_SIZE, NOISE_TEXTURE_SIZE, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

static void DoFlowTexture()
{
	#define FLOW_TEXTURE_SIZE	128

	static unsigned char data[FLOW_TEXTURE_SIZE * FLOW_TEXTURE_SIZE * 3];
	int x, y;

	for(y = 0; y < FLOW_TEXTURE_SIZE; y++)
	{
		for(x = 0; x < FLOW_TEXTURE_SIZE; x++)
		{
			// linear texel address
			int address = (y * 3 * FLOW_TEXTURE_SIZE) + (x * 3);

			// transform the texel location to world space
			float xx = (x / (float)FLOW_TEXTURE_SIZE) + (0.5f / FLOW_TEXTURE_SIZE);
			float yy = (y / (float)FLOW_TEXTURE_SIZE) + (0.5f / FLOW_TEXTURE_SIZE);

			CVec2 curl = PotentialCurl(xx, yy);

			curl *= (1.0f / 8.0f);

			float r = 0.5f + 0.5f * Clamp(curl.x, -1.0f, 1.0f);
			float g = 0.5f + 0.5f * Clamp(curl.y, -1.0f, 1.0f);

			data[address + 0] = (unsigned char)(r * 255.0f);
			data[address + 1] = (unsigned char)(g * 255.0f);
			data[address + 2] = 0;
		}
	}

	if(!glflowtexture)
	{
		glGenTextures(1, &glflowtexture);
	}

	glBindTexture(GL_TEXTURE_2D, glflowtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FLOW_TEXTURE_SIZE, FLOW_TEXTURE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static void DrawNoise()
{
	if(!drawnoise)
		return;

	// update the noise texture
	DoNoiseTexture();

	// stretch a unit quad over the screen
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, glnoisetexture);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1, 1, 1);
	
	DrawQuad();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

static void DrawCursor()
{
	glColor3f(0, 0, 1);

	glBegin(GL_LINES);
	glVertex2f(worldx - 0.01f, worldy);
	glVertex2f(worldx + 0.01f, worldy);
	glVertex2f(worldx, worldy - 0.01f);
	glVertex2f(worldx, worldy + 0.01f);
	glEnd();
}

static void DrawGradient()
{
	CVec2 grad;

	grad = PotentialGrad(worldx, worldy);
	//printf("grad: %f, %f\n", grad.x, grad.y);

	grad.Normalize();

	glColor3f(0, 0, 1);

	glBegin(GL_LINES);
	glVertex2f(worldx, worldy);
	glVertex2f(worldx + (0.1f * grad.x), worldy + (0.1f * grad.y));
	glEnd();
}

static void SetVelocityColor(CVec2 curl)
{
	float l;
	float rgb[3];
       
	l = curl.Length();
	l = l / 16.0f;
	l = Clamp(l, 0.0f, 1.0f);
	//l = StepMap(l);

	// map 0..1 to hue
	l = Lerp(l, 0, 0.33f);

	//printf("l: %f\n", l);

	hsv_to_rgb(rgb, l, 1, 1);

	glColor3fv(rgb);
}

static void DrawCursorCurl()
{
	CVec2 curl;
	float length;

	curl = PotentialCurl(worldx, worldy);

	SetVelocityColor(curl);

	curl.Normalize();

	glBegin(GL_LINES);
	glVertex2f(worldx, worldy);
	glVertex2f(worldx + (0.1f * curl.x), worldy + (0.1f * curl.y));
	glEnd();
}

static void DrawCurlVector(float x, float y)
{
	CVec2 curl;
	float length;

	curl = PotentialGrad(x, y);
	curl = CVec2(curl.y, -curl.x);

	SetVelocityColor(curl);

	curl.Normalize();

	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + (0.01f * curl.x), y + (0.01f * curl.y));
	glEnd();
}

static void DrawCurlField()
{
	if(!drawcurlfield)
		return;

	int divisions = 100;

	for(int x = 0; x < divisions; x++)
	{
		for(int y = 0; y < divisions; y++)
		{
			float xx =  x * (1.0f / divisions) + (0.5f / divisions);
			float yy =  y * (1.0f / divisions) + (0.5f / divisions);

			DrawCurlVector(xx, yy);
		}
	}
}

static void DrawNodes()
{
	if(!drawnodes)
		return;

	NodeApply(DrawNode);
}

static void DisplayFunc()
{
	int i;

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	DrawNoise();

	DrawCurlField();

	DrawCursor();

	DrawGradient();

	DrawCursorCurl();

	DrawNodes();
	
	glutSwapBuffers();
}

static void ReshapeFunc(int width, int height)
{
	windowwidth	= width;
	windowheight	= height;

	glViewport(0, 0, windowwidth, windowheight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void OnMouseButton(int button, int state, int xPos, int yPos)
{}

static void UpdateMouse(int x, int y)
{
	float xx, yy;

	// calculate the position in the range 0 .. 1
	xx = (float)x / (float)(windowwidth);
	yy = (float)y / (float)(windowheight);
	yy = 1.0f - yy;

	// update the global cursor position state
	worldx = xx;
	worldy = yy;
}

void MouseMotionFunc(int x, int y)
{
	UpdateMouse(x, y);
}

void TimerFunc(int timerId)
{
	currenttime += tickmsec;

	SimulateNodes();

	glutPostRedisplay();

	glutTimerFunc(tickmsec, TimerFunc, 0);
}

void KeyFunc(unsigned char key, int x, int y)
{
	static int s_nNoiseExponent = 8;
	
	switch(key)
	{
		case 'c':
		{
			drawcurlfield = !drawcurlfield;
			break;
		}
		case 'n':
		{
			drawnoise = !drawnoise;
			break;
		}
		case 'd':
		{
			drawnodes = !drawnodes;
			break;
		}

		// noise x scaling
		case 'z':
		{
			xscale -= 1;
			break;
		}
		case 'x':
		{
			xscale += 1;
			break;
		}
		// noise y scaling
		case 'a':
		{
			yscale -= 1;
			break;
		}
		case 's':
		{
			yscale += 1;
			break;
		}
	}
}

int main(int argc, const char *argv[])
{
    /* glut init */
    glutInit(&argc, (char **) argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Draw");

    /* gl init */

    /* register glut event callbacks */
    glutReshapeFunc(ReshapeFunc);
    glutDisplayFunc(DisplayFunc);
    glutKeyboardFunc(KeyFunc);
    glutMouseFunc(OnMouseButton);
    glutMotionFunc(MouseMotionFunc);
    glutPassiveMotionFunc(MouseMotionFunc);
    glutTimerFunc(tickmsec, TimerFunc, 0);

    SpawnNodes();

    /* begin event loop */
    glutMainLoop();

    return 0;
}
