#include "glf/program.h"
#include "glf/log.h"
#include "glf/fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static const char* padding = "======================================="
                             "=======================================";

/**
 * @brief XXX
 */
#if 0
static int glf_shader_variable_init(glf_shader_variable *var, 
                                    GLint id, const char *name, GLenum type)
{
	assert(var && id != -1 && name);

	var->id = id;
	var->type = type;
	var->name = (char*)malloc(strlen(name) + 1);

	if(var->name == NULL)
	{
		GLF_LOGE("malloc() failed");
		return 1;
	}

	strcpy(var->name, name);
	return 0;
}

/**
 * @brief XXX
 */
static void glf_shader_variable_deinit(glf_shader_variable *var)
{
	assert(var && var->name);
	free(var->name);
}

/**
 * @brief XXX
 */
static glf_shader_variable *glf_shader_variable_create(
                                        GLint id, const char *name, GLenum type)
{
	glf_shader_variable *var;

	/* alloc */
	var = (glf_shader_variable*)malloc(sizeof(glf_shader_variable));
	if(var == NULL)
	{
		GLF_LOGE("malloc() failed");
		goto exit;
	}

	memset(var, 0, sizeof(glf_shader_variable));
	
	/* set */
	var->id = id;
	var->type = type;
	var->name = (const char*)malloc(strlen(name) + 1);

	if(var->name == NULL)
	{
		GLF_LOGE("malloc() failed");
		goto clean_var;
	}

	strcpy(var->name, name);

clean_var:
	free(var);
exit:
	return NULL;
}

/**
 * @brief XXX
 */
static glf_shader_variable_destroy(glf_shader_variable *var)
{
	assert(var);
	assert(var->name);

	free(var->name);
	free(var);
}
#endif

/**
 * @brief Create a shader given the source
 *
 * @return Zero on failure
 */
static GLuint glf_shader_create(const char *src, GLenum type)
{
	GLuint name;
	GLint success;
	const char* srcs[1];

	assert(src);

	srcs[0] = src;

	name = glCreateShader(type);
	if (name == 0) {
		GLF_LOGE("glCreateShader() failed");
		goto cleanup;
	}

	glShaderSource(name, 1, srcs, NULL);
	glCompileShader(name);
	glGetShaderiv(name, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLint info_len = 0;
		GLint chars_written = 0;
		char info_log[1024];
		char src2[1024 * 10];
		const char *cp = src;
		char *cp2 = src2;
		int line = 1;

		glGetShaderiv(name, GL_INFO_LOG_LENGTH, &info_len);
		assert((size_t)info_len < sizeof(info_log));
		glGetShaderInfoLog(name, info_len, &chars_written, 
				   &info_log[0]);
		info_log[chars_written] = '\0';

		/* Put line numbers */
		sprintf(cp2, "%04d: ", line);
		cp2 += 6;
		++ line;

		while (*cp != '\0') {
			if (*cp == '\n') {
				sprintf(cp2, "\n%04d: ", line);
				cp2 += 7;
				++ line;
			} else {
				*cp2 = *cp;
				++ cp2;
			}

			++ cp;
		}
		*cp2 = '\0';

		GLF_LOGE("Shader compile failed:\n%s\n%s\n%s\n"
			 "Source:\n%s\n%s\n%s", 
			 padding, info_log, padding, padding, src2, padding);
		goto cleanup_shader;
	}

	return name;

cleanup_shader:
	glDeleteShader(name);

cleanup:
	return 0;
}

/**
 * @brief Link a program
 */
static int glf_program_link(GLuint prog)
{
	GLint success;

	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);

	if (!success) {
		GLint info_len = 0;
		GLint chars_written = 0;
		char info_log[1024];

		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &info_len);
		assert((size_t)info_len < sizeof(info_log));
		glGetProgramInfoLog(prog, info_len, &chars_written, info_log);
		info_log[chars_written] = '\0';

		GLF_LOGE("Program link failed:\n%s\n%s\n%s", padding, info_log, 
			 padding);
		return 1;
	}

	return 0;
}

