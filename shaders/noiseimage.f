#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;

uniform sampler2D texUnit;

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

    float centerDist = distance( textureCoord, vec2( .5, .5  ) );
    float distPercent = centerDist / 3.5;
    float rnum = 40.0 * rand( textureCoord );
    float min = distPercent * 255.0;
    float c = ( min + rnum ) / 255.0;
    
    outputColor = vec4( c, c, c, 1 );

}

