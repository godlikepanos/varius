#include <png.h>
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <cstdlib>

typedef unsigned char uchar;

using namespace std;


static GLuint texGlId;

int windowWidth = 256;
int windowHeight = 256;


/*
 * Stuff to check for memory leaks (Unix only)
 */
#include <malloc.h>

typedef struct mallinfo Mallinfo;

inline Mallinfo GetMallInfo()
{
	return mallinfo();
}

inline void printMallInfo(const Mallinfo& minfo)
{
	cout << "used:" << minfo.uordblks << " free:" << minfo.fordblks << " total:" << minfo.arena << endl;
}

inline void printMallInfoDiff(const Mallinfo& prev, const Mallinfo& now)
{
	Mallinfo diff;
	diff.uordblks = now.uordblks-prev.uordblks;
	diff.fordblks = now.fordblks-prev.fordblks;
	diff.arena = now.arena-prev.arena;
	printMallInfo(diff);
}

#define MALLINFO_BEGIN Mallinfo __m__ = GetMallInfo();

#define MALLINFO_END printMallInfoDiff(__m__, GetMallInfo());



#define IMG_ERR(x) \
	cerr << "File \"" << filename << "\": " << x << endl;

#define IMG_WARNING(x) \
	cerr << "File \"" << filename << "\": " << x << endl;

/**
 * Load PNG image
 * @param filename The file to load
 * @param colorType PNG_COLOR_TYPE_*
 * @param data The output data
 * @return True on success
 */
bool loadPng(const char* filename, uint& width, uint& height, uint& colorType, vector<uchar>& data)
{
	/*
	 * Data
	 */
	const uint PNG_SIG_SIZE = 8;
	FILE* file = NULL;
	png_structp pngPtr = NULL;
	png_infop infoPtr = NULL;
	bool ok = false;
	size_t charsRead;
	uint bitDepth;
	uint channels;
	uint rowbytes;
	vector<png_bytep> rowPointers;

	/*
	 * Open file
	 */
	file = fopen(filename, "rb");
	if(file == NULL)
	{
		IMG_ERR("Cannot open file");
		goto cleanup;
	}

	/*
	 * Validate PNG header
	 */
	png_byte pngsig[PNG_SIG_SIZE];
	charsRead = fread(pngsig, 1, PNG_SIG_SIZE, file);
	if(charsRead != PNG_SIG_SIZE)
	{
		IMG_ERR("Cannot read PNG header");
		goto cleanup;
	}

	if(png_sig_cmp(pngsig, 0, PNG_SIG_SIZE) != 0)
	{
		IMG_ERR("File not PNG image");
		goto cleanup;
	}

	/*
	 * Crete some PNG structs
	 */
	pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngPtr)
	{
		IMG_ERR("png_create_read_struct failed");
		goto cleanup;
	}

	infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr)
	{
		IMG_ERR("png_create_info_struct failed");
		goto cleanup;
	}

	/*
	 * Set error handling
	 */
	if(setjmp(png_jmpbuf(pngPtr)))
	{
		IMG_ERR("Reading PNG file failed");
		goto cleanup;
	}

	/*
	 * Init io
	 */
	png_init_io(pngPtr, file);
	png_set_sig_bytes(pngPtr, PNG_SIG_SIZE); // PNG lib knows that we allready have read the header

	/*
	 * Read info and make conversions
	 * This loop reads info, if not acceptable it calls libpng funcs to change them and re-runs the loop
	 */
	png_read_info(pngPtr, infoPtr);
	while(true)
	{
		width = png_get_image_width(pngPtr, infoPtr);
		height = png_get_image_height(pngPtr, infoPtr);
		bitDepth = png_get_bit_depth(pngPtr, infoPtr);
		channels = png_get_channels(pngPtr, infoPtr);
		colorType = png_get_color_type(pngPtr, infoPtr);

		// 1) Convert the color types
		switch(colorType)
		{
			case PNG_COLOR_TYPE_PALETTE:
				IMG_WARNING("Converting PNG_COLOR_TYPE_PALETTE to PNG_COLOR_TYPE_RGB or PNG_COLOR_TYPE_RGBA");
				png_set_palette_to_rgb(pngPtr);
				goto again;
				break;
			case PNG_COLOR_TYPE_GRAY:
				// do nothing
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				IMG_WARNING("Cannot accept PNG_COLOR_TYPE_GRAY_ALPHA. Converting to PNG_COLOR_TYPE_GRAY");
				png_set_strip_alpha(pngPtr);
				goto again;
				break;
			case PNG_COLOR_TYPE_RGB:
				// do nothing
				break;
			case PNG_COLOR_TYPE_RGBA:
				// do nothing
				break;
			default:
				//FATAL("Forgot to handle a color type");
				break;
		}

		// 2) Convert transparenct. Dont know if it needs that
		/*if(png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(pngPtr);
			goto again;
		}*/

		// 3) Convert the bit depths
		if(colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
		{
			IMG_WARNING("Converting bit depth");
			png_set_gray_1_2_4_to_8(pngPtr);
			goto again;
		}

		if(bitDepth > 8)
		{
			IMG_WARNING("Converting bit depth");
			png_set_strip_16(pngPtr);
		}

		// Sanity checks
		if((bitDepth != 8) ||
		   (colorType != PNG_COLOR_TYPE_GRAY && colorType != PNG_COLOR_TYPE_RGB && colorType != PNG_COLOR_TYPE_RGBA))
		{
			IMG_ERR("Sanity checks failed");
			goto cleanup;
		}

		break;

		again:
			png_read_update_info(pngPtr, infoPtr);
	}

	/*
	 * Read this sucker
	 */
	rowbytes = png_get_rowbytes(pngPtr, infoPtr);

	rowPointers.resize(height * sizeof(png_bytep));

	data.resize(rowbytes * height);

	for (uint i = 0; i < height; ++i)
		rowPointers[height - 1 - i] = &data[i * rowbytes];

	png_read_image(pngPtr, &rowPointers[0]);

	ok = true;

	/*
	 * Cleanup
	 */
	cleanup:

	if(pngPtr)
	{
		if(infoPtr)
			png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
		else
			png_destroy_read_struct(&pngPtr, NULL, NULL);
	}

	if(file)
		fclose(file);

	return ok;
}


