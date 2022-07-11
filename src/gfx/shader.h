#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"
#include "../util/util.h"

#include "texture.h"
#include "../entity/camera.h"

typedef struct VertexAttrib {
	GLuint index;
	const GLchar* name;
} VertexAttrib;

typedef struct Shader {
	GLuint handle, vs_handle, fs_handle;
} Shader;

void shader_init(Shader* self, const char* vs_path, const char* fs_path);
void shader_destroy(Shader* self);
void shader_bind(Shader* self);
void shader_uniform_texture2D(Shader* self, const char* name, Texture* texture, GLuint n);
void shader_uniform_mat4(Shader* self, const char* name, mat4s m);
void shader_uniform_camera(Shader* self, Camera* camera);

#endif
