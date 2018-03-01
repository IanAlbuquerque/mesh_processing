#version 460 core

struct Light
{
    vec3 position; //No espaço da câmera
};

uniform Light light;
uniform bool hasWireframe;
uniform bool hasOcclusion;

in vec3 vColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec3 fragCoords;

out vec3 finalColor;

void main()
{
    if(hasWireframe && (vColor.x < 0.01 || vColor.y < 0.01 || vColor.z < 0.01))
    {
        finalColor = vec3(1, 1, 1);
        return;
    }
    if(hasWireframe && !hasOcclusion)
    {
        discard;
        return;
    }

    vec3 materialDiffuse = vec3(1.0, 0.0, 0.0);
    vec3 materialSpecular = vec3(1.0, 1.0, 1.0);

    float materialShininess = 24.0;

    vec3 ambient = vec3(0.1, 0.1, 0.1);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);

    vec3 N = normalize(fragNormal);
    vec3 L = normalize(light.position - fragPos);

    float iDif = dot(L,N);

    if( iDif > 0 )
    {
        diffuse = iDif * materialDiffuse;

        vec3 V = normalize(-fragPos);
        vec3 H = normalize(L + V);

        float iSpec = pow(max(dot(N,H),0.0), materialShininess);
        specular = iSpec * materialSpecular;
    }

    finalColor = ambient + diffuse + specular;
}
