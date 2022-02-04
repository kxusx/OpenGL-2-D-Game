#version 330 core
in vec2 TexCoords;

in vec3 playerPosition;
out vec4 color;

uniform int light;
uniform vec3 lightPosition;
uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{    
    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
    if(light==0)
    {
        float dist = length(lightPosition-playerPosition);
        if(dist>100.0f){
            color = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }

}  