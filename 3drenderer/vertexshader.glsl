#version 330 core

layout( location = 0 ) in vec3 vertexPos;
layout( location = 1 ) in vec3 vertexNormal;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 mv_ti;

out vec3 fragPos;
out vec3 fragNormal;

void main()
{
    gl_Position = mvp * vec4( vertexPos, 1 );
    fragPos = ( mv * vec4( vertexPos, 1 ) ).xyz;
    fragNormal = ( mv_ti * vec4( vertexNormal, 0 ) ).xyz;
}
