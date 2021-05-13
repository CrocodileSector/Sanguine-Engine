#type vertex
#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec3 u_LightPosition; // we now define the uniform in the vertex shader and pass the 'view space' lightpos to the fragment shader. lightPos is currently in world space.

out vec3 v_Normal;
out vec2 v_TexCoords;
out vec3 v_FragPosition;
out vec3 v_LightPosition;

void main()
{
    vec4 vertexPosition = vec4(v_position, 1.0);

    v_TexCoords = v_texcoords;
    v_Normal = mat3(transpose(inverse(u_View * u_Model))) * v_normal;
    v_FragPosition = vec3(u_View * u_Model * vertexPosition);
    v_LightPosition = vec3(u_View * vec4(u_LightPosition, 1.0)); // Transform world-space light position to view-space light position

    gl_Position = u_Projection * u_View * u_Model * vertexPosition;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 Color;

in vec3 v_Normal;
in vec2 v_TexCoords;
in vec3 v_FragPosition;
in vec3 v_LightPosition;

uniform float u_AmbientK;
uniform float u_SpecularK;

uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;

uniform sampler2D u_Texture;

void main()
{
    // ambient
    vec3 ambient = u_AmbientK * u_LightColor;

    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(v_LightPosition - v_FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // specular
    vec3 viewDir = normalize(-v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = u_SpecularK * spec * u_LightColor;

    vec3 shade = (ambient + diffuse + specular) * u_ObjectColor;
    vec4 texColor = texture2D(u_Texture, v_TexCoords);

    Color = texColor + vec4(shade, 1.0);
}