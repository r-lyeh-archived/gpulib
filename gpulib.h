#pragma once

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED 1
#endif

#include "opengl_functions.h"
#include "tinyprofiler.h"
#include "SDL2/SDL.h"

#ifdef _MSC_VER
  #ifndef alloca
  #define alloca _alloca
  #endif
  #ifndef __auto_type
  #define __auto_type auto
  #endif
#endif

struct gpu_cmd_t {
  int count;
  int instance_count;
  int first;
  int instance_first;
};

struct gpu_op_t {
  struct gpu_cmd_t * cmd;
  unsigned * tex;
  unsigned * smp;
  unsigned ppo;
  unsigned mode;
  int cmd_count;
  int tex_first;
  int tex_count;
  int smp_first;
  int smp_count;
  int index;     // Optional index to set to the location 0 uniform
  unsigned vert; // Optional program id for which index will be set
  unsigned frag; // Optional program id for which index will be set
};

enum gpu_draw_t {
  gpu_points_t    = 0x0000, // GL_POINTS
  gpu_lines_t     = 0x0001, // GL_LINES
  gpu_triangles_t = 0x0004  // GL_TRIANGLES
};

enum gpu_shader_t {
  gpu_frag_t = 0x8B30, // GL_FRAGMENT_SHADER
  gpu_vert_t = 0x8B31  // GL_VERTEX_SHADER
};

enum gpu_global_t {
  gpu_cull_t     = 0x0B44, // GL_CULL_FACE
  gpu_depth_t    = 0x0B71, // GL_DEPTH_TEST
  gpu_scissor_t  = 0x0C11, // GL_SCISSOR_TEST
  gpu_fbo_srgb_t = 0x8DB9  // GL_FRAMEBUFFER_SRGB
};

enum gpu_buf_format_t {
  gpu_x_b8_t     = 0x8229, // GL_R8
  gpu_x_f16_t    = 0x822D, // GL_R16F
  gpu_x_f32_t    = 0x822E, // GL_R32F
  gpu_x_i8_t     = 0x8231, // GL_R8I
  gpu_x_i16_t    = 0x8233, // GL_R16I
  gpu_x_i32_t    = 0x8235, // GL_R32I
  gpu_x_u8_t     = 0x8232, // GL_R8UI
  gpu_x_u16_t    = 0x8234, // GL_R16UI
  gpu_x_u32_t    = 0x8236, // GL_R32UI
  gpu_xy_b8_t    = 0x822B, // GL_RG8
  gpu_xy_f16_t   = 0x822F, // GL_RG16F
  gpu_xy_f32_t   = 0x8230, // GL_RG32F
  gpu_xy_i8_t    = 0x8237, // GL_RG8I
  gpu_xy_i16_t   = 0x8239, // GL_RG16I
  gpu_xy_i32_t   = 0x823B, // GL_RG32I
  gpu_xy_u8_t    = 0x8238, // GL_RG8UI
  gpu_xy_u16_t   = 0x823A, // GL_RG16UI
  gpu_xy_u32_t   = 0x823C, // GL_RG32UI
  gpu_xyz_f32_t  = 0x8815, // GL_RGB32F
  gpu_xyz_i32_t  = 0x8D83, // GL_RGB32I
  gpu_xyz_u32_t  = 0x8D71, // GL_RGB32UI
  gpu_xyzw_b8_t  = 0x8058, // GL_RGBA8
  gpu_xyzw_f16_t = 0x881A, // GL_RGBA16F
  gpu_xyzw_f32_t = 0x8814, // GL_RGBA32F
  gpu_xyzw_i8_t  = 0x8D8E, // GL_RGBA8I
  gpu_xyzw_i16_t = 0x8D88, // GL_RGBA16I
  gpu_xyzw_i32_t = 0x8D82, // GL_RGBA32I
  gpu_xyzw_u8_t  = 0x8D7C, // GL_RGBA8UI
  gpu_xyzw_u16_t = 0x8D76, // GL_RGBA16UI
  gpu_xyzw_u32_t = 0x8D70  // GL_RGBA32UI
};

enum gpu_tex_format_t {
  gpu_d_f32_t    = 0x8CAC, // GL_DEPTH_COMPONENT32F
  gpu_rgb_b8_t   = 0x8051, // GL_RGB8
  gpu_rgba_b8_t  = 0x8058, // GL_RGBA8
  gpu_srgb_b8_t  = 0x8C41, // GL_SRGB8
  gpu_srgba_b8_t = 0x8C43, // GL_SRGB8_ALPHA8
  gpu_rgba_f32_t = 0x8814, // GL_RGBA32F
  gpu_rgb_s3tc_dxt1_b8_t   = 0x83F0, // GL_COMPRESSED_RGB_S3TC_DXT1_EXT
  gpu_rgba_s3tc_dxt1_b8_t  = 0x83F1, // GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
  gpu_rgba_s3tc_dxt3_b8_t  = 0x83F2, // GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
  gpu_rgba_s3tc_dxt5_b8_t  = 0x83F3, // GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
  gpu_srgb_s3tc_dxt1_b8_t  = 0x8C4C, // GL_COMPRESSED_SRGB_S3TC_DXT1_EXT
  gpu_srgba_s3tc_dxt1_b8_t = 0x8C4D, // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT
  gpu_srgba_s3tc_dxt3_b8_t = 0x8C4E, // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT
  gpu_srgba_s3tc_dxt5_b8_t = 0x8C4F  // GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT
};

enum gpu_smp_filter_t {
  gpu_nearest_t                = 0x2600, // GL_NEAREST
  gpu_linear_t                 = 0x2601, // GL_LINEAR
  gpu_nearest_mipmap_nearest_t = 0x2700, // GL_NEAREST_MIPMAP_NEAREST
  gpu_linear_mipmap_nearest_t  = 0x2701, // GL_LINEAR_MIPMAP_NEAREST
  gpu_nearest_mipmap_linear_t  = 0x2702, // GL_NEAREST_MIPMAP_LINEAR
  gpu_linear_mipmap_linear_t   = 0x2703  // GL_LINEAR_MIPMAP_LINEAR
};

