#ifndef GLF_LOG_H
#define GLF_LOG_H

#include "glf/common.h"

/**
 * @brief Logger message type
 */
enum glf_log_message_type
{
	GLF_LMT_INFO,
	GLF_LMT_ERROR,
	GLF_LMT_WARNING
};

/**
 * @brief Internal function, use the macros instead
 */
extern void glf_log_printf(const char *file, int line, const char *func, 
                           int t, const char *fmt, ...);

/**
 * @brief Log info
 */
#define GLF_LOGI(...) \
	glf_log_printf( __FILE__, __LINE__, __FUNCTION__, GLF_LMT_INFO, \
	               __VA_ARGS__)

/**
 * @brief Log error
 */
#define GLF_LOGE(...) \
	glf_log_printf( __FILE__, __LINE__, __FUNCTION__, GLF_LMT_ERROR, \
	               __VA_ARGS__)

/**
 * @brief Log warning
 */
#define GLF_LOGW(...) \
	glf_log_printf( __FILE__, __LINE__, __FUNCTION__, GLF_LMT_WARNING, \
	               __VA_ARGS__)

#endif
