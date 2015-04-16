#version 330


layout( location = 0 ) in vec4 position;
layout( location = 1 ) in mat4 starPos;
layout( location = 5 ) in vec4 starColor;

out vec4 glColor;
out vec2 textureCoord;

uniform mat4 ProjView;
uniform mat4 ModelMat;
uniform mat4 ViewMat;

void main()
{

   textureCoord = position.xy;
   glColor = starColor;
   
   gl_Position = ProjView * starPos * ViewMat * position;

}

