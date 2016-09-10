#ifdef OPENGL_ES
precision mediump float;
#endif

uniform sampler2D u_texture;
// Varyings
//varying vec2 v_texCoord;
#ifdef GL_ES
varying mediump vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

void main()
{
	gl_FragColor = texture2D( u_texture, v_texCoord );
	
}