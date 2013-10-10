// provided by Qt
attribute highp vec4 qt_Vertex;
attribute highp vec4 qt_Normal;
uniform mediump mat4 qt_ModelViewProjectionMatrix;
attribute highp vec4 qt_MultiTexCoord0;
varying highp vec4 qt_TexCoord0;
uniform highp mat4 qt_WorldMatrix;

// must be set manually
uniform mediump float scaleX;
uniform mediump float scaleY;
uniform mediump float scaleZ;

// local variable
varying vec4 vertex;
varying vec3 worldNormOut;
varying vec3 worldPosOut;

void main()
{
        vertex.x = qt_Vertex.x * scaleX;
        vertex.y = qt_Vertex.y * scaleY;
        vertex.z = qt_Vertex.z * scaleZ;
        vertex.w = qt_Vertex.w;
        vec3 worldNorm = vec3( qt_WorldMatrix * qt_Normal );
        worldNormOut = normalize(worldNorm);
        vec3 worldPos = vec3( qt_WorldMatrix * qt_Vertex);
        worldPosOut = normalize(worldPos);
        gl_Position =  qt_ModelViewProjectionMatrix * vertex;
        qt_TexCoord0 = qt_MultiTexCoord0;
}
