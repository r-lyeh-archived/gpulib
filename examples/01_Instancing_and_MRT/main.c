#define GPU_DEBUG_CALLBACK
#include "../../gpulib.h"

typedef struct { float x, y, z; } v3;
typedef struct { float x, y, z, w; } v4;

#define let __auto_type const
#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define MAX_STR 10000

struct {
  char monkey_ibo [MAX_STR];
  char sphere_ibo [MAX_STR];
  char teapot_ibo [MAX_STR];
  char texture_1  [MAX_STR];
  char texture_2  [MAX_STR];
  char texture_3  [MAX_STR];
  char cube_1_px  [MAX_STR];
  char cube_1_nx  [MAX_STR];
  char cube_1_py  [MAX_STR];
  char cube_1_ny  [MAX_STR];
  char cube_1_pz  [MAX_STR];
  char cube_1_nz  [MAX_STR];
  char cube_2_px  [MAX_STR];
  char cube_2_nx  [MAX_STR];
  char cube_2_py  [MAX_STR];
  char cube_2_ny  [MAX_STR];
  char cube_2_pz  [MAX_STR];
  char cube_2_nz  [MAX_STR];
  char vs_cube    [MAX_STR];
  char fs_cube    [MAX_STR];
  char vs_mesh    [MAX_STR];
  char fs_mesh    [MAX_STR];
  char vs_quad    [MAX_STR];
  char fs_quad    [MAX_STR];
} g_resources = {
  .monkey_ibo = "data/meshes/monkey.ibo",
  .sphere_ibo = "data/meshes/sphere.ibo",
  .teapot_ibo = "data/meshes/teapot.ibo",
  .texture_1  = "data/textures/texture_1.bmp",
  .texture_2  = "data/textures/texture_2.bmp",
  .texture_3  = "data/textures/texture_3.bmp",
  .cube_1_px  = "data/textures/cube_1_right.bmp",
  .cube_1_nx  = "data/textures/cube_1_left.bmp",
  .cube_1_py  = "data/textures/cube_1_top.bmp",
  .cube_1_ny  = "data/textures/cube_1_bottom.bmp",
  .cube_1_pz  = "data/textures/cube_1_front.bmp",
  .cube_1_nz  = "data/textures/cube_1_back.bmp",
  .cube_2_px  = "data/textures/cube_2_right.bmp",
  .cube_2_nx  = "data/textures/cube_2_left.bmp",
  .cube_2_py  = "data/textures/cube_2_top.bmp",
  .cube_2_ny  = "data/textures/cube_2_bottom.bmp",
  .cube_2_pz  = "data/textures/cube_2_front.bmp",
  .cube_2_nz  = "data/textures/cube_2_back.bmp",
  .vs_cube    = "shaders/cube.vert",
  .fs_cube    = "shaders/cube.frag",
  .vs_mesh    = "shaders/mesh.vert",
  .fs_mesh    = "shaders/mesh.frag",
  .vs_quad    = "shaders/quad.vert",
  .fs_quad    = "shaders/quad.frag"
};

static inline v3 v3addv4(v3 a, v4 b) {
  return (v3){
    a.x + b.x,
    a.y + b.y,
    a.z + b.z
  };
}

static inline v3 v3subv4(v3 a, v4 b) {
  return (v3){
    a.x - b.x,
    a.y - b.y,
    a.z - b.z
  };
}

static inline v4 vermul(v4 a, v4 b) {
  return (v4){
    a.x * b.w + b.x * a.w + (a.y * b.z - b.y * a.z),
    a.y * b.w + b.y * a.w + (a.z * b.x - b.z * a.x),
    a.z * b.w + b.z * a.w + (a.x * b.y - b.x * a.y),
    a.w * b.w - (a.x * b.x + a.y * b.y + a.z * b.z)
  };
}

static inline v4 verneg(v4 v) {
  return (v4){-v.x, -v.y, -v.z, v.w};
}

