#version 330

in vec4 glColor;
in vec2 textureCoord;

uniform sampler2D texUnit;

out vec4 outputColor;


void main()
{

    vec4 fragColor = texture( texUnit, textureCoord ) * glColor;
    float mul = 1;

    if( fragColor.x > .5 && fragColor.y > .5 && fragColor.z > .5 ) {

        mul = 2;

    }

    outputColor = fragColor * mul;

}

