#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;

uniform sampler2D texUnit;

out vec4 outputColor;


void main()
{

   outputColor = vec4( 1, 1, 1, texture( texUnit, textureCoord ).w );
   
}