enum gpu_smp_wrapping_t {
  gpu_repeat_t                 = 0x2901, // GL_REPEAT
  gpu_clamp_to_edge_t          = 0x812F, // GL_CLAMP_TO_EDGE
  gpu_clamp_to_border_t        = 0x812D, // GL_CLAMP_TO_BORDER
  gpu_mirrored_repeat_t        = 0x8370, // GL_MIRRORED_REPEAT
  gpu_mirror_clamp_t           = 0x8742, // GL_MIRROR_CLAMP
  gpu_mirror_clamp_to_edge_t   = 0x8743, // GL_MIRROR_CLAMP_TO_EDGE
  gpu_mirror_clamp_to_border_t = 0x8912  // GL_MIRROR_CLAMP_TO_BORDER
};

enum gpu_pix_format_t {
  gpu_r_t    = 0x1903, // GL_RED
  gpu_rgb_t  = 0x1907, // GL_RGB
  gpu_bgr_t  = 0x80E0, // GL_BGR
  gpu_rgba_t = 0x1908, // GL_RGBA
  gpu_bgra_t = 0x80E1  // GL_BGRA
};

enum gpu_pix_t {
  gpu_i8_t  = 0x1400, // GL_BYTE
  gpu_i16_t = 0x1402, // GL_SHORT
  gpu_i32_t = 0x1404, // GL_INT
  gpu_u8_t  = 0x1401, // GL_UNSIGNED_BYTE
  gpu_u16_t = 0x1403, // GL_UNSIGNED_SHORT
  gpu_u32_t = 0x1405, // GL_UNSIGNED_INT
  gpu_f32_t = 0x1406  // GL_FLOAT
};

static inline void GpuCheckExts(int extensions_count, char ** extensions) {
  profB(__func__);
  for (int i = 0; i < extensions_count; i += 1) {
    if (SDL_GL_ExtensionSupported(extensions[i]) == SDL_FALSE)
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR: Unsupported OpenGL Extension", extensions[i], NULL);
  }
  profE(__func__);
}

