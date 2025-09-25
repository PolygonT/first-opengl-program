#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
// layout(location = 2) in vec4 color;

out vec2 v_TexCoord;
out vec3 v_Position;

uniform mat4 u_Mvp;

void main()
{
    gl_Position = u_Mvp * vec4(position, 1.0f);
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in vec3 v_Position;

// uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    // color = texColor;
    // color = u_Color;
    // color = texColor;
    color = texColor;
}

