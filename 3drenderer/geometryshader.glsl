#version 150 compatibility

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vPos[];
in vec3 vNormal[];

out vec3 fragPos;
out vec3 fragNormal;

out vec3 vColor;

void main()
{

    gl_Position = gl_in[0].gl_Position;
    vColor = vec3(1, 0, 0);
    fragPos = vPos[0];
    fragNormal = vNormal[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    vColor = vec3(0, 1, 0);
    fragPos = vPos[1];
    fragNormal = vNormal[1];
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    vColor = vec3(0, 0, 1);
    fragPos = vPos[2];
    fragNormal = vNormal[2];
    EmitVertex();

    EndPrimitive();
}
