#include <stdio.h>
#include <stdlib.h>

void plot(int x, int y)
{
	printf("x:%i, y:%i\n", x, y);
}

void draw_line(int x0, int y0, int x1, int y1)
{
	int x = x0;
	int y = y0;
	float delta = (float)(y1 - y0) / (float)(x1 - x0);
	float error = 0;

	for (int i = x0; i <= x1; i++)
	{
		plot(x, y);

		//setup for the next pixel
		x++;
		error += delta;
		if (error > 0.5f)
		{
			y += 1;
			error -= 1;
		}
	}
}

int main(int argc, char *argv[])
{
	printf("drawing line\n");
	draw_line(1, 0, 6, 1);
}
