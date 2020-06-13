#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoord;

out vec2 v_textureCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    v_textureCoord = textureCoord;
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
}

#shader fragment
#version 330 core

in vec2 v_textureCoord;
out vec4 fragColor;

uniform sampler2D u_texture;

void main()
{
    // fragColor = texture(u_texture, v_textureCoord);
    fragColor = vec4(1.0f, 1.0f, 5.0f, 1.0f);
}
