#include <glf/glf.h>

static struct glf_vao *create_quad_vao()
{
	float positions[][3] = {{1.0, 1.0, 0.0}, {-1.0, 1.0, 0.0}, 
		{-1.0, -1.0, 0.0}, {1.0, -1.0, 0.0}};
	uint16_t positions_ids[] = {0, 1, 3, 1, 2, 3};

	struct glf_bo *pos_bo, *ids_bo;
	struct glf_vao *vao;
	glf_bo_initializer boinit;
	const GLint POSITION_LOC = 0;

	boinit.target = GL_ARRAY_BUFFER;
	boinit.size = sizeof(positions);
	boinit.data = positions;
	boinit.usage = GL_STATIC_DRAW;
	pos_bo = glf_bo_create(&boinit);
	assert(pos_bo);

	boinit.target = GL_ELEMENT_ARRAY_BUFFER;
	boinit.size = sizeof(positions_ids);
	boinit.data = positions_ids;
	boinit.usage = GL_STATIC_DRAW;
	ids_bo = glf_bo_create(&boinit);
	assert(ids_bo);

	vao = glf_vao_create();
	assert(vao);
	glf_vao_attach_array_bo(vao, pos_bo, POSITION_LOC, 3, GL_FLOAT, 
							GL_FALSE, 0, NULL);
	glf_vao_attach_element_bo(vao, ids_bo);

	return vao;
}

const char *vert_src = 
	"#version 330 core\n \
	in vec3 position;\n \
	out vec4 vcolor;\n \
	void main() {\n \
		gl_Position = vec4(position, 1.0);\n \
		vcolor = vec4(position, 1.0);\n \
	}";

int main(int argc, char** argv)
{
	struct glf_program *prog;
	struct glf_vao *vao;
	struct glf_window *window;
	glf_window_initializer init;
	glf_texture *tex;

	(void)argc;
	(void)argv;

	init.bits_rgba[0] = init.bits_rgba[1] = init.bits_rgba[2] = 8;
	init.bits_rgba[3] = 4;
	init.samples_num = 0;
	init.depth_size = 24;
	init.stencil_size = 8;
	init.width = 16;
	init.height = 16;
	init.title = "example";

	window = glf_window_create(&init);

	prog = glf_program_create_vf(
			vert_src, glf_read_text_file("shaders/varying_color.frag.glsl"));


	vao = create_quad_vao();

	return 0;
}

