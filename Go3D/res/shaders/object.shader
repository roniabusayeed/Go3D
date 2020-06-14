#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 v_normal;
out vec3 v_fragPosition;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    mat4 modelView = u_model;   // Multiply with u_view too if lighting calculations
                                // is done in eye space.
    v_normal = vec3(transpose(inverse(modelView)) * vec4(normal, 0.0f));

    v_fragPosition = vec3(u_model * vec4(position, 1.0f));
    // We are doing the lighting calculations in the world space coordinates.
    // So we need the world space coordinates. The way we get that is just multiplying
    // with model matrix.

    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
}



#shader fragment
#version 330 core

out vec4 fragColor;

in vec3 v_normal;
in vec3 v_fragPosition;
uniform vec3 u_lightSourcePosition;
uniform vec3 u_cameraPosition;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;

void main()
{
    // Ambient lighting calculations.
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * u_lightColor;

    // Diffuse lighting calculations.
    vec3 lightRayDirection = normalize(u_lightSourcePosition - v_fragPosition);
    float diff = max(0.0f, dot(lightRayDirection, normalize(v_normal)));
    vec3 diffuse = diff * u_lightColor;

    // Specular lighting calculations.
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(u_cameraPosition - v_fragPosition);
    vec3 reflectionDir = reflect(-lightRayDirection, normalize(v_normal)); 
    
    // Negating because reflect function expects the light ray direction to be from the light source
    // to the object. But for our calculations we did it the other way around. Thus the negation.

    float spec = pow(max(0.0f, dot(viewDir, reflectionDir)), 32);   // 32 is the value of shininess.
    vec3 specular = specularStrength * spec * u_lightColor;
                                                                  


    vec3 result = u_objectColor * (ambient + diffuse + specular);
    fragColor = vec4(result, 1.0f);
}