static inline void GpuWindow(
    char * window_title, int window_width, int window_height, int msaa_samples,
    unsigned sdl_init_flags, unsigned sdl_window_flags, SDL_Window ** sdl_window, void ** sdl_glcontext)
{
  profB(__func__);
  SDL_assert(sdl_window != NULL);

  int sdl_init = SDL_Init(SDL_INIT_VIDEO | sdl_init_flags);

  SDL_assert(sdl_init == 0);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa_samples);
  SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

  *sdl_window = SDL_CreateWindow(
      window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height,
      SDL_WINDOW_OPENGL | sdl_window_flags);

  SDL_assert(*sdl_window != NULL);

  void * glcontext = SDL_GL_CreateContext(*sdl_window);

  SDL_assert(glcontext != NULL);

  if (sdl_glcontext)
     *sdl_glcontext = glcontext;

  int gl_load = SDL_GL_LoadLibrary(NULL);

  SDL_assert(gl_load == 0);

  char * extensions[] = {
#ifndef RELEASE
    "GL_KHR_debug",
#endif
    "GL_ARB_multi_bind",
    "GL_ARB_gpu_shader5",
    "GL_ARB_clip_control",
    "GL_ARB_texture_view",
    "GL_EXT_texture_sRGB",
    "GL_ARB_clear_texture",
    "GL_ARB_buffer_storage",
    "GL_ARB_texture_storage",
    "GL_ARB_shader_precision",
    "GL_ARB_get_program_binary",
    "GL_ARB_transform_feedback2",
    "GL_ARB_direct_state_access",
    "GL_ARB_texture_buffer_range",
    "GL_EXT_texture_mirror_clamp",
    "GL_ARB_get_texture_sub_image",
    "GL_ARB_texture_cube_map_array",
    "GL_ARB_separate_shader_objects",
    "GL_ARB_shading_language_420pack",
    "GL_ARB_shading_language_packing",
    "GL_EXT_texture_compression_s3tc",
    "GL_EXT_texture_filter_anisotropic",
    "GL_ARB_texture_buffer_object_rgb32",
    "GL_ARB_texture_storage_multisample"
  };

  GpuCheckExts(sizeof(extensions) / sizeof(extensions[0]), extensions);

  glAttachShader = SDL_GL_GetProcAddress("glAttachShader");
  glBeginTransformFeedback = SDL_GL_GetProcAddress("glBeginTransformFeedback");
  glBindFramebuffer = SDL_GL_GetProcAddress("glBindFramebuffer");
  glBindProgramPipeline = SDL_GL_GetProcAddress("glBindProgramPipeline");
  glBindSamplers = SDL_GL_GetProcAddress("glBindSamplers");
  glBindTextures = SDL_GL_GetProcAddress("glBindTextures");
  glBindTransformFeedback = SDL_GL_GetProcAddress("glBindTransformFeedback");
  glBlitNamedFramebuffer = SDL_GL_GetProcAddress("glBlitNamedFramebuffer");
  glClear = SDL_GL_GetProcAddress("glClear");
  glClearColor = SDL_GL_GetProcAddress("glClearColor");
  glClearTexSubImage = SDL_GL_GetProcAddress("glClearTexSubImage");
  glCompressedTextureSubImage3D = SDL_GL_GetProcAddress("glCompressedTextureSubImage3D");
  glCompileShader = SDL_GL_GetProcAddress("glCompileShader");
  glCreateBuffers = SDL_GL_GetProcAddress("glCreateBuffers");
  glCreateFramebuffers = SDL_GL_GetProcAddress("glCreateFramebuffers");
  glCreateProgram = SDL_GL_GetProcAddress("glCreateProgram");
  glCreateProgramPipelines = SDL_GL_GetProcAddress("glCreateProgramPipelines");
  glCreateSamplers = SDL_GL_GetProcAddress("glCreateSamplers");
  glCreateShader = SDL_GL_GetProcAddress("glCreateShader");
  glCreateTextures = SDL_GL_GetProcAddress("glCreateTextures");
  glCreateTransformFeedbacks = SDL_GL_GetProcAddress("glCreateTransformFeedbacks");
  glDebugMessageCallback = SDL_GL_GetProcAddress("glDebugMessageCallback");
  glDeleteBuffers = SDL_GL_GetProcAddress("glDeleteBuffers");
  glDeleteFramebuffers = SDL_GL_GetProcAddress("glDeleteFramebuffers");
  glDeleteProgram = SDL_GL_GetProcAddress("glDeleteProgram");
  glDeleteProgramPipelines = SDL_GL_GetProcAddress("glDeleteProgramPipelines");
  glDeleteSamplers = SDL_GL_GetProcAddress("glDeleteSamplers");
  glDeleteShader = SDL_GL_GetProcAddress("glDeleteShader");
  glDeleteTextures = SDL_GL_GetProcAddress("glDeleteTextures");
  glDeleteTransformFeedbacks = SDL_GL_GetProcAddress("glDeleteTransformFeedbacks");
  glDetachShader = SDL_GL_GetProcAddress("glDetachShader");
  glDisable = SDL_GL_GetProcAddress("glDisable");
  glDrawArraysInstanced = SDL_GL_GetProcAddress("glDrawArraysInstanced");
  glEnable = SDL_GL_GetProcAddress("glEnable");
  glEndTransformFeedback = SDL_GL_GetProcAddress("glEndTransformFeedback");
  glFinish = SDL_GL_GetProcAddress("glFinish");
  glGenTextures = SDL_GL_GetProcAddress("glGenTextures");
  glGetCompressedTextureSubImage = SDL_GL_GetProcAddress("glGetCompressedTextureSubImage");
  glGetTextureSubImage = SDL_GL_GetProcAddress("glGetTextureSubImage");
  glLinkProgram = SDL_GL_GetProcAddress("glLinkProgram");
  glMapNamedBufferRange = SDL_GL_GetProcAddress("glMapNamedBufferRange");
  glNamedBufferStorage = SDL_GL_GetProcAddress("glNamedBufferStorage");
  glNamedFramebufferDrawBuffer = SDL_GL_GetProcAddress("glNamedFramebufferDrawBuffer");
  glNamedFramebufferDrawBuffers = SDL_GL_GetProcAddress("glNamedFramebufferDrawBuffers");
  glNamedFramebufferReadBuffer = SDL_GL_GetProcAddress("glNamedFramebufferReadBuffer");
  glNamedFramebufferTextureLayer = SDL_GL_GetProcAddress("glNamedFramebufferTextureLayer");
  glProgramParameteri = SDL_GL_GetProcAddress("glProgramParameteri");
  glProgramUniform1fv = SDL_GL_GetProcAddress("glProgramUniform1fv");
  glProgramUniform1iv = SDL_GL_GetProcAddress("glProgramUniform1iv");
  glProgramUniform1uiv = SDL_GL_GetProcAddress("glProgramUniform1uiv");
  glProgramUniform2fv = SDL_GL_GetProcAddress("glProgramUniform2fv");
  glProgramUniform3fv = SDL_GL_GetProcAddress("glProgramUniform3fv");
  glProgramUniform4fv = SDL_GL_GetProcAddress("glProgramUniform4fv");
  glSamplerParameteri = SDL_GL_GetProcAddress("glSamplerParameteri");
  glScissor = SDL_GL_GetProcAddress("glScissor");
  glShaderSource = SDL_GL_GetProcAddress("glShaderSource");
  glTextureBufferRange = SDL_GL_GetProcAddress("glTextureBufferRange");
  glTextureStorage3D = SDL_GL_GetProcAddress("glTextureStorage3D");
  glTextureStorage3DMultisample = SDL_GL_GetProcAddress("glTextureStorage3DMultisample");
  glTextureSubImage3D = SDL_GL_GetProcAddress("glTextureSubImage3D");
  glTextureView = SDL_GL_GetProcAddress("glTextureView");
  glTransformFeedbackBufferRange = SDL_GL_GetProcAddress("glTransformFeedbackBufferRange");
  glTransformFeedbackVaryings = SDL_GL_GetProcAddress("glTransformFeedbackVaryings");
  glUseProgramStages = SDL_GL_GetProcAddress("glUseProgramStages");
  glViewport = SDL_GL_GetProcAddress("glViewport");

#ifndef RELEASE
  glEnable(0x92E0); // GL_DEBUG_OUTPUT
  glEnable(0x8242); // GL_DEBUG_OUTPUT_SYNCHRONOUS
#endif
  glEnable(0x884F); // GL_TEXTURE_CUBE_MAP_SEAMLESS
  glEnable(0x8DB9); // GL_FRAMEBUFFER_SRGB
  glEnable(0x809D); // GL_MULTISAMPLE
  glEnable(0x864f); // GL_DEPTH_CLAMP
  glEnable(0x0B71); // GL_DEPTH_TEST
  glEnable(0x0B44); // GL_CULL_FACE
  glEnable(0x0BE2); // GL_BLEND

  void (*glBlendFunc)(unsigned, unsigned) = SDL_GL_GetProcAddress("glBlendFunc");
  void (*glClipControl)(unsigned, unsigned) = SDL_GL_GetProcAddress("glClipControl");
  void (*glDepthRange)(double, double) = SDL_GL_GetProcAddress("glDepthRange");
  void (*glClearDepth)(double) = SDL_GL_GetProcAddress("glClearDepth");
  void (*glDepthFunc)(unsigned) = SDL_GL_GetProcAddress("glDepthFunc");

  glBlendFunc(0x0302, 0x0303); // GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
  glClipControl(0x8CA1, 0x935F); // GL_LOWER_LEFT, GL_ZERO_TO_ONE
  glDepthRange(1, 0);
  glClearDepth(0);
  glDepthFunc(0x0204); // GL_GREATER

  // OPENGL REQUIRED GARBAGE //////////////////////////////////////////////////////////////////////
  void (*glCreateVertexArrays)(int, unsigned *) = SDL_GL_GetProcAddress("glCreateVertexArrays"); //
  void (*glBindVertexArray)(unsigned) = SDL_GL_GetProcAddress("glBindVertexArray");              //
  unsigned vao = 0;                                                                              //
  glCreateVertexArrays(1, &vao);                                                                 //
  glBindVertexArray(vao);                                                                        //
  /////////////////////////////////////////////////////////////////////////////////////////////////
  profE(__func__);
}

