#version 410


in vec2 textureCoord;
smooth in vec4 textureColor;
smooth in vec2 blurOffset[14];

uniform sampler2D texUnit;
uniform sampler2D texUnit2;

out vec4 outputColor;

#define PRECISION 0.000001


float is_zero( float val ) {

    return step(-PRECISION, val) * (1.0 - step(PRECISION, val));

}

void main()
{
   outputColor = texture( texUnit, textureCoord ) * textureColor;

   float mul = 1.5;
   
    if( outputColor.a < .1 ) {
   
        mul = .6;
   
    }

    outputColor += texture( texUnit2, textureCoord ) * mul * 0.159576912161;
    outputColor += texture( texUnit2, blurOffset[0] ) * mul * 0.0044299121055113265;
    outputColor += texture( texUnit2, blurOffset[1] ) * mul * 0.00895781211794;
    outputColor += texture( texUnit2, blurOffset[2] ) * mul * 0.0215963866053;
    outputColor += texture( texUnit2, blurOffset[3] ) * mul * 0.0443683338718;
    outputColor += texture( texUnit2, blurOffset[4] ) * mul * 0.0776744219933;
    outputColor += texture( texUnit2, blurOffset[5] ) * mul * 0.115876621105;
    outputColor += texture( texUnit2, blurOffset[6] ) * mul * 0.147308056121;
    outputColor += texture( texUnit2, blurOffset[7] ) * mul * 0.147308056121;
    outputColor += texture( texUnit2, blurOffset[8] ) * mul * 0.115876621105;
    outputColor += texture( texUnit2, blurOffset[9] ) * mul * 0.0776744219933;
    outputColor += texture( texUnit2, blurOffset[10] ) * mul * 0.0443683338718;
    outputColor += texture( texUnit2, blurOffset[11] ) * mul * 0.0215963866053;
    outputColor += texture( texUnit2, blurOffset[12] ) * mul * 0.00895781211794;
    outputColor += texture( texUnit2, blurOffset[13] ) * mul * 0.0044299121055113265;

}

