#include "glf/math.h"
#include <stdio.h>
#include <assert.h>

#define AT(m, _i, _j) m[(_i) * 4 + (_j)]

static float identity[16] = {1.0, 0.0, 0.0, 0.0,
			     0.0, 1.0, 0.0, 0.0,
			     0.0, 0.0, 1.0, 0.0,
			     0.0, 0.0, 0.0, 1.0};

void glf_mat_set_identity(float *m)
{
	int i = 0;
	for (i = 0; i < 16; i++) {
		m[i] = identity[i];
	}
}

void glf_mat_set_projection(float fovx, float fovy, float znear, float zfar,
			    float *m)
{
	float f = 1.0 / tan(fovy * 0.5);

	AT(m, 0, 0) = f * fovy / fovx;
	AT(m, 0, 1) = 0.0;
	AT(m, 0, 2) = 0.0;
	AT(m, 0, 3) = 0.0;
	AT(m, 1, 0) = 0.0;
	AT(m, 1, 1) = f;
	AT(m, 1, 2) = 0.0;
	AT(m, 1, 3) = 0.0;
	AT(m, 2, 0) = 0.0;
	AT(m, 2, 1) = 0.0;
	AT(m, 2, 2) = (zfar + znear) / (znear - zfar);
	AT(m, 2, 3) = (2.0 * zfar * znear) / (znear - zfar);
	AT(m, 3, 0) = 0.0;
	AT(m, 3, 1) = 0.0;
	AT(m, 3, 2) = -1.0;
	AT(m, 3, 3) = 0.0;
}

void glf_mat_mul(const float *a, const float *b, float *o)
{
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			AT(o, i, j) = AT(a, i, 0) * AT(b, 0, j) 
				    + AT(a, i, 1) * AT(b, 1, j)
				    + AT(a, i, 2) * AT(b, 2, j) 
				    + AT(a, i, 3) * AT(b, 3, j);
		}
	}
}

void glf_mat_set_translation(const float v[4], float m[16])
{
	AT(m, 0, 3) = v[0];
	AT(m, 1, 3) = v[1];
	AT(m, 2, 3) = v[2];
	AT(m, 3, 3) = v[3];
}

void glf_mat_set_rotation(float x, float y, float z, float *m)
{
	float ch, sh, ca, sa, cb, sb;
	sh = sin(y);
	ch = cos(y);
	sa = sin(z);
	ca = cos(z);
	sb = sin(x);
	cb = cos(x);

	AT(m, 0, 0) = ch * ca;
	AT(m, 0, 1) = sh * sb - ch * sa * cb;
	AT(m, 0, 2) = ch * sa * sb + sh * cb;
	AT(m, 1, 0) = sa;
	AT(m, 1, 1) = ca * cb;
	AT(m, 1, 2) = -ca * sb;
	AT(m, 2, 0) = -sh * ca;
	AT(m, 2, 1) = sh * sa * cb + ch * sb;
	AT(m, 2, 2) = -sh * sa * sb + ch * cb;
}

void glf_mat_invert(const float *in, float *out)
{
	assert(0 && "Unimplemented");
	assert(in != out);
	(void)in;
	(void)out;
}

void glf_mat_transpose(const float *in, float *out)
{
	int i, j;

	assert(in != out);
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			AT(out, i, j) = AT(in, j, i);
		}
	}
}

void glf_mat_print(const float *m)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("%f ", AT(m, i, j));
		}
		printf("\n");
	}
}
