#version 410

layout( location = 0 ) in vec2 position;
layout( location = 1 ) in vec4 color;
layout( location = 2 ) in vec2 st;

out vec2 textureCoord;
smooth out vec4 textureColor;
smooth out vec2 blurOffset[14];

uniform mat4 ProjView;
uniform mat4 ModelMat;
uniform mat4 ViewMat;

void main()
{
   textureColor = color;
   textureCoord = st;

   vec4 pos = vec4( position.x, position.y, 1, 1 );

   gl_Position = ProjView * ModelMat * ViewMat * pos;
   

   blurOffset[0] = textureCoord + vec2( 0, -0.196 );
   blurOffset[1] = textureCoord + vec2( 0, -0.168 );
   blurOffset[2] = textureCoord + vec2( 0, -0.14 );
   blurOffset[3] = textureCoord + vec2( 0, -0.112 );
   blurOffset[4] = textureCoord + vec2( 0, -0.084 );
   blurOffset[5] = textureCoord + vec2( 0, -0.056 );
   blurOffset[6] = textureCoord + vec2( 0, -0.028 );
   blurOffset[7] = textureCoord + vec2( 0, 0.028 );
   blurOffset[8] = textureCoord + vec2( 0, 0.056 );
   blurOffset[9] = textureCoord + vec2( 0, 0.084 );
   blurOffset[10] = textureCoord + vec2( 0, 0.112 );
   blurOffset[11] = textureCoord + vec2( 0, 0.14 );
   blurOffset[12] = textureCoord + vec2( 0, 0.168 );
   blurOffset[13] = textureCoord + vec2( 0, 0.196 );


}
