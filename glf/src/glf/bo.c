#include "glf/bo.h"
#include "glf/log.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct glf_bo *glf_bo_create(const struct glf_bo_initializer *init)
{
	struct glf_bo *bo;
	int buffer_size = 0;

	assert(init);
	assert(init->size > 0);

	/* alloc */
	bo = (struct glf_bo*)malloc(sizeof(glf_bo));
	if (!bo) {
		GLF_LOGE("malloc() failed");
		goto exit;
	}
	memset(bo, 0, sizeof(glf_bo));

	/* GL */
	bo->usage = init->usage;
	bo->target = init->target;
	bo->size = init->size;

	glGenBuffers(1, &bo->id);
	if (bo->id == 0) {
		GLF_LOGE("glGenBuffers() failed");
		goto clean;
	}
	glf_bo_bind(bo);
	glBufferData(bo->target, bo->size, init->data, bo->usage);

	/* make a check */
	glGetBufferParameteriv(bo->target, GL_BUFFER_SIZE, &buffer_size);
	if (bo->size != (uint32_t)buffer_size) {
		GLF_LOGE("Data size mismatch");
		goto clean_bo;
	}

	GLF_CHECK_GL_ERROR();
	return bo;

clean_bo:
	glDeleteBuffers(1, &bo->id);
clean:
	free(bo);
exit:
	return NULL;
}

void glf_bo_destroy(struct glf_bo *bo)
{
	assert(bo);
	assert(bo->id != 0);
	glDeleteBuffers(1, &bo->id);
	free(bo);
}

void glf_bo_bind(struct glf_bo *bo)
{
	assert(bo);
	glBindBuffer(bo->target, bo->id);
}
