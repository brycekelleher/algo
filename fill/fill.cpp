#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "imagedata3.inc"
static int imagew = 512;
static int imageh = 512;

struct dpixel
{
	//int	dx;
	//int	dy;
	int	px;
	int	py;
	float	d;
};

struct texel
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	s;
};

static texel*	texture;
static texel*	dilation;
static dpixel*	distance;

static void* MallocCleared(int numbytes)
{
	void *mem = malloc(numbytes);
	memset(mem, 0, numbytes);

	return mem;
}

static int IntAbs(int x)
{
	if(x < 0)
		x = -x;
	
	return x;
}

static int IntMin(int a, int b)
{
	return (a < b ? a : b);
}
	
static int IntMax(int a, int b)
{
	return (a > b ? a : b);
}

dpixel *AllocDistanceSurface(int w, int h)
{
	return (dpixel*)MallocCleared(w * h * sizeof(dpixel));
}

texel *AllocTextureSurface(int w, int h)
{
	return (texel*)MallocCleared(w * h * sizeof(texel));
}

static int CalculateAddr(int x, int y)
{
	return y * imagew + x;
}

static texel GetTexel(texel *texels, int x, int y)
{
	int addr = CalculateAddr(x, y);

	return texels[addr];
}

static void SetTexel(texel *texels, int x, int y, texel t)
{
	int addr = CalculateAddr(x, y);

	texels[addr] = t;
}

static dpixel GetDexel(dpixel *dexels, int x, int y)
{
	int addr = CalculateAddr(x, y);

	return dexels[addr];
}

static void SetDexel(dpixel *dexels, int x, int y, dpixel d)
{
	int addr = CalculateAddr(x, y);

	dexels[addr] = d;
}

typedef void (*imagefunc_t)(int x, int y, void *a, void* b);

// transform one image to another
static void ImageTransform(void *a, void *b, imagefunc_t func)
{
	for(int y = 0; y < imageh; y++)
	{
		for(int x = 0; x < imagew; x++)
		{
			func(x, y, a, b);
		}
	}
}

static void AllocSurfaces()
{
	distance = AllocDistanceSurface(imagew, imageh);

	texture = AllocTextureSurface(imagew, imageh);

	dilation = AllocTextureSurface(imagew, imageh);
}

void CopyTextureData(int x, int y, void *a, void* b)
{
	texel t;
	int addr = CalculateAddr(x, y);

	t.r	= imagedata[addr][0];
	t.g	= imagedata[addr][1];
	t.b	= imagedata[addr][2];

	// mask out alpha
	t.r	*= (imagedata[addr][3] ? 1 : 0);
	t.g	*= (imagedata[addr][3] ? 1 : 0);
	t.b	*= (imagedata[addr][3] ? 1 : 0);

	// fake a stencil stencil test
	// in reality the triangle would be rasterized with always pass and increment the stencil
	// fake this by setting all non-black pixels to 1
	//t.s	= ((!t.r && !t.g && !t.b) ? 0 : ~0);
	t.s	= (!imagedata[addr][3] ? 0 : ~0);

	SetTexel(texture, x, y, t);
}

static void BuildTextureSurface()
{
	ImageTransform(texture, NULL, CopyTextureData);
}

static void FillDistances(int x, int y, void *a, void *b)
{
	texel t		= GetTexel(texture, x, y);
	dpixel d	= GetDexel(distance, x, y);

	d.px	= x;
	d.py	= y;
	d.d	= (!t.s ? 100000.0f : 0.0f);

	SetDexel(distance, x, y, d);
}

static void SetupDistanceSurface()
{
	// texture and distance are backwards?
	ImageTransform(texture, distance, FillDistances);
}

static bool RangeCheck(int x, int y)
{
	if(x < 0 || y < 0)
		return false;
	if(x >= imagew || y >= imageh)
		return false;

	return true;
}

#if 0
// compares the distance at x y against the offset and returns
// a new dexel that has an updated distance
// manhattan distance
static dpixel Compare(dpixel d, int x, int y, int dx, int dy)
{
	if(!RangeCheck(x + dx, y + dy))
		return d;

	dpixel s = GetDexel(distance, x + dx, y + dy);

	dx = IntAbs(dx);
	dy = IntAbs(dy);

	// if the distance is better than what we have update d
	if(s.d + dx + dy < d.d)
	{
		d.d = s.d + dx + dy;
		d.px = s.px;
		d.py = s.py;
	}

	return d;
}
#endif

#if 0
static dpixel Compare(dpixel d, int x, int y, int dx, int dy)
{
	if(!RangeCheck(x + dx, y + dy))
		return d;

	dpixel s = GetDexel(distance, x + dx, y + dy);

	dx = IntAbs(dx);
	dy = IntAbs(dy);

	float dd = ((dx == 0 || dy == 0) ? 12 : 17);

	// if the distance is better than what we have update d
	if(s.d + dd < d.d)
	{
		d.d = s.d + dd;
		d.px = s.px;
		d.py = s.py;
	}

	return d;
}
#endif

