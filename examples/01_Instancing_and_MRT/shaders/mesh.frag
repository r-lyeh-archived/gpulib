#version 330
#extension GL_ARB_gpu_shader5               : enable
#extension GL_ARB_shader_precision          : enable
#extension GL_ARB_texture_cube_map_array    : enable
#extension GL_ARB_separate_shader_objects   : enable
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_shading_language_packing  : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform int index;
layout(location = 1) uniform vec3 cam_pos;
layout(location = 2) uniform int show_pass;
layout(location = 3) uniform int cube_index;

layout(binding = 2) uniform sampler2DArray s_texture;
layout(binding = 3) uniform samplerCubeArray s_cubemaps;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 fbo_color;

int ColorToInt(vec3 c) {
  int r = int(c.r * 255.0);
  int g = int(c.g * 255.0);
  int b = int(c.b * 255.0);
  return b + (g << 8) + (r << 16);
}

vec3 IntToColor(int i) {
  vec3 color;
  color.r = float(i >> 16 & 255) / 255.0;
  color.g = float(i >> 8  & 255) / 255.0;
  color.b = float(i       & 255) / 255.0;
  return color;
}

void main() {
  vec4 diff = texture(s_texture, vec3(uv, 0));
  vec4 refl = texture(s_cubemaps, vec4(reflect((pos - cam_pos), nor), cube_index));
  vec4 tint = vec4(0);

  if (index == 0) tint += vec4(1, 0, 0, 1);
  if (index == 1) tint += vec4(0, 1, 0, 1);
  if (index == 2) tint += vec4(0, 0, 1, 1);

  fbo_color = mix(diff * 0.4 + tint * 0.6, refl, dot(nor, normalize(pos - cam_pos)) * 0.5 + 0.5);

  if (show_pass == 1) fbo_color = diff;
  if (show_pass == 2) fbo_color = refl;
  if (show_pass == 3) fbo_color = tint;
  if (show_pass == 4) fbo_color = vec4(IntToColor(index + 1), 1);
  if (show_pass == 5) fbo_color = vec4(uv, 0, 1);
  if (show_pass == 6) fbo_color = vec4(nor, 1);
  if (show_pass == 7) fbo_color = vec4(pos, 1);
  if (show_pass == 8) fbo_color = vec4(vec3(gl_FragCoord.z), 1);
}