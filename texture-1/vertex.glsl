attribute vec2 position;
attribute vec2 qt_MultiTexCoord0;
varying vec2 qt_TexCoord0;

void main(void)
{
    gl_Position = vec4(position, 0.0, 1.0);
    qt_TexCoord0 = qt_MultiTexCoord0;
}
