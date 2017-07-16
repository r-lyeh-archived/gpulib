#include "../../gpulib.h"

typedef struct { float x, y, z; } v3;

#define let __auto_type const
#define countof(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  SDL_Window * sdl_window = NULL;
  GpuWindow("Transform Feedback", 1280, 720, 4, 0, 0, &sdl_window, NULL);
  GpuSetDebugCallback(GpuDebugCallback, NULL);

  v3 * vector1 = GpuMalloc(4 * sizeof(v3));
  v3 * vector2 = GpuMalloc(4 * sizeof(v3));
  v3 * vector3 = GpuMalloc(4 * sizeof(v3));

  vector1[0].x = 1.0;
  vector1[0].y = 2.0;
  vector1[0].z = 3.0;

  vector1[1].x = 4.0;
  vector1[1].y = 5.0;
  vector1[1].z = 6.0;

  vector1[2].x = 7.0;
  vector1[2].y = 8.0;
  vector1[2].z = 9.0;

  vector1[3].x = 10.0;
  vector1[3].y = 11.0;
  vector1[3].z = 12.0;

  vector2[0].x = 13.0;
  vector2[0].y = 14.0;
  vector2[0].z = 15.0;

  vector2[1].x = 16.0;
  vector2[1].y = 17.0;
  vector2[1].z = 18.0;

  vector2[2].x = 19.0;
  vector2[2].y = 20.0;
  vector2[2].z = 21.0;

  vector2[3].x = 22.0;
  vector2[3].y = 23.0;
  vector2[3].z = 24.0;

  char * vert_string = GPU_VERT_HEAD
      " layout(binding = 0) uniform samplerBuffer s_v1; \n"
      " layout(binding = 1) uniform samplerBuffer s_v2; \n"
      "                                                 \n"
      " out vec3 out_v3;                                \n"
      "                                                 \n"
      " void main() {                                   \n"
      "   vec3 v1 = texelFetch(s_v1, gl_VertexID).xyz;  \n"
      "   vec3 v2 = texelFetch(s_v2, gl_VertexID).xyz;  \n"
      "                                                 \n"
      "   out_v3 = v1 + v2;                             \n"
      " }                                               \n";

  let compute_vert = GpuVertXfb(vert_string, "out_v3", NULL, NULL, NULL);

  let xfb = GpuXfb(vector3, 0, 4 * sizeof(v3), NULL, 0, 0, NULL, 0, 0, NULL, 0, 0);
  let ppo = GpuPpo(compute_vert, 0);

  let vector1_tex = GpuCast(vector1, gpu_xyz_f32_t, 0, 4 * sizeof(v3));
  let vector2_tex = GpuCast(vector2, gpu_xyz_f32_t, 0, 4 * sizeof(v3));

  unsigned textures[] = {
    [0] = vector1_tex,
    [1] = vector2_tex
  };

  struct gpu_op_t ops[] = {
    [0].tex_count = countof(textures),
    [0].tex = textures,
    [0].mode = gpu_points_t,
    [0].ppo = ppo,
    [0].cmd_count = 1,
    [0].cmd = (struct gpu_cmd_t []){[0].count = 4, [0].instance_count = 1}
  };

  GpuBindXfb(xfb);
  GpuDrawXfb(countof(ops), ops);
  GpuBindXfb(0);
  GpuFinish();

  char print_str[10000] = {0};
  SDL_snprintf(
      print_str, 10000,
      "vector3[0].xyz: %f %f %f\n"
      "vector3[1].xyz: %f %f %f\n"
      "vector3[2].xyz: %f %f %f\n"
      "vector3[3].xyz: %f %f %f\n",
      vector3[0].x, vector3[0].y, vector3[0].z,
      vector3[1].x, vector3[1].y, vector3[1].z,
      vector3[2].x, vector3[2].y, vector3[2].z,
      vector3[3].x, vector3[3].y, vector3[3].z);

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Completed", print_str, NULL);
  return 0;
}