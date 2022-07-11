#ifndef MESH_H
#define MESH_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "../gfx/bo.h"
#include "../gfx/vao.h"

typedef struct MeshBuffer {
	void* data;
	u64 count, index;
} MeshBuffer;

typedef struct Mesh {
	MeshBuffer data, indices;
	u64 vertex_count;

	VAO vao;
	BO vbo, ebo;
} Mesh;

void mesh_init(Mesh* self);
void mesh_prepare(Mesh* self);
void mesh_emit_face(Mesh* mesh, vec3s position, Direction d);
void mesh_finalize(Mesh* self);
void mesh_render(Mesh* self);
void mesh_destroy(Mesh* self);

#endif
