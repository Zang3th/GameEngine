#version 450 core
  
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D image;
uniform vec3 color;

void main()
{
    //fragColor = vec4(color, 1.0f) * texture(image, texCoords);
    float depthValue = texture(image, texCoords).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}