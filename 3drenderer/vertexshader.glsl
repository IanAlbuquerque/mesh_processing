#version 460 core

layout( location = 0 ) in vec3 vertexPos;
layout( location = 1 ) in vec3 vertexNormal;
layout( location = 2 ) in int vertexIdx;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 mv_ti;

out vec3 vPos;
out vec3 vNormal;

void main()
{
    gl_Position = mvp * vec4( vertexPos, 1 );
    vPos = ( mv * vec4( vertexPos, 1 ) ).xyz;
    vNormal = ( mv_ti * vec4( vertexNormal, 0 ) ).xyz;
}
