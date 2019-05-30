#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D qt_Texture0;
varying vec2 qt_TexCoord0;

void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0);
}