#if 0
// exact float difference
static dpixel Compare(dpixel d, int x, int y, int dx, int dy)
{
	if(!RangeCheck(x + dx, y + dy))
		return d;

	dpixel s = GetDexel(distance, x + dx, y + dy);

	// take the abs of dx and dy
	dx = IntAbs(dx);
	dy = IntAbs(dy);

	float dd = s.d + sqrtf((dx * dx) + (dy * dy));

	// squared distance
	// if the distance is better than what we have update d
	if(dd < d.d)
	{
		d.d = dd;
		d.px = s.px;
		d.py = s.py;
	}

	return d;
}
#endif

#if 1
// dead reckoning
static dpixel Compare(dpixel d, int x, int y, int dx, int dy)
{
	if(!RangeCheck(x + dx, y + dy))
		return d;

	dpixel s = GetDexel(distance, x + dx, y + dy);

	// take the abs of dx and dy
	dx = IntAbs(dx);
	dy = IntAbs(dy);

	float dd = ((dx == 0 || dy == 0) ? 1 : sqrtf(2));

	// if the distance is better than what we have update d
	if(s.d + dd < d.d)
	{
		// calculate a new dead reckoned version of the distance
		dd = sqrtf(((x - s.px) * (x - s.px)) + ((y - s.py) * (y - s.py)));

		d.d = dd;
		d.px = s.px;
		d.py = s.py;
	}

	return d;
}
#endif

static void ComputeDistances()
{
	int x, y;
	dpixel d;

	for(y = 0; y < imageh; y++)
	{
		for(x = 0; x < imagew; x++)
		{
			d = GetDexel(distance, x, y);
			d = Compare(d, x, y, -1, 0);
			d = Compare(d, x, y, 0, -1);
			d = Compare(d, x, y, -1, -1);
			d = Compare(d, x, y, 1, -1);
			SetDexel(distance, x ,y, d);
		}

		for(x = imagew - 1; x >= 0; x--)
		{
			d = GetDexel(distance, x, y);
			d = Compare(d, x, y, 1, 0);
			SetDexel(distance, x ,y, d);
		}
	}

	for(y = imageh - 1; y >= 0; y--)
	{
		for(x = imagew - 1; x >= 0; x--)
		{
			d = GetDexel(distance, x, y);
			d = Compare(d, x, y, 1, 0);
			d = Compare(d, x, y, 0, 1);
			d = Compare(d, x, y, -1, 1);
			d = Compare(d, x, y, 1, 1);
			SetDexel(distance, x ,y, d);
		}

		for(x = 0; x < imagew; x++)
		{
			d = GetDexel(distance, x, y);
			d = Compare(d, x, y, -1, 0);
			SetDexel(distance, x ,y, d);
		}
	}
}

#if 0
// for each pixel, if it's a valid pixel, calculate the distance and take the minimum of all of them
void ComputePixelDistanceBruteForce(int x, int y, void *a, void *b)
{
	dpixel d;
	texel t;

	int xx, yy;

	// find the minimum distance to all valid pixels
	for(yy = 0; yy < imageh; yy++)
	{
		for(xx = 0; xx < imagew; xx++)
		{
			d = GetDexel(distance, x, y);

			// skip invalid pixels
			dpixel s = GetDexel(distance, xx, yy);
			texel t = GetTexel(texture, xx, yy);

			if(t.r == 0 && t.g == 0 && t.b == 0)
				continue;
			
			int dx = xx - x;
			int dy = yy - y;

			if(dx < 0)
				dx = -dx;
			if(dy < 0)
				dy = -dy;

			float d1 = sqrtf((dx * dx) + (dy * dy));

			if(d1 <= d.floatd)
			{
				d.dx = s.dx + dx;
				d.dy = s.dy + dy;
				d.px = s.px;
				d.py = s.py;
				d.floatd = d1;

				SetDexel(distance, x, y, d);
			}
		}
	}
}

static void ComputeDistancesBruteForce()
{
	ImageTransform(distance, texture, ComputePixelDistanceBruteForce);
}
#endif

static void DilatePixel(int x, int y, void *ia, void *ib)
{
	texel *dilation = (texel*)ia;
	texel *texture = (texel*)ib;

	// filter the neighbourhood pixels
	float r, g, b;
	r = g = b = 0;
	float w = 0;
	for(int xx = 0; xx < 3; xx++)
	{
		for(int yy = 0; yy < 3; yy++)
		{
			int offsets[3][3][2] = 
			{
				{ { -1, -1 }, { 0, -1 }, { 1, -1 } },
				{ { -1,  0 }, { 0,  0 }, { 1,  0 } },
				{ { -1,  1 }, { 0,  1 }, { 1,  1 } },
			};

			float weights[3][3] = 
			{
				{ 0.7f, 1.0f, 0.7f },
				{ 1.0f, 0.0f, 1.0f },
				{ 0.7f, 1.0f, 0.7f }
			};

			int px = x + offsets[xx][yy][0];
			int py = y + offsets[xx][yy][1];

			if(!RangeCheck(px, py))
				continue;

			texel st = GetTexel(texture, px, py);

			//if(!st.s)
			//	continue;

			r += weights[xx][yy] * st.r;
			g += weights[xx][yy] * st.g;
			b += weights[xx][yy] * st.b;
			w += weights[xx][yy] * (st.s / 255.0f);
		}
	}

	// prevent divide by zero
	if(!w)
		return;

	//printf("weight is: %f\n", weight);

	texel sum;
	sum.r = r / w;
	sum.g = g / w;
	sum.b = b / w;
	sum.s = 0xff;

#if 1	
	// perform the stencil test
	texel t = GetTexel(texture, x, y);
	if(t.s)
	{
		SetTexel(dilation, x, y, t);
		return;
	}
#endif
	
	SetTexel(dilation, x, y, sum);
}