/**
 * Init opengl
 */
void initGl()
{
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}


/**
 * Load the PNG into an image buffer.
 * Set up OpenGL texture using image buffer.
 */
void init(void)
{
	uint width, height, colorType;
	vector<uchar> imageData;
	const char* filename = "test.png";

	// a unit test
	MALLINFO_BEGIN
	{
		uint width, height, colorType;
		vector<uchar> imageData;
		loadPng(filename, width, height, colorType, imageData);
	}
	MALLINFO_END

	if(!loadPng(filename, width, height, colorType, imageData))
	{
		cerr << "See prev error" << endl;
		return;
	}

	GLint internalFormat;
	GLint format;
	GLint type;
	switch(colorType)
	{
		case PNG_COLOR_TYPE_GRAY:
			internalFormat = GL_RED;
			format = GL_RED;
			type = GL_UNSIGNED_BYTE;
			break;

		case PNG_COLOR_TYPE_RGB:
			internalFormat = GL_RGB;
			format = GL_RGB;
			type = GL_UNSIGNED_BYTE;
			break;

		case PNG_COLOR_TYPE_RGBA:
			internalFormat = GL_RGBA;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
	}

	glGenTextures(1, &texGlId);
	glBindTexture(GL_TEXTURE_2D, texGlId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, &imageData[0]);
}


/**
 * Callback function registered to glutDisplayFunc
 */
void display(void)
{
	if(!windowHeight)
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -20); /* eye position */

	glEnable(GL_TEXTURE_2D); /* enable texture mapping */
	glBindTexture(GL_TEXTURE_2D, texGlId); /* bind to our texture*/

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-10.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(10.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-10.0f, 10.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D); /* disable texture mapping */
	glutSwapBuffers();
}


/**
 * Callback function registered to glutReshapeFunc
 */
void resize(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(55, (GLfloat) w / (GLfloat) h, 1, 9999);
	glutPostRedisplay();
}

/**
 * Callback function registered to glutKeyboardFunc
 */
void keyboard(uchar key, int /*x*/, int /*y*/)
{
	switch(key)
	{
		case 27: /* ESC key */
		case 'Q': /* Q|q keys */
		case 'q':
			exit(0);
			break;
		default:
			break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("PNG Texture Example");

	initGl();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}

