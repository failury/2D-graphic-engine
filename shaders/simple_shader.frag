# version 450

layout(location = 0 ) in vec3 fragColor;
layout(location = 0) out vec4 outColor; // set the custom return variable: outColor -> vec4
void main(){
    outColor = vec4(fragColor, 1.0); // RGB, alpa
}