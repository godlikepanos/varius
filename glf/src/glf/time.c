#include "glf/time.h"

#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#if HAVE_NANOSLEEP || HAVE_CLOCK_GETTIME
#	include <time.h>
#endif

/* The first ticks value of the application */
#ifdef HAVE_CLOCK_GETTIME
static struct timespec start;
#else
static struct timeval start;
#endif

void glf_time_init()
{
#if HAVE_CLOCK_GETTIME
	clock_gettime(CLOCK_MONOTONIC, &start);
#else
	gettimeofday(&start, NULL);
#endif
}

uint32_t glf_get_ticks()
{
	uint32_t ticks;

#if HAVE_CLOCK_GETTIME
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	ticks = (now.tv_sec - start.tv_sec) * 1000 
	        + (now.tv_nsec - start.tv_nsec) / 1000000;
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	ticks = (now.tv_sec - start.tv_sec) * 1000 
	        + (now.tv_usec - start.tv_usec) / 1000;
#endif

	return ticks;
}

void glf_delay(uint32_t ms)
{
	int was_error;

#if HAVE_NANOSLEEP
	struct timespec elapsed, tv;
#else
	struct timeval tv;
	uint32_t then, now, elapsed;
#endif

	/* Set the timeout interval */
#if HAVE_NANOSLEEP
	elapsed.tv_sec = ms / 1000;
	elapsed.tv_nsec = (ms % 1000) * 1000000;
#else
	then = glf_get_ticks();
#endif
	do 
	{
		errno = 0;

#if HAVE_NANOSLEEP
		tv.tv_sec = elapsed.tv_sec;
		tv.tv_nsec = elapsed.tv_nsec;
		was_error = nanosleep(&tv, &elapsed);
#else
		/* Calculate the time interval left (in case of interrupt) */
		now = glf_get_ticks();
		elapsed = now - then;
		then = now;
		if(elapsed >= ms)
		{
			break;
		}
		ms -= elapsed;
		tv.tv_sec = ms / 1000;
		tv.tv_usec = (ms % 1000) * 1000;

		was_error = select(0, NULL, NULL, NULL, &tv);
#endif /* HAVE_NANOSLEEP */
	} while(was_error && (errno == EINTR));
}
