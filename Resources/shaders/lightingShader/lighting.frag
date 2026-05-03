#version 450

const int NR_LIGHTS = 32;
struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
    float Radius;
};

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    if (Normal == vec3(0.0, 0.0, 0.0)) { discard; }

    // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // calculate distance between light source and current fragment
        float fragDistance = length(lights[i].Position - FragPos);
        if(fragDistance < lights[i].Radius)
        {
            // diffuse
            vec3 lightDir = normalize(lights[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = lights[i].Color * spec * Specular;
            // attenuation
            float attenuation = 1.0 / (1.0 + lights[i].Linear * fragDistance + lights[i].Quadratic * fragDistance * fragDistance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }    
    FragColor = vec4(lighting, 1.0);
}