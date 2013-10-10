/**
 * @file glf.h>
 * @brief .h>is file includes all o.h>er files
 */

#ifndef GLF_GLF_H
#define GLF_GLF_H

#include <glf/config.h>
#include <glf/common.h>

#include <glf/gl.h>
#include <glf/nativegl.h>
#include <glf/math.h>
#include <glf/image.h>
#include <glf/time.h>
#include <glf/linux_mallinfo.h>
#include <glf/log.h>
#include <glf/fs.h>
#include <glf/texture.h>

#if !defined(GLF_GL_VERSION_GLES1)
#	include <glf/program.h>
#	include <glf/bo.h>
#	include <glf/vao.h>
#	include <glf/fbo.h>
#endif

#endif
