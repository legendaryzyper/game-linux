#include "window.h"

Window window;

static void _size_callback(GLFWwindow* handle, int width, int height) {
	glViewport(0, 0, width, height);
	window.size = (ivec2s) {{ width, height }};
}

static void _key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
	if (key < 0) {
		return;
	}

	switch (action) {
		case GLFW_PRESS:
			window.keyboard.keys[key].down = true;
			break;
		case GLFW_RELEASE:
			window.keyboard.keys[key].down = false;
			break;
		default:
			break;
	}
}

static void _cursor_callback(GLFWwindow* handle, double xp, double yp) {
	vec2s p = {{ xp, yp }};

    window.mouse.delta = window.mouse.not_first ? 
        glms_vec2_sub(p, window.mouse.position) : GLMS_VEC2_ZERO, window.mouse.not_first = 1;
	window.mouse.delta.x = clamp(window.mouse.delta.x, -100.0f, 100.0f);
	window.mouse.delta.y = clamp(window.mouse.delta.y, -100.0f, 100.0f);

	window.mouse.position = p;
}

void window_create(FWindow init, FWindow tick, FWindow update, FWindow render, FWindow destroy) {
	window.init = init;
	window.tick = tick;
	window.update = update;
	window.render = render;
	window.destroy = destroy;

	window.last_frame = NOW();
	window.last_second = NOW();

	window.size = (ivec2s) {{ 1200, 720 }};
	window.tickrate = 128;
	window.mouse.sensitivity = 3.0f;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window.handle = glfwCreateWindow(window.size.x, window.size.y, "game", NULL, NULL);
	glfwMakeContextCurrent(window.handle);
	glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window.handle, _size_callback);
	glfwSetCursorPosCallback(window.handle, _cursor_callback);
	glfwSetKeyCallback(window.handle, _key_callback);

	gladLoadGL();
}

static void _button_array_tick(u32 n, Button* buttons) {
	for (u32 i = 0; i < n; i++) {
		buttons[i].pressed_tick = buttons[i].down && !buttons[i].last_tick;
		buttons[i].last_tick = buttons[i].down;
	}
}

static void _init(void) {
	window.init();
}

static void _tick(void) {
	window.ticks++;
	_button_array_tick(GLFW_KEY_LAST, window.keyboard.keys);
	window.tick();
}

static void _update(void) {
	window.update();
	window.mouse.delta = GLMS_VEC2_ZERO;
}

static void _render(void) {
	window.frames++;
	window.render();
}

static void _destroy(void) {
	window.destroy();
    glfwDestroyWindow(window.handle);
	glfwTerminate();
}

void window_loop(void) {
	_init();

	while (!glfwWindowShouldClose(window.handle)) {
		const u64 now = NOW();

		window.frame_delta = now - window.last_frame;
		window.last_frame = now;

		if (now - window.last_second > NS_PER_SECOND) {
			window.fps = window.frames;
			window.tps = window.ticks;
			window.frames = 0;
			window.ticks = 0;
			window.last_second = now;

			printf("FPS: %" PRIu64 " | TPS: %" PRIu64 "\n", window.fps, window.tps);
		}

		const u64 NS_PER_TICK = (NS_PER_SECOND / window.tickrate);
		u64 tick_time = window.frame_delta + window.tick_remainder;
		while (tick_time > NS_PER_TICK) {
			_tick();
			tick_time -= NS_PER_TICK;
		}
		window.tick_remainder = max(tick_time, 0);
		
		_update();
		_render();
		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	_destroy();
}
