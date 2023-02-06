#version 450
// input from vertex buffer
layout(location = 0) in vec2 position;

void main(){
    gl_Position = vec4(position, 0.0, 1.0); 
    // last 2: z value  and normalize vale
}