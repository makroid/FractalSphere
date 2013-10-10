#version 400

attribute highp vec4 qt_Vertex;
attribute highp vec4 qt_Normal;

//out vec3 ReflectionDir;
//out vec3 RefractionDir;
out vec3 worldCameraPositionOut;
out vec3 worldPosOut;
out vec3 worldNormOut;
out vec3 worldViewOut;
out vec3 position;
out vec3 normal;
out vec3 ReflectionDir;

uniform vec3 WorldCameraPosition;

uniform highp mat4 qt_ModelViewProjectionMatrix;
uniform highp mat4 qt_WorldMatrix;


void main() {
    vec3 worldPos = vec3( qt_WorldMatrix * qt_Vertex );
    vec3 worldNorm = vec3( qt_WorldMatrix * qt_Normal );
    vec3 worldView = normalize( WorldCameraPosition-worldPos );

    worldCameraPositionOut = WorldCameraPosition;
    worldPosOut = worldPos;
    worldNormOut = normalize(worldNorm);
    worldViewOut = worldView;


    position = qt_Vertex.xyz;
    normal = qt_Normal.xyz;

    ReflectionDir = reflect(-worldView, worldNorm);
    //RefractionDir = refract(worldView, worldNorm, 1.1).rgb;

    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
}
/*
attribute highp vec4 qt_Vertex;

uniform highp mat4 qt_ModelViewProjectionMatrix;
//uniform highp mat4 qt_WorldMatrix;

//varying vec3 WorldCameraPosition;

void main() {
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
}*/
