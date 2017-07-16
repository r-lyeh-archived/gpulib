#include "../../gpulib.h"

typedef struct { float x, y, z; } v3;

int main() {
  SDL_Window * sdl_window = NULL;
  GpuWindow("Hello Triangle", 1280, 720, 4, 0, 0, &sdl_window, NULL);
  GpuSetDebugCallback(GpuDebugCallback, NULL);

  v3 * mesh = GpuMalloc(3 * sizeof(v3));

  mesh[0] = (v3){ 0.0,  0.5, 0.0};
  mesh[1] = (v3){-0.5, -0.5, 0.0};
  mesh[2] = (v3){ 0.5, -0.5, 0.0};

  unsigned mesh_tex = GpuCast(mesh, gpu_xyz_f32_t, 0, 3 * sizeof(v3));

  unsigned textures[16] = {0};
  textures[0] = mesh_tex;

  unsigned vert = GpuVert(
      GPU_VERT_HEAD " layout(binding = 0) uniform samplerBuffer s_pos; \n"
                    "                                                  \n"
                    " void main() {                                    \n"
                    "   vec3 pos = texelFetch(s_pos, gl_VertexID).xyz; \n"
                    "   gl_Position = vec4(pos, 1);                    \n"
                    " }                                                \n");

  unsigned frag = GpuFrag(
      GPU_FRAG_HEAD " layout(location = 0) out vec4 color; \n"
                    "                                      \n"
                    " void main() {                        \n"
                    "   color = vec4(1);                   \n"
                    " }                                    \n");

  unsigned ppo = GpuPpo(vert, frag);

  struct gpu_cmd_t cmd[1] = {0};
  cmd[0].count = 3;
  cmd[0].instance_count = 1;

  struct gpu_op_t op[1] = {0};
  op[0].tex_count = 16;
  op[0].tex = textures;
  op[0].ppo = ppo;
  op[0].mode = gpu_triangles_t;
  op[0].cmd_count = 1;
  op[0].cmd = cmd;

  for (;;) {
    GpuClear();
    GpuDraw(1, op);
    GpuSwap(sdl_window);
    
    for (SDL_Event e; SDL_PollEvent(&e);) {
      if (e.type == SDL_QUIT)
        goto exit;
    }
  }

exit:
  return 0;
}