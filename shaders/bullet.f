#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;
uniform sampler2D texUnit;

out vec4 outputColor;


void main()
{

   float cosineVal = cos( textureCoord.y * 360 );
   
   outputColor = texture( texUnit, textureCoord + vec2( cosineVal * .001, 0 ) ) * textureColor;
   
}

