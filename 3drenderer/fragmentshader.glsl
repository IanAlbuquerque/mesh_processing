#version 330 core

struct Light
{
    vec3 position; //No espaço da câmera
};

uniform Light light;

in vec3 fragPos;
in vec3 fragNormal;

out vec3 finalColor;

void main()
{
    vec3 materialDiffuse = vec3(1.0, 0.0, 0.0);
    vec3 materialSpecular = vec3(1.0, 0.0, 0.0);
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