static inline void * GpuMalloc(int bytes) {
  profB(__func__);
  unsigned buf_id = 0;
  glCreateBuffers(1, &buf_id);

  int size = 4 + bytes;

  glNamedBufferStorage(buf_id, size, NULL, 194);
  void * p = glMapNamedBufferRange(buf_id, 0, size, 194);

  if (p == NULL)
    return NULL;

  unsigned * p_u32 = p;
  p_u32[0] = buf_id;
  p_u32 += 1;
  profE(__func__);

  return (void *)p_u32;
}

static inline unsigned GpuCast(void * gpu_buf_ptr, enum gpu_buf_format_t format, int bytes_first, int bytes_count) {
  profB(__func__);
  unsigned tex_id = 0;
  glCreateTextures(35882, 1, &tex_id);

  unsigned buf_id = ((unsigned *)gpu_buf_ptr)[-1];

  glTextureBufferRange(tex_id, format, buf_id, 4 + bytes_first, bytes_count);
  profE(__func__);

  return tex_id;
}

static inline unsigned GpuMallocImg(enum gpu_tex_format_t format, int width, int height, int layer_count, int mipmap_count) {
  profB(__func__);
  unsigned tex_id = 0;
  glCreateTextures(35866, 1, &tex_id);

  glTextureStorage3D(tex_id, mipmap_count, format, width, height, layer_count);
  profE(__func__);

  return tex_id;
}

static inline unsigned GpuMallocCbm(enum gpu_tex_format_t format, int width, int height, int layer_count, int mipmap_count) {
  profB(__func__);
  unsigned tex_id = 0;
  glCreateTextures(36873, 1, &tex_id);

  glTextureStorage3D(tex_id, mipmap_count, format, width, height, layer_count * 6);
  profE(__func__);

  return tex_id;
}

static inline unsigned GpuMallocMsi(enum gpu_tex_format_t format, int width, int height, int layer_count, int msaa_samples) {
  profB(__func__);
  unsigned tex_id = 0;
  glCreateTextures(37122, 1, &tex_id);

  glTextureStorage3DMultisample(tex_id, msaa_samples, format, width, height, layer_count, 0);
  profE(__func__);

  return tex_id;
}

static inline unsigned GpuCastImg(
    unsigned tex_id, enum gpu_tex_format_t format, int layer_first, int layer_count, int mipmap_first, int mipmap_count)
{
  profB(__func__);
  unsigned new_tex_id = 0;
  glGenTextures(1, &new_tex_id);

  glTextureView(new_tex_id, 35866, tex_id, format, mipmap_first, mipmap_count, layer_first, layer_count);
  profE(__func__);

  return new_tex_id;
}

static inline unsigned GpuCastCbm(
    unsigned tex_id, enum gpu_tex_format_t format, int layer_first, int layer_count, int mipmap_first, int mipmap_count)
{
  profB(__func__);
  unsigned new_tex_id = 0;
  glGenTextures(1, &new_tex_id);

  glTextureView(new_tex_id, 36873, tex_id, format, mipmap_first, mipmap_count, layer_first * 6, layer_count * 6);
  profE(__func__);

  return new_tex_id;
}

static inline unsigned GpuCastMsi(
    unsigned tex_id, enum gpu_tex_format_t format, int layer_first, int layer_count, int mipmap_first, int mipmap_count)
{
  profB(__func__);
  unsigned new_tex_id = 0;
  glGenTextures(1, &new_tex_id);

  glTextureView(new_tex_id, 37122, tex_id, format, mipmap_first, mipmap_count, layer_first, layer_count);
  profE(__func__);

  return new_tex_id;
}

static inline void GpuGet(
    unsigned tex_id, int layer, int x, int y, int width, int height, int count, int mipmap_level,
    enum gpu_pix_format_t pixel_format, enum gpu_pix_t pixel_type, int pixels_bytes, void * pixels)
{
  profB(__func__);
  glGetTextureSubImage(tex_id, mipmap_level, x, y, layer, width, height, count, pixel_format, pixel_type, pixels_bytes, pixels);
  profE(__func__);
}

static inline void GpuSet(
    unsigned tex_id, int layer, int x, int y, int width, int height, int count, int mipmap_level,
    enum gpu_pix_format_t pixel_format, enum gpu_pix_t pixel_type, void * pixels)
{
  profB(__func__);
  glTextureSubImage3D(tex_id, mipmap_level, x, y, layer, width, height, count, pixel_format, pixel_type, pixels);
  profE(__func__);
}

static inline void GpuSetPix(
    unsigned tex_id, int layer, int x, int y, int width, int height, int count, int mipmap_level,
    enum gpu_pix_format_t pixel_format, enum gpu_pix_t pixel_type, void * pixel)
{
  profB(__func__);
  glClearTexSubImage(tex_id, mipmap_level, x, y, layer, width, height, count, pixel_format, pixel_type, pixel);
  profE(__func__);
}

static inline void GpuGetCpi(
    unsigned tex_id, int layer, int x, int y, int width, int height, int count, int mipmap_level,
    int pixels_bytes, void * pixels)
{
  profB(__func__);
  glGetCompressedTextureSubImage(tex_id, mipmap_level, x, y, layer, width, height, count, pixels_bytes, pixels);
  profE(__func__);
}

static inline void GpuSetCpi(
    unsigned tex_id, int layer, int x, int y, int width, int height, int count, int mipmap_level,
    enum gpu_tex_format_t compression_format, int pixels_bytes, void * pixels)
{
  profB(__func__);
  glCompressedTextureSubImage3D(tex_id, mipmap_level, x, y, layer, width, height, count, compression_format, pixels_bytes, pixels);
  profE(__func__);
}

