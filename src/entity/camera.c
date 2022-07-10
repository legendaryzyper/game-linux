#include "camera.h"
#include "../gfx/window.h"

void camera_init(Camera* self, f32 fov) {
	memset(self, 0, sizeof(Camera));
	self->fov = fov;
	self->aspect = (f32)window.size.x / window.size.y;
	self->znear = 0.1f;
	self->zfar = 100.0f;
	self->yaw = 0.0f;
	self->pitch = 0.0f;
}

void camera_update(Camera* self) {
	self->pitch = clamp(self->pitch, -PI_2 + 0.01f, PI_2 - 0.01f);
	self->yaw = self->yaw < -PI ? PI : self->yaw > PI ? -PI : self->yaw;

	self->direction = (vec3s) {{
		cosf(self->pitch) * cosf(self->yaw),
		sinf(self->pitch),
		cosf(self->pitch) * sinf(self->yaw)
	}};
	glms_vec3_normalize(self->direction);

	self->right = glms_vec3_cross(self->direction, (vec3s) {{ 0.0f, 1.0f, 0.0f }}); 
	self->up = glms_vec3_cross(self->right, self->direction);
	self->view = glms_lookat(self->position, glms_vec3_add(self->position, self->direction), self->up);
	self->proj = glms_perspective(self->fov, self->aspect, self->znear, self->zfar);
}
