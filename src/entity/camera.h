#ifndef CAMERA_H
#define CAMERA_H

#include "../gfx/gfx.h"
#include "../util/util.h"

typedef struct Camera {
	mat4s view, proj;
	vec3s position, direction, right, up;
	f32 pitch, yaw, fov, aspect, znear, zfar;
} Camera;

void camera_init(Camera* self, f32 fov);
void camera_update(Camera* self);

#endif
