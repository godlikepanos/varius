#include "glf/image.h"
#include "glf/log.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t tga_header_uncompr[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t tga_header_compr[12] = {0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * @brief The name is self explanatory
 */
static int glf_load_uncompressed_tga(FILE *fs, glf_image *img)
{
	uint8_t header6[6];
	size_t read_count;
	int bytes_per_pixel;
	size_t img_size;
	size_t i;

	assert(fs && img);

	/* read the info from header */
	read_count = fread(header6, 1, sizeof(header6), fs);
	if(read_count != sizeof(header6))
	{
		GLF_LOGE("Cannot read info header");
		goto error;
	}

	img->width  = header6[1] * 256 + header6[0];
	img->height = header6[3] * 256 + header6[2];
	img->bpp = header6[4];

	if((img->width <= 0) || (img->height <= 0) 
	   || ((img->bpp != 24) && (img->bpp != 32)))
	{
		GLF_LOGE("Invalid image information");
		goto error;
	}

	/* read the data */
	bytes_per_pixel = (img->bpp / 8);
	img_size = bytes_per_pixel * img->width * img->height;
	img->data = malloc(img_size);
	if(!img->data)
	{
		GLF_LOGE("malloc() failed");
		goto error;
	}

	read_count = fread(img->data, 1, img_size, fs);
	if(read_count != img_size)
	{
		GLF_LOGE("Cannot read image data");
		goto cleanup;
	}

	/* swap red with blue */
	for(i = 0; i < img_size; i += bytes_per_pixel)
	{
		uint32_t temp = img->data[i];
		img->data[i] = img->data[i + 2];
		img->data[i + 2] = temp;
	}

	return 0;

cleanup:
	free(img->data);
error:
	return 1;
}

static int glf_load_compressed_tga(FILE *fs, glf_image *img)
{
	assert(fs && img);

	(void)fs;
	(void)img;
	return 0;
}

glf_image *glf_image_load(const char *filename)
{
	glf_image *img;
	FILE *fs;
	size_t bytes_read;
	char tga_header[12];

	assert(filename);

	/* Alloc */
	img = (glf_image*)malloc(sizeof(glf_image));
	if(!img)
	{
		GLF_LOGE("malloc() failed");
		goto error;
	}
	memset(img, 0, sizeof(glf_image));

	/* Read */
	fs = fopen(filename, "rb");
	if(!fs)
	{
		GLF_LOGE("Cannot read file: %s", filename);
		goto clean_img;
	}

	bytes_read = fread(tga_header, 1, sizeof(tga_header), fs);
	if(bytes_read != sizeof(tga_header))
	{
		GLF_LOGE("Cannot read file: %s", filename);
		goto clean_file;
	}

	/* Chose the correct loading func */
	if(memcmp(tga_header_uncompr, tga_header, sizeof(tga_header)) == 0)
	{
		if(glf_load_uncompressed_tga(fs, img))
		{
			goto clean_file;
		}
	}
	else if(memcmp(tga_header_compr, tga_header, sizeof(tga_header)) == 0)
	{
		if(glf_load_compressed_tga(fs, img))
		{
			goto clean_file;
		}
	}
	else
	{
		GLF_LOGE("Invalid TGA header");
		goto clean_file;
	}

	/* Return */
	fclose(fs);
	return img;

clean_file:
	fclose(fs);
clean_img:
	free(img);
error:
	return NULL;
}

void glf_image_destroy(glf_image *img)
{
	assert(img);
	assert(img->data);

	free(img->data);
	free(img);
}
