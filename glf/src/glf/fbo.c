#include "glf/fbo.h"
#include "glf/log.h"
#include "glf/gl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const GLenum color_attachments[] = {
	GL_COLOR_ATTACHMENT0
#ifdef GLF_GL_VERSION_GL3
	, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7
#endif
};

struct glf_fbo *glf_fbo_create()
{
	struct glf_fbo *fbo;

	fbo = (struct glf_fbo*)malloc(sizeof(struct glf_fbo));
	if (fbo == NULL) {
		GLF_LOGE("malloc() failed");
		goto exit;
	}

	memset(fbo, 0, sizeof(struct glf_fbo));

	glGenFramebuffers(1, &fbo->id);
	if (fbo->id == 0) {
		GLF_LOGE("glGenFramebuffers() failed");
		goto clean;
	}

	return fbo;

clean:
	free(fbo);
exit:
	return NULL;
}

void glf_fbo_destroy(struct glf_fbo *fbo)
{
	assert(fbo);
	assert(fbo->id != 0);
	glDeleteFramebuffers(1, &fbo->id);
	free(fbo);
}

void glf_fbo_bind(const struct glf_fbo *fbo, GLenum target)
{
	assert(fbo);
	assert(fbo->id != 0);
	glBindFramebuffer(target, fbo->id);
}

int glf_fbo_check_current_completeness(GLenum target)
{
	GLenum status;

	status = glCheckFramebufferStatus(target);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		GLF_LOGE("FBO is incomplete");
		return 1;
	}
	return 0;
}