static inline void GpuBmpImg(unsigned tex_id, int width, int height, int layer_count, char ** bmp_filepaths) {
  profB(__func__);
  unsigned char * p = SDL_calloc((width * height * 3) * layer_count, 1);

  for (int i = 0; i < layer_count; i += 1) {
    SDL_Surface * bmp = SDL_LoadBMP(bmp_filepaths[i]);
    if (bmp) {
      SDL_memcpy(&p[(width * height * 3) * i], bmp->pixels, width * height * 3);
      SDL_FreeSurface(bmp);
    }
  }

  glTextureSubImage3D(tex_id, 0, 0, 0, 0, width, height, layer_count, gpu_bgr_t, gpu_u8_t, p);

  SDL_free(p);
  profE(__func__);
}

static inline void GpuBmpCbm(
    unsigned tex_id, int width, int height, int layer_count,
    char ** pos_x_bmp_filepaths,
    char ** neg_x_bmp_filepaths,
    char ** pos_y_bmp_filepaths,
    char ** neg_y_bmp_filepaths,
    char ** pos_z_bmp_filepaths,
    char ** neg_z_bmp_filepaths)
{
  profB(__func__);
  unsigned char * p = SDL_calloc((width * height * 3) * layer_count * 6, 1);

  for (int i = 0; i < layer_count; i += 1) {
    SDL_Surface * pos_x_bmp = SDL_LoadBMP(pos_x_bmp_filepaths[i]);
    SDL_Surface * neg_x_bmp = SDL_LoadBMP(neg_x_bmp_filepaths[i]);
    SDL_Surface * pos_y_bmp = SDL_LoadBMP(pos_y_bmp_filepaths[i]);
    SDL_Surface * neg_y_bmp = SDL_LoadBMP(neg_y_bmp_filepaths[i]);
    SDL_Surface * pos_z_bmp = SDL_LoadBMP(pos_z_bmp_filepaths[i]);
    SDL_Surface * neg_z_bmp = SDL_LoadBMP(neg_z_bmp_filepaths[i]);

    if (pos_x_bmp) SDL_memcpy(&p[(width * height * 3) * (i * 6 + 0)], pos_x_bmp->pixels, width * height * 3);
    if (neg_x_bmp) SDL_memcpy(&p[(width * height * 3) * (i * 6 + 1)], neg_x_bmp->pixels, width * height * 3);
    if (pos_y_bmp) SDL_memcpy(&p[(width * height * 3) * (i * 6 + 2)], pos_y_bmp->pixels, width * height * 3);
    if (neg_y_bmp) SDL_memcpy(&p[(width * height * 3) * (i * 6 + 3)], neg_y_bmp->pixels, width * height * 3);
    if (pos_z_bmp) SDL_memcpy(&p[(width * height * 3) * (i * 6 + 4)], pos_z_bmp->pixels, width * height * 3);
    if (neg_z_bmp) SDL_memcpy(&p[(width * height * 3) * (i * 6 + 5)], neg_z_bmp->pixels, width * height * 3);

    if (pos_x_bmp) SDL_FreeSurface(pos_x_bmp);
    if (neg_x_bmp) SDL_FreeSurface(neg_x_bmp);
    if (pos_y_bmp) SDL_FreeSurface(pos_y_bmp);
    if (neg_y_bmp) SDL_FreeSurface(neg_y_bmp);
    if (pos_z_bmp) SDL_FreeSurface(pos_z_bmp);
    if (neg_z_bmp) SDL_FreeSurface(neg_z_bmp);
  }

  glTextureSubImage3D(tex_id, 0, 0, 0, 0, width, height, layer_count * 6, gpu_bgr_t, gpu_u8_t, p);

  SDL_free(p);
  profE(__func__);
}

static inline unsigned GpuSmp(
    int max_anisotropy, enum gpu_smp_filter_t min_filter, enum gpu_smp_filter_t mag_filter, enum gpu_smp_wrapping_t wrapping)
{
  profB(__func__);
  unsigned smp_id = 0;
  glCreateSamplers(1, &smp_id);

  glSamplerParameteri(smp_id, 34046, max_anisotropy);
  glSamplerParameteri(smp_id, 10241, min_filter);
  glSamplerParameteri(smp_id, 10240, mag_filter);
  glSamplerParameteri(smp_id, 10242, wrapping);
  glSamplerParameteri(smp_id, 10243, wrapping);
  glSamplerParameteri(smp_id, 32882, wrapping);
  profE(__func__);

  return smp_id;
}

