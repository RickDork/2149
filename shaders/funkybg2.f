#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;

uniform float funkFactor;
uniform float theta;
uniform sampler2D texUnit;
uniform sampler2D noiseImage;
uniform sampler2D scientistImage;

out vec4 outputColor;

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}


void main()
{

    float ty = .5 * sin( 4 * textureCoord.x + theta );
    vec2 newTexCoord = textureCoord + vec2( 0, ty ) * .25;
    float noiseColor = texture( noiseImage, newTexCoord ).r;
    float inverseFunk = 150 - funkFactor;
    
    float sftx1 = .15;
    float sfty1 = .1;
    
    float sftx2 = .85;
    float sfty2 = .9;
    
    float xrange = sftx2 - sftx1;
    float yrange = sfty2 - sfty1;
    
    float curx = newTexCoord.x - sftx1;
    float cury = newTexCoord.y - sfty1;
    
    vec2 newTexCoord2 = vec2( curx / xrange, cury / yrange );
    
    float centerDist = distance( textureCoord, vec2( .5, .5  ) );
    float distPercent = centerDist / 3.5;

    if( inverseFunk <= noiseColor * 255 ) {

        outputColor = vec4( noiseColor + centerDist * theta, 1 - noiseColor * centerDist * 3 * theta, noiseColor * .5, 1 );
        outputColor.w = 1;

        if( curx > 0 && cury > 0 && sftx2 >= newTexCoord.x && sfty2 >= newTexCoord.y ) {

            vec4 texColor = texture( scientistImage, newTexCoord2 );
            
            if( texColor.a > 0 ) {
            
                texColor = texColor * 1.5;
                
                if( texColor.g >= 1.4 ) { 

                    outputColor = vec4( ty * 1.5, 1 - ty * 1.5, newTexCoord.x * newTexCoord.y, 1 );

                } else {
                
                    outputColor.r = ( texColor.r + outputColor.r ) * .75;
                    outputColor.g = ( texColor.g + outputColor.g ) * .5;
                    outputColor.b = ( texColor.b + outputColor.b ) * .75;
                    outputColor.a = ( texColor.a + outputColor.a ) * .5;
                    
                }
                
            }
            
        }

    } else
        outputColor = vec4( 0, 0, 0, 0 );
   
}

