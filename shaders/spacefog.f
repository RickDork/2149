#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;

uniform sampler2D texUnit;
uniform sampler2D texUnit2;
uniform float STAdd;

out vec4 outputColor;


#define PRECISION 0.000001


float is_zero( float val ) {

    return step(-PRECISION, val) * (1.0 - step(PRECISION, val));

}


void main()
{

    float st = STAdd;
    vec2 tc = textureCoord + vec2( 0, st );
    
    if( textureCoord.y + st > 1 ) {
    
        tc.y = tc.y - 1;
    
    }

   outputColor = texture( texUnit, tc ) * textureColor * vec4( 1, 1, 1.15, 1 );
   
   vec2 expCoord = ceil( textureCoord * 50 ) / 50;
   
   vec4 expColor = texture( texUnit2, expCoord );
   
   if( is_zero( expColor.a ) != 1 ) {
   
        outputColor *= vec4( 1.25, 1.25, 3, 1 );
        
   }
   
}

