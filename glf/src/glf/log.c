#include "glf/log.h"
#include <stdarg.h>
#include <stdio.h>

void glf_log_printf(const char *file, int line, const char *func, 
                    int t, const char *fmt, ...)
{
	va_list args;
	FILE *file_descr;
	const char *s;

	switch(t)
	{
	case GLF_LMT_INFO:
		file_descr = stdout;
		s = "I";
		break;
	case GLF_LMT_ERROR:
		file_descr = stderr;
		s = "E";
		break;
	case GLF_LMT_WARNING:
		file_descr = stderr;
		s = "W";
		break;
	}

	if(t == GLF_LMT_ERROR || t == GLF_LMT_WARNING)
	{
		fprintf(file_descr, "\033[1;31;6m");
	}

	fprintf(file_descr, "libglf (%s:%d %s) [%s]: ", file, line, func, s);
	va_start(args, fmt);
	vfprintf(file_descr, fmt, args);
	va_end( args );
	fprintf(file_descr, "\n");

	if(t == GLF_LMT_ERROR || t == GLF_LMT_WARNING)
	{
		fprintf(file_descr, "\033[0;;m");
	}
}
