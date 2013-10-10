/**
 * @file fs.h
 * @brief File system related functions
 */

#ifndef GLF_FS_H
#define GLF_FS_H

#include "glf/common.h"
#include <stddef.h>

/**
 * @brief Read a binary file and return a buffer
 * The caller is responcible of deallocating the buffer
 * @param[in] filename The file to open
 * @param[out] size The size of the buffer. Its optional
 * @return NULL on failure
 */
extern char *glf_read_file(const char *filename, size_t *size);

/**
 * @brief Read a text file and return a C string
 * The caller is responcible of deallocating the buffer
 * @param[in] filename The file to open
 * @return NULL on failure
 */
extern char *glf_read_text_file(const char *filename);


/**
 * @brief Read a path and return the extension
 * No deallocations are needed
 * @param[in] filename The file to open
 * @return NULL on failure and if the dot is the last character
 */
extern const char *glf_get_file_extension(const char *filename);

#endif