static inline unsigned GpuPro(
    enum gpu_shader_t shader_type, char * shader_string,
    char * xfb_name_0,
    char * xfb_name_1,
    char * xfb_name_2,
    char * xfb_name_3)
{
  profB(__func__);
  unsigned shader_id = glCreateShader(shader_type);

  glShaderSource(shader_id, 1, (char **)&shader_string, NULL);
  glCompileShader(shader_id);

  // Fucking AMD Driver™
  {
    int is_compiled = 0;
    void (*glGetShaderiv)(unsigned, unsigned, int *) = SDL_GL_GetProcAddress("glGetShaderiv");
    glGetShaderiv(shader_id, 0x8B81, &is_compiled); // GL_COMPILE_STATUS
    if (!is_compiled) {
      int info_len = 0;
      glGetShaderiv(shader_id, 0x8B84, &info_len); // GL_INFO_LOG_LENGTH
      if (info_len > 1) {
        char * info_log = alloca(info_len + 1);
        void (*glGetShaderInfoLog)(unsigned, int, int *, char *) = SDL_GL_GetProcAddress("glGetShaderInfoLog");
        glGetShaderInfoLog(shader_id, info_len, NULL, info_log);
        printf("Shader compile error:\n%s", info_log);
        {
          int line = 1;
          printf("%04d: ", line);
          for (char c = *shader_string; c != '\0'; c = *++shader_string) {
            printf("%c", c);
            if (c == '\n') {
              line += 1;
              printf("%04d: ", line);
            }
          }
          printf("\n");
        }
      }
      glDeleteShader(shader_id);
      return 0;
    }
  }

  unsigned pro_id = glCreateProgram();
  glProgramParameteri(pro_id, 33368, 1);

  glAttachShader(pro_id, shader_id);

  char * xfb_names[4] = {xfb_name_0, xfb_name_1, xfb_name_2, xfb_name_3};
  int xfb_count = 0;
  if (xfb_name_0) xfb_count += 1;
  if (xfb_name_1) xfb_count += 1;
  if (xfb_name_2) xfb_count += 1;
  if (xfb_name_3) xfb_count += 1;
  if (xfb_count > 0)
    glTransformFeedbackVaryings(pro_id, xfb_count, xfb_names, 35981);

  glLinkProgram(pro_id);

  // Fucking AMD Driver™
  {
    int is_linked = 0;
    void (*glGetProgramiv)(unsigned, unsigned, int *) = SDL_GL_GetProcAddress("glGetProgramiv");
    glGetProgramiv(pro_id, 0x8B82, &is_linked); // GL_LINK_STATUS
    if (!is_linked) {
      int info_len = 0;
      glGetProgramiv(pro_id, 0x8B84, &info_len); // GL_INFO_LOG_LENGTH
      if (info_len > 1) {
        char * info_log = alloca(info_len + 1);
        void (*glGetProgramInfoLog)(unsigned, int, int *, char *) = SDL_GL_GetProcAddress("glGetProgramInfoLog");
        glGetProgramInfoLog(pro_id, info_len, NULL, info_log);
        printf("Program link error:\n%s", info_log);
        printf("Transform feedback varyings count: %d\n", xfb_count);
        printf("Transform feedback varying name 0: %s%s%s\n", xfb_name_0 ? "\"" : "", xfb_name_0 ? xfb_name_0 : "NULL", xfb_name_0 ? "\"" : "");
        printf("Transform feedback varying name 1: %s%s%s\n", xfb_name_1 ? "\"" : "", xfb_name_1 ? xfb_name_1 : "NULL", xfb_name_1 ? "\"" : "");
        printf("Transform feedback varying name 2: %s%s%s\n", xfb_name_2 ? "\"" : "", xfb_name_2 ? xfb_name_2 : "NULL", xfb_name_2 ? "\"" : "");
        printf("Transform feedback varying name 3: %s%s%s\n", xfb_name_3 ? "\"" : "", xfb_name_3 ? xfb_name_3 : "NULL", xfb_name_3 ? "\"" : "");
        {
          int line = 1;
          printf("%04d: ", line);
          for (char c = *shader_string; c != '\0'; c = *++shader_string) {
            printf("%c", c);
            if (c == '\n') {
              line += 1;
              printf("%04d: ", line);
            }
          }
          printf("\n");
        }
      }
      glDetachShader(pro_id, shader_id);
      glDeleteShader(shader_id);
      return 0;
    }
  }

  glDetachShader(pro_id, shader_id);
  glDeleteShader(shader_id);
  profE(__func__);

  return pro_id;
}

static inline unsigned GpuProFile(
    enum gpu_shader_t shader_type, char * shader_filepath,
    char * xfb_name_0,
    char * xfb_name_1,
    char * xfb_name_2,
    char * xfb_name_3)
{
  profB(__func__);
  SDL_RWops * fd = SDL_RWFromFile(shader_filepath, "rb");

  if (fd == NULL)
    return 0;

  SDL_RWseek(fd, 0, RW_SEEK_END);
  long bytes = SDL_RWtell(fd);
  SDL_RWseek(fd, 0, RW_SEEK_SET);
  char * src = alloca(bytes + 1);
  src[bytes] = 0;
  SDL_RWread(fd, src, bytes, 1);
  SDL_RWclose(fd);
  char * shader_string = &src[0];
  profE(__func__);

  return GpuPro(shader_type, shader_string, xfb_name_0, xfb_name_1, xfb_name_2, xfb_name_3);
}

static inline unsigned GpuVert(char * shader_string) { return GpuPro(gpu_vert_t, shader_string, NULL, NULL, NULL, NULL); }
static inline unsigned GpuFrag(char * shader_string) { return GpuPro(gpu_frag_t, shader_string, NULL, NULL, NULL, NULL); }
static inline unsigned GpuVertFile(char * shader_filepath) { return GpuProFile(gpu_vert_t, shader_filepath, NULL, NULL, NULL, NULL); }
static inline unsigned GpuFragFile(char * shader_filepath) { return GpuProFile(gpu_frag_t, shader_filepath, NULL, NULL, NULL, NULL); }
static inline unsigned GpuVertXfb(char * shader_string, char * xfb_name_0, char * xfb_name_1, char * xfb_name_2, char * xfb_name_3) { return GpuPro(gpu_vert_t, shader_string, xfb_name_0, xfb_name_1, xfb_name_2, xfb_name_3); }
static inline unsigned GpuFragXfb(char * shader_string, char * xfb_name_0, char * xfb_name_1, char * xfb_name_2, char * xfb_name_3) { return GpuPro(gpu_frag_t, shader_string, xfb_name_0, xfb_name_1, xfb_name_2, xfb_name_3); }
static inline unsigned GpuVertXfbFile(char * shader_filepath, char * xfb_name_0, char * xfb_name_1, char * xfb_name_2, char * xfb_name_3) { return GpuProFile(gpu_vert_t, shader_filepath, xfb_name_0, xfb_name_1, xfb_name_2, xfb_name_3); }
static inline unsigned GpuFragXfbFile(char * shader_filepath, char * xfb_name_0, char * xfb_name_1, char * xfb_name_2, char * xfb_name_3) { return GpuProFile(gpu_frag_t, shader_filepath, xfb_name_0, xfb_name_1, xfb_name_2, xfb_name_3); }

static inline void GpuU32(unsigned program, int location, int count, unsigned * value) { glProgramUniform1uiv(program, location, count, value); }
static inline void GpuI32(unsigned program, int location, int count, int      * value) { glProgramUniform1iv(program, location, count, value);  }
static inline void GpuF32(unsigned program, int location, int count, float    * value) { glProgramUniform1fv(program, location, count, value);  }
static inline void GpuV2f(unsigned program, int location, int count, float    * value) { glProgramUniform2fv(program, location, count, value);  }
static inline void GpuV3f(unsigned program, int location, int count, float    * value) { glProgramUniform3fv(program, location, count, value);  }
static inline void GpuV4f(unsigned program, int location, int count, float    * value) { glProgramUniform4fv(program, location, count, value);  }

