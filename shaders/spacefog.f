#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;

uniform sampler2D texUnit;
uniform float STAdd;

out vec4 outputColor;


void main()
{

    float st = STAdd;
    vec2 tc = textureCoord + vec2( 0, st );
    
    if( textureCoord.y + st > 1 ) {
    
        tc.y = tc.y - 1;
    
    }

   outputColor = texture( texUnit, tc ) * textureColor;
   
}

