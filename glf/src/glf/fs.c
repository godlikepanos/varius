#include "glf/fs.h"
#include "glf/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

char *glf_read_file(const char *filename, size_t *size)
{
	char *buff;
	FILE *fh = fopen(filename, "rb");
	size_t size1;

	assert(filename);

	if(!fh)
	{
		GLF_LOGE("Cannot open file: %s", filename);
		goto exit;
	}

	fseek(fh, 0, SEEK_END);
	size1 = ftell(fh);

	if(size1 < 1)
	{
		GLF_LOGE("File is empty: %s", filename);
		goto clean_fclose;
	}

	rewind(fh);
	buff = malloc(size1);

	if(!buff)
	{
		GLF_LOGE("malloc() failed");
		goto clean_fclose;
	}

	fread(buff, size1, 1, fh);
	fclose(fh);

	if(size)
	{
		*size = size1;
	}

	return buff;

clean_fclose:
	fclose(fh);
exit:
	return NULL;
}

char *glf_read_text_file(const char *filename)
{
	char *buff;
	FILE *fh = fopen(filename, "rb");
	size_t size1;

	assert(filename);

	if(!fh)
	{
		GLF_LOGE("Cannot open file: %s", filename);
		goto exit;
	}

	fseek(fh, 0, SEEK_END);
	size1 = ftell(fh);

	if(size1 < 1)
	{
		GLF_LOGE("File is empty: %s", filename);
		goto clean_fclose;
	}

	rewind(fh);
	buff = malloc(size1 + 1);

	if(!buff)
	{
		GLF_LOGE("malloc() failed");
		goto clean_fclose;
	}

	fread(buff, size1, 1, fh);
	fclose(fh);
	
	buff[size1] = '\0';

	return buff;

clean_fclose:
	fclose(fh);
exit:
	return NULL;
}

const char *glf_get_file_extension(const char *filename)
{
	const char *pc;

	assert(filename);
	pc = strrchr(filename, '.');

	if(pc == NULL)
	{
		return NULL;
	}

	++pc;
	if(*pc == '\0')
	{
		return NULL;
	}

	return pc;
}
