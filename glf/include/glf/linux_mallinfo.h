#ifndef GLF_LINUX_MALLINFO_H
#define GLF_LINUX_MALLINFO_H

#include "glf/log.h"
#include <malloc.h>
#include <mcheck.h>

void glf_mallinfo_print_diff(const struct mallinfo *prev,
                             const struct mallinfo *now)
{
	struct mallinfo diff;
	diff.uordblks = now->uordblks - prev->uordblks;
	diff.fordblks = now->fordblks - prev->fordblks;
	diff.arena = now->arena - prev->arena;
	
	GLF_LOGI("In use: %d, free: %d, total: %d", 
	         diff.uordblks, diff.fordblks, diff.arena);
}

#endif

