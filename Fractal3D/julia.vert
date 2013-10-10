/*attribute highp vec4 qt_Vertex;
attribute highp vec4 qt_MultiTexCoord0;
uniform highp mat4 qt_ModelViewProjectionMatrix;
varying highp vec4 qt_TexCoord0;

void main(void)
{
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
    qt_TexCoord0 = qt_MultiTexCoord0;
}*/

attribute highp vec4 qt_Vertex;
attribute highp vec4 qt_Normal;
uniform mediump mat4 qt_ModelViewProjectionMatrix;

varying vec3 vertex_color;

void main()
{
        gl_Position =  qt_ModelViewProjectionMatrix * qt_Vertex;
        vertex_color = qt_Vertex.xyz;
}