static void DilateTexture()
{
	ImageTransform(dilation, texture, DilatePixel);
	ImageTransform(texture, dilation, DilatePixel);
	ImageTransform(dilation, texture, DilatePixel);
	ImageTransform(texture, dilation, DilatePixel);
	ImageTransform(dilation, texture, DilatePixel);
	ImageTransform(texture, dilation, DilatePixel);
	ImageTransform(dilation, texture, DilatePixel);
	ImageTransform(texture, dilation, DilatePixel);
}

static void OutlinePixel(int x, int y, void *ia, void *ib)
{
	texel *dilation = (texel*)ia;
	texel *texture = (texel*)ib;

	static int offsets[3][3][2] = 
	{
		{ { -1, -1 }, { 0, -1 }, { 1, -1 } },
		{ { -1,  0 }, { 0,  0 }, { 1,  0 } },
		{ { -1,  1 }, { 0,  1 }, { 1,  1 } },
	};

	// check the destination pixel
	texel dst = GetTexel(texture, x, y);

	if(dst.s)
	{
		SetTexel(dilation, x, y, dst);
		return;
	}

	// fill the destination pixel with the first valid source pixel
	for(int xx = 0; xx < 3; xx++)
	{
		for(int yy = 0; yy < 3; yy++)
		{
			int px = x + offsets[xx][yy][0];
			int py = y + offsets[xx][yy][1];

			if(!RangeCheck(px, py))
				continue;

			texel src = GetTexel(texture, px, py);
			if(src.s)
			{
				SetTexel(dilation, x, y, src);
				return;
			}
		}
	}
}


static void OutlineTexture()
{
	ImageTransform(dilation, texture, OutlinePixel);
	ImageTransform(texture, dilation, OutlinePixel);
	ImageTransform(dilation, texture, OutlinePixel);
	ImageTransform(texture, dilation, OutlinePixel);
	ImageTransform(dilation, texture, OutlinePixel);
	ImageTransform(texture, dilation, OutlinePixel);
	ImageTransform(dilation, texture, OutlinePixel);
	ImageTransform(texture, dilation, OutlinePixel);
}

static void FillTexels(int x, int y, void *a, void *b)
{
	dpixel d	= GetDexel(distance, x, y);
	texel t		= GetTexel(texture, x, y);

	if(d.d > 10)
		return;

	if(!t.s)
	{
		texel st = GetTexel(texture, d.px, d.py);

		SetTexel(texture, x, y, st);
	}
}

static void FillTexture()
{
	ImageTransform(distance, texture, FillTexels);
}

static void WriteReferenceSurface()
{
	int x, y;

	FILE *fp = fopen("reference.ppm", "w");

	fprintf(fp, "P3\n%d\n%d\n255\n", imagew, imageh);

	for(y = 0; y < imageh; y++)
	{
		for(x = 0; x < imagew; x++)
		{
			texel t = GetTexel(texture, x, y);
			fprintf(fp, "\t%3i %3i %3i", t.r, t.g, t.b);
		}

		fprintf(fp, "\n");
	}
}

static void WriteTextureSurface()
{
	int x, y;

	FILE *fp = fopen("texture.ppm", "w");

	fprintf(fp, "P3\n%d\n%d\n255\n", imagew, imageh);

	for(y = 0; y < imageh; y++)
	{
		for(x = 0; x < imagew; x++)
		{
			texel t = GetTexel(texture, x, y);
			fprintf(fp, "\t%3i %3i %3i", t.r, t.g, t.b);
		}

		fprintf(fp, "\n");
	}
}

static void WriteDistanceSurface()
{
	int x, y;

	FILE *fp = fopen("distance.ppm", "w");

	fprintf(fp, "P3\n%d\n%d\n255\n", imagew, imageh);

	for(y = 0; y < imageh; y++)
	{
		for(x = 0; x < imagew; x++)
		{
			dpixel d = GetDexel(distance, x, y);

			int i = d.d * 4.0;
			if(i > 255)
				i = 255;

			fprintf(fp, "\t%3i %3i %3i", i, i, i);
		}

		fprintf(fp, "\n");
	}
}

static void DoWork()
{
	AllocSurfaces();

	BuildTextureSurface();

	WriteReferenceSurface();

	SetupDistanceSurface();

	ComputeDistances();

	//FillTexture();
	//DilateTexture();
	OutlineTexture();

	WriteTextureSurface();

	WriteDistanceSurface();
}

int main(int argc, char* argv[])
{
	DoWork();
}