#if 0
static int glf_program_populate_vars(glf_program *prog)
{
	int num;
	int num2;
	char name_[512];
	GLsizei length;
	GLint size;
	GLenum type;
	int i;
	GLint loc;

	assert(prog);
	assert(prog->id);
	assert(prog->vars == NULL);
	assert(prog->vars_num == 0);

	glGetProgramiv(prog->id, GL_ACTIVE_ATTRIBUTES, &num);
	glGetProgramiv(prog->id, GL_ACTIVE_UNIFORMS, &num2);

	/* make the allocations */
	prog->vars_num = num + num2;
	prog->vars = (glf_shader_variable*)malloc(
	                              sizeof(glf_shader_variable) * prog->vars_num);

	assert(prog->vars);
	memset(prog->vars, 0, sizeof(glf_shader_variable) * prog->vars_num);

	/* attrib locations */
	for(i = 0; i < num; i++)
	{
		glGetActiveAttrib(prog->id, i, sizeof(name_) / sizeof(char), &length,
			&size, &type, &name_[0]);
		name_[length] = '\0';

		/* check if its FFP location */
		loc = glGetAttribLocation(prog->id, &name_[0]);
		if(loc == -1) // if -1 it means that its an FFP var
		{

			GLF_LOGE("You are using FFP vertex attributes");

		}

		ShaderProgramAttributeVariable* var =
			new ShaderProgramAttributeVariable(loc, &name_[0], type, 
			size, this);

		vars.push_back(var);
		attribs.push_back(var);
		nameToVar[var->getName().c_str()] = var;
		nameToAttribVar[var->getName().c_str()] = var;
	}

	// uni locations
	glGetProgramiv(glId, GL_ACTIVE_UNIFORMS, &num);
	for(int i = 0; i < num; i++) // loop all uniforms
	{
		glGetActiveUniform(glId, i, sizeof(name_) / sizeof(char), &length,
			&size, &type, &name_[0]);
		name_[length] = '\0';

		// check if its FFP location
		int loc = glGetUniformLocation(glId, &name_[0]);
		if(loc == -1) // if -1 it means that its an FFP var
		{
			throw ANKI_EXCEPTION("You are using FFP vertex uniforms");
		}

		ShaderProgramUniformVariable* var =
			new ShaderProgramUniformVariable(loc, &name_[0], type, 
			size, this);

		vars.push_back(var);
		unis.push_back(var);
		nameToVar[var->getName().c_str()] = var;
		nameToUniVar[var->getName().c_str()] = var;
	}
}
#endif

struct glf_program *glf_program_create(const char *vert, const char *tc,
				       const char *te, const char *geom,
				       const char *frag)
{
	struct glf_program *prog;
	int err;

	(void)tc;
	(void)te;
	(void)geom;

	prog = (struct glf_program *)malloc(sizeof(struct glf_program));
	if (prog == NULL) {
		GLF_LOGE("malloc() failed");
		goto cleanup;
	}

	memset(prog, 0, sizeof(struct glf_program));

	prog->name = glCreateProgram();
	if (prog->name == 0) {
		GLF_LOGE("glCreateProgram() failed");
		goto cleanup_free;
	}

	/*
	 * Vert
	 */
	assert(vert != NULL);
	prog->vert = glf_shader_create(vert, GL_VERTEX_SHADER);
	if (prog->vert == 0) {
		goto cleanup_prog;
	}
	glAttachShader(prog->name, prog->vert);

	/*
	 * Frag
	 */
	assert(frag != NULL);
	prog->frag = glf_shader_create(frag, GL_FRAGMENT_SHADER);
	if (prog->frag == 0) {
		goto cleanup_vert;
	}
	glAttachShader(prog->name, prog->frag);

	/*
	 * Link
	 */
	err = glf_program_link(prog->name);
	if (err) {
		goto cleanup_frag;
	}

	GLF_CHECK_GL_ERROR();
	return prog;

cleanup_frag:
	glDeleteShader(prog->frag);
cleanup_vert:
	glDeleteShader(prog->vert);
cleanup_prog:
	glDeleteProgram(prog->name);
cleanup_free:
	free(prog);
cleanup:
	return NULL;
}

struct glf_program *glf_program_create_vf(const char *vert, const char *frag)
{
	return glf_program_create(vert, NULL, NULL, NULL, frag);
}

void glf_program_use(const struct glf_program *prog)
{
	assert(prog);
	assert(prog->name != 0);
	glUseProgram(prog->name);
}

GLint glf_attribute_get_location(const struct glf_program *prog,
				 const char *name)
{
	GLint loc;

	assert(prog && name);

	loc = glGetAttribLocation(prog->name, name);
	if (loc == -1) {
		GLF_LOGW("glGetAttribLocation(\"%s\") returned -1", name);
	}
	return loc;
}

GLint glf_uniform_get_location(const struct glf_program* prog,
			       const char *name)
{
	GLint loc;

	assert(prog && name);

	loc = glGetUniformLocation(prog->name, name);
	if (loc == -1) {
		GLF_LOGW("glGetUniformLocation(\"%s\") returned -1", name);
	}
	return loc;
}

struct glf_program *glf_program_load_vf(const char *vert_filename,
					const char *frag_filename)
{
	char *vert, *frag;
	struct glf_program *prog;

	assert(vert_filename && frag_filename);

	vert = glf_read_text_file(vert_filename);
	if (!vert) {
		return NULL;
	}

	frag = glf_read_text_file(frag_filename);
	if (!frag) {
		free(vert);
		return NULL;
	}

	prog = glf_program_create_vf(vert, frag);
	free(vert);
	free(frag);
	return prog;
}
