#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;

uniform sampler2D texUnit;

out vec4 outputColor;

float PI = 3.14159;

float atan2(in float y, in float x)
{
    
    if( x == 0.0 ) 
        return sign(y)*PI/2.0;
        
    bool s = (abs(x) > abs(y));
    return mix(PI/2.0 - atan(x,y), atan(y,x), s);
    
}

void main()
{

    float RAD2DEG = 57.2957;
    float theta = atan2( textureCoord.y - .5, textureCoord.x - .5 );
    float centerDist = distance( textureCoord, vec2( .5, .5  ) );
    float distPercent = centerDist / .8;
        
    float range = 2.0 * PI;
    
    if( theta < 0.0 ) {
    
        theta = PI - abs( theta ) + PI;
    
    }
    
    float coordx = theta / range;
    float coordy = ( 1 - distPercent );
    
    outputColor = texture( texUnit, vec2( coordx, coordy ) );
    
}

