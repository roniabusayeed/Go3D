#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoord;

out vec2 v_textureCoord;

void main()
{
    v_textureCoord = textureCoord;
    gl_Position = vec4(position, 1.0f, 1.0f);
}

#shader fragment
#version 330 core

in vec2 v_textureCoord;
out vec4 fragColor;

uniform sampler2D u_texture;

void main()
{
    fragColor = texture(u_texture, v_textureCoord);
}
