/* example1.c                                                      */
/*                                                                 */
/* This small program shows how to print a rotated string with the */
/* FreeType 2 library.                                             */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#define WIDTH   640
#define HEIGHT  480

#define FONT_WIDTH 74
#define DEVICE_ASPECT 640 / 480

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];

/* Replace this function with something useful. */


void save_image(unsigned char* buff, int w, int h)
{
	char tga_header[12] =
	{ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	FILE* fp = fopen("./img.tga", "wb");

	fwrite(tga_header, 1, sizeof(tga_header), fp);

	unsigned char header6[6];

	header6[0] = w % 256;
	header6[1] = w / 256;
	header6[2] = h % 256;
	header6[3] = h / 256;
	header6[4] = 24;
	header6[5] = 0;

	fwrite(header6, 1, sizeof(header6), fp);

	int i;
	for(i = h - 1; i > -1; i--)
	{
		int j;
		for(j = 0; j < w; j++)
		{
			fwrite(&buff[i * w + j], 1, sizeof(unsigned char), fp);
			fwrite(&buff[i * w + j], 1, sizeof(unsigned char), fp);
			fwrite(&buff[i * w + j], 1, sizeof(unsigned char), fp);
		}
	}

	fclose(fp);
}


void draw_bitmap(FT_Bitmap* bitmap, FT_Int x, FT_Int y)
{
	FT_Int i, j, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;

	for (i = x, p = 0; i < x_max; i++, p++)
	{
		for (j = y, q = 0; j < y_max; j++, q++)
		{
			if(i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
				continue;

			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}

void show_image(void)
{
	int i, j;

	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
			putchar(image[i][j] == 0 ? ' ' : image[i][j] < 128 ? '+' : '*');
		putchar('\n');
	}

	save_image(image, img_size.x, img_size.y);
}

int main(int argc, char** argv)
{
	FT_Library library;
	FT_Face face;

	FT_GlyphSlot slot;
	FT_Matrix matrix; /* transformation matrix */
	FT_Vector pen; /* untransformed origin  */
	FT_Error error;
	FT_Glyph glyphs[128];

	char* filename;
	char* text;

	double angle;
	int target_height;
	int n, num_chars;

	filename = "Ruritania-Outline.ttf"; /* first argument     */
	text = "AV"; /* second argument    */
	num_chars = strlen(text);
	angle = 0; /* use 25 degrees     */
	target_height = HEIGHT;

	error = FT_Init_FreeType(&library); /* initialize library */
	/* error handling omitted */

	error = FT_New_Face(library, "Ruritania-Outline.ttf", 0, &face); /* create face object */
	/* error handling omitted */

	/* use 50pt at 100dpi */
	//error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0); /* set character size */
	FT_Set_Pixel_Sizes(face, FONT_WIDTH, DEVICE_ASPECT * FONT_WIDTH);
	/* error handling omitted */

	slot = face->glyph;

	/* set up matrix */
	matrix.xx = (FT_Fixed) (cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed) (-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed) (sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed) (cos(angle) * 0x10000L);

	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (300,200) relative to the upper left corner  */
	pen.x = 0;
	pen.y = HEIGHT / 2;

	for (n = 0; n < num_chars; n++)
	{
		/* set transformation */
		FT_Set_Transform(face, 0, &pen);

		FT_UInt glyph_index = FT_Get_Char_Index(face, text[n]);

		FT_Get_Glyph(slot, )

		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);


		FT_BBox bbox;
		FT_Glyph_Get_CBox(slot, ft_glyph_bbox_pixels, &bbox);

		if(error)
			continue; /* ignore errors */

		/* now, draw to our target surface (convert position) */
		draw_bitmap(&slot->bitmap, slot->bitmap_left, slot->bitmap_top);

		/* increment pen position */
		//pen.x += slot->advance.x;
		pen.x += bbox.xMax;
		pen.y += slot->advance.y;
	}

	save_image(image, WIDTH, HEIGHT);

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return 0;
}

/* EOF */
