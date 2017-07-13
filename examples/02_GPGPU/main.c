#define GPU_DEBUG_CALLBACK
#include "../../gpulib.h"

typedef struct { float x, y, z, w; } v4;

#define let __auto_type const
#define countof(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  SDL_Window * sdl_window = NULL;
  GpuWindow("GPGPU", 1280, 720, 4, 0, 0, &sdl_window, NULL);
  GpuSetDebugCallback(GpuDebugCallback, NULL);

  let dim_x = 2048;
  let dim_y = 2048;
  let dim = dim_x * dim_y;

  float * mat_1 = GpuMalloc(dim * sizeof(float));
  float * mat_2 = GpuMalloc(dim * sizeof(float));

  for (int i = 0; i < dim; i += 1) mat_1[i] = i + 1.0;
  for (int i = 0; i < dim; i += 1) mat_2[i] = i + 1.0;

  char * vert_string = GPU_VERT_HEAD
      " const vec2 quad[] = vec2[]                                       \n"
      " (                                                                \n"
      "   vec2(-1,-1), vec2( 1,-1), vec2(-1, 1),                         \n"
      "   vec2(-1, 1), vec2( 1,-1), vec2( 1, 1)                          \n"
      " );                                                               \n"
      "                                                                  \n"
      " void main() {                                                    \n"
      "   gl_Position = vec4(quad[gl_VertexID], 0, 1);                   \n"
      " }                                                                \n";

  char * frag_string = GPU_FRAG_HEAD
      " layout(binding = 0) uniform samplerBuffer s_mat_1;               \n"
      " layout(binding = 1) uniform samplerBuffer s_mat_2;               \n"
      "                                                                  \n"
      " layout(location = 0) out vec4 fbo_color;                         \n"
      "                                                                  \n"
      " void main() {                                                    \n"
      "   int x_coord = int(floor(gl_FragCoord.x));                      \n"
      "   int y_coord = int(floor(gl_FragCoord.y));                      \n"
      "                                                                  \n"
      "   float mat_1 = texelFetch(s_mat_1, y_coord * 2048 + x_coord).x; \n"
      "   float mat_2 = texelFetch(s_mat_2, y_coord * 2048 + x_coord).x; \n"
      "                                                                  \n"
      "   float mat_sum = mat_1 + mat_2;                                 \n"
      "                                                                  \n"
      "   fbo_color = vec4(mat_sum, 0, 0, 1);                            \n"
      " }                                                                \n";

  let vert = GpuVert(vert_string);
  let frag = GpuFrag(frag_string);
  let ppo = GpuPpo(vert, frag);

  let img_tex = GpuMallocImg(gpu_rgba_f32_t, dim_x, dim_y, 1, 1);
  let fbo = GpuFbo(img_tex, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  float pixel[4] = {0};
  GpuSetPix(img_tex, 0, 0, 0, dim_x, dim_y, 1, 0, gpu_rgba_t, gpu_f32_t, pixel);

  let mat_1_tex = GpuCast(mat_1, gpu_x_f32_t, 0, dim * sizeof(float));
  let mat_2_tex = GpuCast(mat_2, gpu_x_f32_t, 0, dim * sizeof(float));

  uint32_t textures[] = {
    [0] = mat_1_tex,
    [1] = mat_2_tex
  };

  struct gpu_cmd_t cmd[] = {
    [0].count = 6,
    [0].instance_count = 1
  };

  struct gpu_op_t ops[] = {
    [0].tex_count = countof(textures),
    [0].tex = textures,
    [0].ppo = ppo,
    [0].mode = gpu_triangles_t,
    [0].cmd_count = countof(cmd),
    [0].cmd = cmd
  };

  GpuClear();
  SDL_GL_SwapWindow(sdl_window);

  int win_w = 0, win_h = 0;
  SDL_GetWindowSize(sdl_window, &win_w, &win_h);

  GpuViewport(0, 0, dim_x, dim_y);
  GpuBindFbo(fbo);
  GpuDraw(countof(ops), ops);
  GpuBindFbo(0);
  GpuViewport(0, 0, win_w, win_h);

  let pixels_bytes = dim * sizeof(v4);

  void * pix = SDL_malloc((size_t)pixels_bytes);

  GpuGet(img_tex, 0, 0, 0, dim_x, dim_y, 1, 0, gpu_rgba_t, gpu_f32_t, pixels_bytes, pix);

  v4 * out = pix;
  char print_str[10000] = {0};
  SDL_snprintf(print_str, 10000, "pixels[%d].rgba: %f %f %f %f\n", dim - 1,
               out[dim - 1].x, out[dim - 1].y, out[dim - 1].z, out[dim - 1].w);

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Completed", print_str, NULL);

  SDL_free(pix);

  return 0;
}