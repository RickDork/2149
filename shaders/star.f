#version 330

in vec4 glColor;
in vec2 textureCoord;

uniform sampler2D texUnit;

out vec4 outputColor;


void main()
{

   outputColor = texture( texUnit, textureCoord ) * glColor;

}