static inline v4 v4rot(v4 p, v4 v) {
  return vermul(vermul(v, p), verneg(v));
}

static inline float sindegdiv2(float d) { return sin(d * (M_PI / 180.0) / 2.0); }
static inline float cosdegdiv2(float d) { return cos(d * (M_PI / 180.0) / 2.0); }
static inline float tandegdiv2(float d) { return tan(d * (M_PI / 180.0) / 2.0); }

static inline void ReadIBO(const char * ibo_filepath, ptrdiff_t * bytes, v4 * data) {
  SDL_RWops * fd = SDL_RWFromFile(ibo_filepath, "rb");

  SDL_assert(fd != NULL);

  SDL_RWseek(fd, 0, RW_SEEK_END);
  *bytes = SDL_RWtell(fd);
  SDL_RWseek(fd, 0, RW_SEEK_SET);

  if (data)
    SDL_RWread(fd, data, *bytes, 1);

  SDL_RWclose(fd);
}

int main() {
  char * path_exe = SDL_GetBasePath();

  for (int i = 0, size = sizeof(g_resources); i < size; i += MAX_STR) {
    char path_res[MAX_STR] = {0};
    SDL_memcpy(path_res, (char *)&g_resources + i, MAX_STR);
    SDL_snprintf((char *)&g_resources + i, MAX_STR, "%s%s", path_exe, path_res);
  }

  SDL_Window * sdl_window = NULL;
  GpuWindow("Instancing and MRT", 1280, 720, 4, 0, 0, &sdl_window, NULL);
  GpuSetDebugCallback(GpuDebugCallback, NULL);

  ptrdiff_t monkey_bytes = 0;
  ptrdiff_t sphere_bytes = 0;
  ptrdiff_t teapot_bytes = 0;
  ReadIBO(g_resources.monkey_ibo, &monkey_bytes, NULL);
  ReadIBO(g_resources.sphere_ibo, &sphere_bytes, NULL);
  ReadIBO(g_resources.teapot_ibo, &teapot_bytes, NULL);

  void * meshes = GpuMalloc(
    monkey_bytes +
    sphere_bytes +
    teapot_bytes
  );

  v4 * monkey_mesh = meshes;
  v4 * sphere_mesh = meshes;
  v4 * teapot_mesh = meshes;
  uint32_t monkey_mesh_tex = 0;
  uint32_t sphere_mesh_tex = 0;
  uint32_t teapot_mesh_tex = 0;
  {
    ptrdiff_t i = 0;
    monkey_mesh += i / sizeof(v4); monkey_mesh_tex = GpuCast(meshes, gpu_xyzw_f32_t, i, monkey_bytes); i += monkey_bytes;
    sphere_mesh += i / sizeof(v4); sphere_mesh_tex = GpuCast(meshes, gpu_xyzw_f32_t, i, sphere_bytes); i += sphere_bytes;
    teapot_mesh += i / sizeof(v4); teapot_mesh_tex = GpuCast(meshes, gpu_xyzw_f32_t, i, teapot_bytes);
  }

  ReadIBO(g_resources.monkey_ibo, &monkey_bytes, monkey_mesh);
  ReadIBO(g_resources.sphere_ibo, &sphere_bytes, sphere_mesh);
  ReadIBO(g_resources.teapot_ibo, &teapot_bytes, teapot_mesh);

  struct gpu_cmd_t monkey_cmds[] = {[0].count = monkey_bytes / sizeof(v4) / 2, [0].instance_count = 30};
  struct gpu_cmd_t sphere_cmds[] = {[0].count = sphere_bytes / sizeof(v4) / 2, [0].instance_count = 30};
  struct gpu_cmd_t teapot_cmds[] = {[0].count = teapot_bytes / sizeof(v4) / 2, [0].instance_count = 30};

  v3 * pos = GpuMalloc(90 * sizeof(v3));

  for (int i = 0, row = 10, space = 3; i < 90; i += 1) {
    pos[i].x = i * space - (i / row) * row * space;
    pos[i].y = 0;
    pos[i].z = (i / row) * space;
  }

  let monkey_pos_tex = GpuCast(pos, gpu_xyz_f32_t, 0 * 30 * sizeof(v3), 30 * sizeof(v3));
  let sphere_pos_tex = GpuCast(pos, gpu_xyz_f32_t, 1 * 30 * sizeof(v3), 30 * sizeof(v3));
  let teapot_pos_tex = GpuCast(pos, gpu_xyz_f32_t, 2 * 30 * sizeof(v3), 30 * sizeof(v3));

  const char * tex[] = {
    g_resources.texture_1,
    g_resources.texture_2,
    g_resources.texture_3
  };

  const char * cbm_px[] = {g_resources.cube_1_px, g_resources.cube_2_px};
  const char * cbm_nx[] = {g_resources.cube_1_nx, g_resources.cube_2_nx};
  const char * cbm_py[] = {g_resources.cube_1_py, g_resources.cube_2_py};
  const char * cbm_ny[] = {g_resources.cube_1_ny, g_resources.cube_2_ny};
  const char * cbm_pz[] = {g_resources.cube_1_pz, g_resources.cube_2_pz};
  const char * cbm_nz[] = {g_resources.cube_1_nz, g_resources.cube_2_nz};

  let textures = GpuMallocImg(gpu_srgb_s3tc_dxt1_b8_t, 512, 512, countof(tex), 1);
  let skyboxes = GpuMallocCbm(gpu_srgb_s3tc_dxt1_b8_t, 512, 512, countof(cbm_px), 1);

  GpuBmpImg(textures, 512, 512, countof(tex), tex);
  GpuBmpCbm(skyboxes, 512, 512, countof(cbm_px), cbm_px, cbm_nx, cbm_py, cbm_ny, cbm_pz, cbm_nz);

  let monkey_tex = GpuCastImg(textures, gpu_srgb_s3tc_dxt1_b8_t, 0, 1, 0, 1);
  let sphere_tex = GpuCastImg(textures, gpu_srgb_s3tc_dxt1_b8_t, 1, 1, 0, 1);
  let teapot_tex = GpuCastImg(textures, gpu_srgb_s3tc_dxt1_b8_t, 2, 1, 0, 1);

  let mrt_msi_depth = GpuMallocMsi(gpu_d_f32_t, 1280, 720, 1, 4);
  let mrt_msi_color = GpuMallocMsi(gpu_srgba_b8_t, 1280, 720, 1, 4);
  let mrt_nms_color = GpuMallocImg(gpu_srgba_b8_t, 1280, 720, 1, 1);

  let mrt_msi_fbo = GpuFbo(mrt_msi_color, 0, 0, 0, 0, 0, 0, 0, mrt_msi_depth, 0);
  let mrt_nms_fbo = GpuFbo(mrt_nms_color, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  uint32_t monkey_textures[] = {
    [0] = monkey_mesh_tex,
    [1] = monkey_pos_tex,
    [2] = monkey_tex
  };

  uint32_t sphere_textures[] = {
    [0] = sphere_mesh_tex,
    [1] = sphere_pos_tex,
    [2] = sphere_tex
  };

  uint32_t teapot_textures[] = {
    [0] = teapot_mesh_tex,
    [1] = teapot_pos_tex,
    [2] = teapot_tex
  };

  uint32_t other_textures[] = {
    [3] = skyboxes,
    [4] = mrt_nms_color
  };

  let smp_textures = GpuSmp(4, gpu_linear_mipmap_linear_t, gpu_linear_t, gpu_repeat_t);
  let smp_mrtcolor = GpuSmp(1, gpu_nearest_t, gpu_nearest_t, gpu_clamp_to_border_t);

  uint32_t mesh_samplers[] = {
    [2] = smp_textures
  };

  uint32_t other_samplers[] = {
    [3] = smp_textures,
    [4] = smp_mrtcolor
  };

  let mesh_vert = GpuVertFile(g_resources.vs_mesh);
  let mesh_frag = GpuFragFile(g_resources.fs_mesh);

  let quad_vert = GpuVertFile(g_resources.vs_quad);
  let quad_frag = GpuFragFile(g_resources.fs_quad);

  let cube_vert = GpuVertFile(g_resources.vs_cube);
  let cube_frag = GpuFragFile(g_resources.fs_cube);

  let mesh_ppo = GpuPpo(mesh_vert, mesh_frag);
  let quad_ppo = GpuPpo(quad_vert, quad_frag);
  let cube_ppo = GpuPpo(cube_vert, cube_frag);

  struct gpu_op_t ops[] = {
    [0].vert = mesh_vert, [0].frag = mesh_frag, [0].ppo = mesh_ppo, [0].smp_count = countof(mesh_samplers), [0].smp = mesh_samplers, [0].mode = gpu_triangles_t,
    [1].vert = mesh_vert, [1].frag = mesh_frag, [1].ppo = mesh_ppo, [1].smp_count = countof(mesh_samplers), [1].smp = mesh_samplers, [1].mode = gpu_triangles_t,
    [2].vert = mesh_vert, [2].frag = mesh_frag, [2].ppo = mesh_ppo, [2].smp_count = countof(mesh_samplers), [2].smp = mesh_samplers, [2].mode = gpu_triangles_t,

    [0].id = 0,
    [0].tex_count = countof(monkey_textures),
    [0].cmd_count = countof(monkey_cmds),
    [0].tex = monkey_textures,
    [0].cmd = monkey_cmds,

    [1].id = 1,
    [1].tex_count = countof(sphere_textures),
    [1].cmd_count = countof(sphere_cmds),
    [1].tex = sphere_textures,
    [1].cmd = sphere_cmds,

    [2].id = 2,
    [2].tex_count = countof(teapot_textures),
    [2].cmd_count = countof(teapot_cmds),
    [2].tex = teapot_textures,
    [2].cmd = teapot_cmds
  };

  struct gpu_op_t skybox_ops[] = {
    [0].tex_count = countof(other_textures),
    [0].smp_count = countof(other_samplers),
    [0].tex = other_textures,
    [0].smp = other_samplers,
    [0].ppo = cube_ppo,
    [0].mode = gpu_triangles_t,
    [0].cmd_count = 1,
    [0].cmd = (struct gpu_cmd_t []){[0].count = 36, [0].instance_count = 1}
  };

  struct gpu_op_t mrt_ops[] = {
    [0].tex_count = countof(other_textures),
    [0].smp_count = countof(other_samplers),
    [0].tex = other_textures,
    [0].smp = other_samplers,
    [0].ppo = quad_ppo,
    [0].mode = gpu_triangles_t,
    [0].cmd_count = 1,
    [0].cmd = (struct gpu_cmd_t []){[0].count = 6, [0].instance_count = 1}
  };

  v3 cam_pos = {23.518875, 5.673130, 26.64900};
  v4 cam_rot = {-0.351835, 0.231701, 0.090335, 0.902411};

  float fov = 1.0 / tandegdiv2(85.0);
  float fov_x = fov / (1280 / 720.0);
  float fov_y = fov;

  SDL_SetRelativeMouseMode(1);
  uint32_t t_prev = SDL_GetTicks();

  for (;;) {
    uint32_t t_curr = SDL_GetTicks();
    double dt = ((t_curr - t_prev) * 60.0) / 1000.0;

    SDL_PumpEvents();
    const unsigned char * key = SDL_GetKeyboardState(0);

    int mx_i32 = 0;
    int my_i32 = 0;
    SDL_GetRelativeMouseState(&mx_i32, &my_i32);
    float mx = mx_i32 * -0.05;
    float my = my_i32 * -0.05;
    cam_rot = vermul(cam_rot, (v4){sindegdiv2(my), 0, 0, cosdegdiv2(my)});
    cam_rot = vermul((v4){0, sindegdiv2(mx), 0, cosdegdiv2(mx)}, cam_rot);

    if (key[SDL_SCANCODE_D]) cam_pos = v3addv4(cam_pos, v4rot((v4){0.05, 0, 0}, cam_rot));
    if (key[SDL_SCANCODE_A]) cam_pos = v3subv4(cam_pos, v4rot((v4){0.05, 0, 0}, cam_rot));
    if (key[SDL_SCANCODE_E]) cam_pos = v3addv4(cam_pos, v4rot((v4){0, 0.05, 0}, cam_rot));
    if (key[SDL_SCANCODE_Q]) cam_pos = v3subv4(cam_pos, v4rot((v4){0, 0.05, 0}, cam_rot));
    if (key[SDL_SCANCODE_S]) cam_pos = v3addv4(cam_pos, v4rot((v4){0, 0, 0.05}, cam_rot));
    if (key[SDL_SCANCODE_W]) cam_pos = v3subv4(cam_pos, v4rot((v4){0, 0, 0.05}, cam_rot));

    static int show_pass = 0;
    if (key[SDL_SCANCODE_1]) show_pass = 1;
    if (key[SDL_SCANCODE_2]) show_pass = 2;
    if (key[SDL_SCANCODE_3]) show_pass = 3;
    if (key[SDL_SCANCODE_4]) show_pass = 4;
    if (key[SDL_SCANCODE_5]) show_pass = 5;
    if (key[SDL_SCANCODE_6]) show_pass = 6;
    if (key[SDL_SCANCODE_7]) show_pass = 7;
    if (key[SDL_SCANCODE_8]) show_pass = 8;

    for (int i = 0; i < 90; i += 1)
      pos[i].y = sin(t_curr * 0.0015 + i * 0.5) * 0.3;

    GpuV3f(mesh_vert, 1, 1, &cam_pos.x);
    GpuV4f(mesh_vert, 2, 1, &cam_rot.x);
    GpuF32(mesh_vert, 3, 1, &fov_x);
    GpuF32(mesh_vert, 4, 1, &fov_y);

    GpuV3f(mesh_frag, 1, 1, &cam_pos.x);
    GpuI32(mesh_frag, 2, 1, &show_pass);

    GpuV4f(cube_vert, 0, 1, &cam_rot.x);
    GpuF32(cube_vert, 1, 1, &fov_x);
    GpuF32(cube_vert, 2, 1, &fov_y);

    static int cube_id = 0;
    if (key[SDL_SCANCODE_9]) { cube_id = 1; show_pass = 0; }
    if (key[SDL_SCANCODE_0]) { cube_id = 0; show_pass = 0; }
    GpuI32(mesh_frag, 3, 1, &cube_id);
    GpuI32(cube_frag, 0, 1, &cube_id);

    float t = t_curr / 1000.0;
    GpuF32(quad_frag, 0, 1, &t);

    GpuBindFbo(mrt_msi_fbo);
    GpuClear();
    GpuDraw(countof(ops), ops);
    GpuBindFbo(0);

    GpuBlit(mrt_msi_fbo, 0, 0, 0, 1280, 720,
            mrt_nms_fbo, 0, 0, 0, 1280, 720);

    GpuClear();
    if (!show_pass) {
      GpuDisable(gpu_depth_t);
      GpuDraw(countof(skybox_ops), skybox_ops);
      GpuEnable(gpu_depth_t);
    }
    GpuDraw(countof(mrt_ops), mrt_ops);

    GpuSwap(sdl_window);

    t_prev = t_curr;

    for (SDL_Event e; SDL_PollEvent(&e);) {
      if (e.type == SDL_QUIT)
        goto exit;
    }
  }

exit:
  return 0;
}