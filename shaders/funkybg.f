#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;

uniform float funkFactor;
uniform float theta;
uniform sampler2D texUnit;
uniform sampler2D noiseImage;

out vec4 outputColor;


void main()
{

    float noiseColor = texture( noiseImage, textureCoord ).r;
    
    if( funkFactor >= noiseColor * 255 ) {

        float a = ( funkFactor - noiseColor * 255 ) / 20;
        
        if( a > 1 )
            a = 1;

        float centerDist = distance( textureCoord, vec2( .5, .5  ) );
        float distPercent = centerDist / .5;
        float angleFromCenter = ( atan( textureCoord.y - .5, textureCoord.x - .5 ) ) + theta * distPercent;
        float ntx = cos( angleFromCenter ) * centerDist;
        float nty = sin( angleFromCenter ) * centerDist;
        float nc = texture( noiseImage, vec2( ntx, nty ) ).r;

        outputColor = vec4( max( 1 - ntx, .5 ) * ( funkFactor / 300 ) * max( sin( theta ), .6 ) * .5 * ( 1 - nc ), ntx * nty * ( funkFactor / 300 ) * cos( theta ), nc * ( max( 1 - cos( theta ) * distPercent, .5 ) ) * .7, 1 );

    } else
        outputColor = texture( texUnit, textureCoord ) * textureColor;
   
}

