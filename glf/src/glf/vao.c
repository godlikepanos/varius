#include "glf/vao.h"
#include "glf/bo.h"
#include "glf/program.h"
#include "glf/log.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

glf_vao *glf_vao_create()
{
	glf_vao *vao;

	/* alloc */
	vao = (glf_vao*)malloc(sizeof(glf_vao));
	if(!vao)
	{
		GLF_LOGE("malloc() failed");
		goto exit;
	}
	memset(vao, 0, sizeof(glf_vao));

	/* GL */
	glGenVertexArrays(1, &vao->id);
	if(vao->id == 0)
	{
		GLF_LOGE("glGenVertexArrays() failed");
		goto clean_free;
	}

	return vao;

clean_free:
	free(vao);
exit:
	return NULL;
}

void glf_vao_destroy(glf_vao *vao)
{
	assert(vao && vao->id != 0);
	glDeleteVertexArrays(1, &vao->id);
	free(vao);
	vao = NULL;
}

void glf_vao_bind(const glf_vao *vao)
{
	assert(vao && vao->id != 0);
	glBindVertexArray(vao->id);
}

void glf_vao_attach_element_bo(glf_vao *vao, glf_bo *bo)
{
	assert(vao && bo && vao->id != 0 
			&& bo->target == GL_ELEMENT_ARRAY_BUFFER);

	glf_vao_bind(vao);
	glf_bo_bind(bo);
	glBindVertexArray(0);
}

void glf_vao_attach_array_bo(glf_vao *vao, 
                             glf_bo *bo,
                             GLuint attib_loc,
                             GLint size,
                             GLenum type,
                             GLboolean normalized,
                             GLsizei stride,
                             const GLvoid *pointer)
{
	assert(vao && bo && bo->target == GL_ARRAY_BUFFER);

	glf_vao_bind(vao);
	glf_bo_bind(bo);

	glVertexAttribPointer(attib_loc, size, type, normalized,
		stride, pointer);
	glEnableVertexAttribArray(attib_loc);

	glBindBuffer(bo->target, 0);
	glBindVertexArray(0);
}
