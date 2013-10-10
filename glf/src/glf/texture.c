#include "glf/texture.h"
#include "glf/image.h"
#include "glf/log.h"
#include <stdlib.h>
#include <string.h>

struct glf_texture *
glf_texture_create_2d(const struct glf_texture_initializer *init)
{
	struct glf_texture *tex;
	GLenum target = GL_TEXTURE_2D;

	assert(init);

	tex = (struct glf_texture*)malloc(sizeof(struct glf_texture));
	if (tex == NULL) {
		GLF_LOGE("malloc() failed");
		goto exit;
	}

	memset(tex, 0, sizeof(struct glf_texture));

	glGenTextures(1, &tex->id);
	if (tex->id == 0) {
		GLF_LOGE("glGetTextures() failed");
		goto cleanup_free;
	}

	glBindTexture(target, tex->id);

	glTexImage2D(target, 0, init->internal_format, init->width, 
		     init->height, 0, init->format, init->type, init->data);

	if (init->repeat) {
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else {
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	if (init->mipmapping && init->data) {
		glGenerateMipmap(target);
	}

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return tex;

cleanup_free:
	free(tex);
exit:
	return NULL;
}
