#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangents;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

out vec3 WorldPos;
out vec3 Normal;
out vec3 Tangents;
out vec4 Color;
out vec2 TexCoords;
out mat3 TBN;

void main()
{
  TexCoords = aTexCoords; 
  Color = color;
  WorldPos = vec3(model * vec4(aPosition, 1.0));
  Normal = aNormal;
  Tangents = aTangents;

  vec3 T = normalize(vec3(model * vec4(aTangents, 0.0)));
  vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
  T = normalize(T - dot(T, N) * N);
  // then retrieve perpendicular vector B with the cross product of T and N
  vec3 B = cross(N, T);
  TBN = mat3(T, B, N);  

  gl_Position = projection * view * model * vec4(aPosition, 1.0f);

}