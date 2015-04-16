#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;
smooth in vec2 vPos;

uniform sampler2D texUnit;

out vec4 outputColor;

void main()
{

    float x = vPos.x * 2.5;
    float y = vPos.y * 2;
    float alpha = texture( texUnit, textureCoord ).w;
    
    float centerDist = distance( textureCoord, vec2( .5, .5  ) );
    
    if( centerDist > .45 ) {
        
        outputColor = vec4( textureCoord.x, textureCoord.y, ( 1 - textureCoord.y + y ) * ( 1 - textureCoord.x + x ), alpha );


    } else {

        outputColor = vec4( textureCoord.x + x, textureCoord.y + y, ( 1 - textureCoord.y + y ) * ( 1 - textureCoord.x + x ), alpha );

        if( outputColor.x < .2 )
            outputColor.x = .2 + ( .2 - outputColor.x );

        if( outputColor.y < .2 )
            outputColor.y = .2 + ( .2 - outputColor.y );


        if( outputColor.z < .2 )
            outputColor.z = .2 + ( .2 - outputColor.z );

        if( outputColor.x > 1 )
            outputColor.x = 1 - ( outputColor.x - 1 );

        if( outputColor.y > 1 )
            outputColor.y = 1 - ( outputColor.y - 1 );

        if( outputColor.z > 1 )
            outputColor.z = 1 - ( outputColor.z - 1 );
            
    }

}

