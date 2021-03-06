#version 330
#extension GL_ARB_gpu_shader5               : enable
#extension GL_ARB_shader_precision          : enable
#extension GL_ARB_texture_cube_map_array    : enable
#extension GL_ARB_separate_shader_objects   : enable
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_shading_language_packing  : enable
#extension GL_ARB_explicit_uniform_location : enable
out gl_PerVertex { vec4 gl_Position; };

layout(location = 0) out vec2 uv;

const vec2 quad[] = vec2[](
  vec2(-1,-1), vec2( 1,-1), vec2(-1, 1),
  vec2(-1, 1), vec2( 1,-1), vec2( 1, 1)
);

const vec2 quad_uv[] = vec2[](
  vec2(0, 0), vec2(1, 0), vec2(0, 1),
  vec2(0, 1), vec2(1, 0), vec2(1, 1)
);

void main() {
  uv = quad_uv[gl_VertexID];
  gl_Position = vec4(quad[gl_VertexID], 0, 1);
}
