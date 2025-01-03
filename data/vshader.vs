#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

out vec3 blend_color;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


void main(){
  gl_Position = projection * view * model * vec4(position, 1.0f);
  //gl_Position = vec4(position.x, position.y, position.z, 1.0);
  blend_color = normals;
}