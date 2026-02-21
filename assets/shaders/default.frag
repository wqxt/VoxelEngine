#version 460 core

in vec3 vertexColor;
in vec2 vertexUV;
uniform float u_flatShade;
out vec4 FragColor;


void main(){

   float t = (vertexUV.x + vertexUV.y) * 0.5; // Calculate a simple gradient based on UV coordinates
   vec3 color = u_flatShade > 0.5 ? vertexColor : vertexColor * t; // Use flat color or gradient based on uniform
   FragColor = vec4(color, 1.0);

}
