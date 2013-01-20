#ifndef __GL_DEFINES__
#define __GL_DEFINES__

// Some <math.h> files do not define M_PI...
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define GLfloat  float
#define GLenum   int
#define GLdouble float
#define GLint    int

void glVertex3fv(float* v) {
	glVertex3v16(floattov16(v[0]), floattov16(v[1]), floattov16(v[2]));
}

void glVertex3v16v(v16* v) {
	glVertex3v16(v[0], v[1], v[2]);
}

#endif