#ifndef GPU_VERT_HEAD
#define GPU_VERT_HEAD                                       \
  "#version 330                                         \n" \
  "#extension GL_ARB_gpu_shader5               : enable \n" \
  "#extension GL_ARB_shader_precision          : enable \n" \
  "#extension GL_ARB_texture_cube_map_array    : enable \n" \
  "#extension GL_ARB_separate_shader_objects   : enable \n" \
  "#extension GL_ARB_shading_language_420pack  : enable \n" \
  "#extension GL_ARB_shading_language_packing  : enable \n" \
  "#extension GL_ARB_explicit_uniform_location : enable \n" \
  "out gl_PerVertex { vec4 gl_Position; };              \n"
#endif

#ifndef GPU_FRAG_HEAD
#define GPU_FRAG_HEAD                                       \
  "#version 330                                         \n" \
  "#extension GL_ARB_gpu_shader5               : enable \n" \
  "#extension GL_ARB_shader_precision          : enable \n" \
  "#extension GL_ARB_texture_cube_map_array    : enable \n" \
  "#extension GL_ARB_separate_shader_objects   : enable \n" \
  "#extension GL_ARB_shading_language_420pack  : enable \n" \
  "#extension GL_ARB_shading_language_packing  : enable \n" \
  "#extension GL_ARB_explicit_uniform_location : enable \n"
#endif

static inline unsigned GpuPpo(unsigned vert_pro_id, unsigned frag_pro_id) {
  profB(__func__);
  unsigned ppo_id = 0;
  glCreateProgramPipelines(1, &ppo_id);

  if (vert_pro_id) glUseProgramStages(ppo_id, 1, vert_pro_id);
  if (frag_pro_id) glUseProgramStages(ppo_id, 2, frag_pro_id);
  profE(__func__);

  return ppo_id;
}

static inline unsigned GpuFbo(
    unsigned color_tex_id_0, int color_tex_layer_0,
    unsigned color_tex_id_1, int color_tex_layer_1,
    unsigned color_tex_id_2, int color_tex_layer_2,
    unsigned color_tex_id_3, int color_tex_layer_3,
    unsigned depth_tex_id_0, int depth_tex_layer_0)
{
  profB(__func__);
  unsigned fbo_id = 0;
  glCreateFramebuffers(1, &fbo_id);

  glNamedFramebufferTextureLayer(fbo_id, 36096 + 0, depth_tex_id_0, 0, depth_tex_layer_0);
  glNamedFramebufferTextureLayer(fbo_id, 36064 + 0, color_tex_id_0, 0, color_tex_layer_0);
  glNamedFramebufferTextureLayer(fbo_id, 36064 + 1, color_tex_id_1, 0, color_tex_layer_1);
  glNamedFramebufferTextureLayer(fbo_id, 36064 + 2, color_tex_id_2, 0, color_tex_layer_2);
  glNamedFramebufferTextureLayer(fbo_id, 36064 + 3, color_tex_id_3, 0, color_tex_layer_3);

  int attachments[4] = {
    color_tex_id_0 ? 36064 + 0 : 0,
    color_tex_id_1 ? 36064 + 1 : 0,
    color_tex_id_2 ? 36064 + 2 : 0,
    color_tex_id_3 ? 36064 + 3 : 0
  };

  glNamedFramebufferDrawBuffers(fbo_id, 4, attachments);
  profE(__func__);

  return fbo_id;
}

static inline unsigned GpuXfb(
    void * gpu_buf_ptr_0, int buf_0_bytes_first, int buf_0_bytes_count,
    void * gpu_buf_ptr_1, int buf_1_bytes_first, int buf_1_bytes_count,
    void * gpu_buf_ptr_2, int buf_2_bytes_first, int buf_2_bytes_count,
    void * gpu_buf_ptr_3, int buf_3_bytes_first, int buf_3_bytes_count)
{
  profB(__func__);
  unsigned xfb_id = 0;
  glCreateTransformFeedbacks(1, &xfb_id);

  unsigned buf_0_id = gpu_buf_ptr_0 ? ((unsigned *)gpu_buf_ptr_0)[-1] : 0;
  unsigned buf_1_id = gpu_buf_ptr_1 ? ((unsigned *)gpu_buf_ptr_1)[-1] : 0;
  unsigned buf_2_id = gpu_buf_ptr_2 ? ((unsigned *)gpu_buf_ptr_2)[-1] : 0;
  unsigned buf_3_id = gpu_buf_ptr_3 ? ((unsigned *)gpu_buf_ptr_3)[-1] : 0;

  if (buf_0_id) glTransformFeedbackBufferRange(xfb_id, 0, buf_0_id, 4 + buf_0_bytes_first, buf_0_bytes_count);
  if (buf_1_id) glTransformFeedbackBufferRange(xfb_id, 1, buf_1_id, 4 + buf_1_bytes_first, buf_1_bytes_count);
  if (buf_2_id) glTransformFeedbackBufferRange(xfb_id, 2, buf_2_id, 4 + buf_2_bytes_first, buf_2_bytes_count);
  if (buf_3_id) glTransformFeedbackBufferRange(xfb_id, 3, buf_3_id, 4 + buf_3_bytes_first, buf_3_bytes_count);
  profE(__func__);

  return xfb_id;
}

static inline void GpuBindFbo(unsigned fbo_id) {
  profB(__func__);
  glBindFramebuffer(36160, fbo_id);
  profE(__func__);
}

static inline void GpuBindXfb(unsigned xfb_id) {
  profB(__func__);
  glBindTransformFeedback(36386, xfb_id);
  profE(__func__);
}

