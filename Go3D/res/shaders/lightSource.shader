#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoord;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
}

#shader fragment
#version 330 core

out vec4 fragColor;
void main()
{
    fragColor = vec4(0.9f, 0.9f, 1.0f, 1.0f);
}