#version 330
#extension GL_ARB_gpu_shader5               : enable
#extension GL_ARB_shader_precision          : enable
#extension GL_ARB_texture_cube_map_array    : enable
#extension GL_ARB_separate_shader_objects   : enable
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_shading_language_packing  : enable
#extension GL_ARB_explicit_uniform_location : enable
out gl_PerVertex { vec4 gl_Position; };

layout(location = 0) uniform vec4 cam_rot;
layout(location = 1) uniform float fov_x;
layout(location = 2) uniform float fov_y;

layout(location = 0) out vec3 pos;

const vec3 cube[] = vec3[](
  vec3(-1, 1,-1), vec3(-1,-1,-1), vec3( 1,-1,-1),
  vec3( 1,-1,-1), vec3( 1, 1,-1), vec3(-1, 1,-1),
  vec3(-1,-1, 1), vec3(-1,-1,-1), vec3(-1, 1,-1),
  vec3(-1, 1,-1), vec3(-1, 1, 1), vec3(-1,-1, 1),
  vec3( 1,-1,-1), vec3( 1,-1, 1), vec3( 1, 1, 1),
  vec3( 1, 1, 1), vec3( 1, 1,-1), vec3( 1,-1,-1),
  vec3(-1,-1, 1), vec3(-1, 1, 1), vec3( 1, 1, 1),
  vec3( 1, 1, 1), vec3( 1,-1, 1), vec3(-1,-1, 1),
  vec3(-1, 1,-1), vec3( 1, 1,-1), vec3( 1, 1, 1),
  vec3( 1, 1, 1), vec3(-1, 1, 1), vec3(-1, 1,-1),
  vec3(-1,-1,-1), vec3(-1,-1, 1), vec3( 1,-1,-1),
  vec3( 1,-1,-1), vec3(-1,-1, 1), vec3( 1,-1, 1)
);

vec3 v3rot(vec3 p, vec4 v) {
  return fma(cross(v.xyz, fma(p, vec3(v.w), cross(v.xyz, p))), vec3(2), p);
}

vec4 verneg(vec4 v) {
  return vec4(-v.xyz, v.w);
}

void main() {
  pos = cube[gl_VertexID];

  vec3 mv = pos;
  mv = v3rot(mv, verneg(cam_rot));

  mv.x *= fov_x;
  mv.y *= fov_y;

  mv.z *= -1;

  gl_Position = vec4(mv, mv.z + 0.1);
}