static inline void GpuDraw(int gpu_op_count, struct gpu_op_t * gpu_op) {
  profB(__func__);
  for (int i = 0; i < gpu_op_count; i += 1) {
    struct gpu_op_t op = gpu_op[i];

    if (op.cmd == NULL) continue;

    profB("bind");
    if (op.tex != NULL) glBindTextures(op.tex_first, op.tex_count, op.tex);
    if (op.smp != NULL) glBindSamplers(op.smp_first, op.smp_count, op.smp);

    if (op.vert != 0) glProgramUniform1iv(op.vert, 0, 1, &op.index);
    if (op.frag != 0) glProgramUniform1iv(op.frag, 0, 1, &op.index);

    glBindProgramPipeline(op.ppo);
    profE("bind");

    profB("submit");
    unsigned mode = op.mode;
    for (int i = 0, cmd_count = op.cmd_count; i < cmd_count; i += 1) {
      struct gpu_cmd_t cmd = op.cmd[i];
      glDrawArraysInstanced(mode, cmd.first, cmd.count, cmd.instance_count);
    }
    profE("submit");
  }
  profE(__func__);
}

static inline void GpuDrawXfb(int gpu_op_count, struct gpu_op_t * gpu_op) {
  profB(__func__);
  for (int i = 0; i < gpu_op_count; i += 1) {
    struct gpu_op_t op = gpu_op[i];

    if (op.cmd == NULL) continue;

    profB("bind");
    if (op.tex != NULL) glBindTextures(op.tex_first, op.tex_count, op.tex);
    if (op.smp != NULL) glBindSamplers(op.smp_first, op.smp_count, op.smp);

    if (op.vert != 0) glProgramUniform1iv(op.vert, 0, 1, &op.index);
    if (op.frag != 0) glProgramUniform1iv(op.frag, 0, 1, &op.index);

    glBindProgramPipeline(op.ppo);
    profE("bind");

    profB("submit");
    glBeginTransformFeedback(op.mode);
    unsigned mode = op.mode;
    for (int i = 0, cmd_count = op.cmd_count; i < cmd_count; i += 1) {
      struct gpu_cmd_t cmd = op.cmd[i];
      glDrawArraysInstanced(mode, cmd.first, cmd.count, cmd.instance_count);
    }
    glEndTransformFeedback();
    profE("submit");
  }
  profE(__func__);
}

static inline void GpuBlit(
    unsigned source_fbo_id, int source_color_id, int source_x, int source_y, int source_width, int source_height,
    unsigned target_fbo_id, int target_color_id, int target_x, int target_y, int target_width, int target_height)
{
  profB(__func__);
  glNamedFramebufferReadBuffer(source_fbo_id, 36064 + source_color_id);
  glNamedFramebufferDrawBuffer(target_fbo_id, 36064 + target_color_id);

  glBlitNamedFramebuffer(
      source_fbo_id, target_fbo_id,
      source_x, source_y, source_width, source_height,
      target_x, target_y, target_width, target_height, 16384, 9728);
  profE(__func__);
}

static inline void GpuBlitToScreen(
    unsigned source_fbo_id, int source_color_id,
    int source_x, int source_y, int source_width, int source_height,
    int screen_x, int screen_y, int screen_width, int screen_height)
{
  profB(__func__);
  glNamedFramebufferReadBuffer(source_fbo_id, 36064 + source_color_id);
  glNamedFramebufferDrawBuffer(0, 1029);
  glBlitNamedFramebuffer(
      source_fbo_id, 0,
      source_x, source_y, source_width, source_height,
      screen_x, screen_y, screen_width, screen_height, 16384, 9728);
  profE(__func__);
}

static inline void GpuSwap(SDL_Window * sdl_window) {
  profB(__func__);
  SDL_GL_SwapWindow(sdl_window);
  glFinish();
  profE(__func__);
}

static inline void GpuClear() {
  profB(__func__);
  glClear(16640);
  profE(__func__);
}

static inline void GpuFinish() {
  profB(__func__);
  glFinish();
  profE(__func__);
}

static inline void GpuEnable(unsigned flags) {
  profB(__func__);
  glEnable(flags);
  profE(__func__);
}

static inline void GpuDisable(unsigned flags) {
  profB(__func__);
  glDisable(flags);
  profE(__func__);
}

static inline void GpuViewport(int x, int y, int width, int height) {
  profB(__func__);
  glViewport(x, y, width, height);
  profE(__func__);
}

#ifndef RELEASE

static inline void GpuSetDebugCallback(void * callback, void * user_data) {
  glDebugMessageCallback(callback, user_data);
}

static inline void GpuDebugCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, char * message, void * userdata) {
  char * GL_ERROR_SOURCE[] = {
    "API",
    "WINDOW SYSTEM",
    "SHADER COMPILER",
    "THIRD PARTY",
    "APPLICATION",
    "OTHER"
  };

  char * GL_ERROR_SEVERITY[] = {
    "HIGH",
    "MEDIUM",
    "LOW",
    "NOTIFICATION"
  };

  char * GL_ERROR_TYPE[] = {
    "ERROR",
    "DEPRECATED BEHAVIOR",
    "UNDEFINED DEHAVIOUR",
    "PORTABILITY",
    "PERFORMANCE",
    "OTHER"
  };

  SDL_Log(
    "\n" "OPENGL DEBUG"
    "\n" "ID:       %u"
    "\n" "SOURCE:   %s"
    "\n" "SEVERITY: %s"
    "\n" "TYPE:     %s"
    "\n" "MESSAGE:  %s"
    "\n", id,
    GL_ERROR_SOURCE[source - 0x8246], // GL_DEBUG_SOURCE_API
    GL_ERROR_SEVERITY[
      severity != 0x826B ? // GL_DEBUG_SEVERITY_NOTIFICATION
      severity  - 0x9146 : 3], // GL_DEBUG_SEVERITY_HIGH
    GL_ERROR_TYPE[type - 0x824C], // GL_DEBUG_TYPE_ERROR
    message);
}

#else // RELEASE

static inline void GpuSetDebugCallback(void *, void *) {}
static inline void GpuDebugCallback(unsigned, unsigned, unsigned, unsigned, int, char *, void *) {}

#endif // RELEASE