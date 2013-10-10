#version 400

// provided by qt
struct qt_MaterialParameters {
     mediump vec4 emission;
     mediump vec4 ambient;
     mediump vec4 diffuse;
     mediump vec4 specular;
     mediump float shininess;
};
uniform qt_MaterialParameters qt_Material;

struct qt_SingleLightParameters {
     mediump vec4 position;
     mediump vec3 spotDirection;
     mediump float spotExponent;
     mediump float spotCutoff;
     mediump float spotCosCutoff;
     mediump float constantAttenuation;
     mediump float linearAttenuation;
     mediump float quadraticAttenuation;
};
uniform qt_SingleLightParameters qt_Light;


attribute highp vec4 qt_Vertex;
attribute highp vec4 qt_Normal;

uniform highp mat4 qt_WorldMatrix;
uniform highp mat4 qt_ModelViewMatrix;
uniform highp mat4 qt_NormalMatrix;

// from vertex shader
//in vec3 ReflectionDir;
in vec3 worldCameraPositionOut;
in vec3 worldPosOut;
in vec3 worldNormOut;
in vec3 worldViewOut;
in vec3 position;
in vec3 normal;
in vec3 ReflectionDir;

// must be set manually
uniform samplerCube cubeMapTex;
uniform float ReflectFactor;
uniform vec4 MaterialColor;
//uniform vec3 WorldCameraPosition;

layout(location = 0) out vec4 FragColor;

float coeffs(int i)
{
        return 1.0 / (3.0 + 0.1 * float(i));
}


void main() {


    //vec3 N = normalize(normal);
/*    vec3 I = -normalize(position);
    mat3 V = mat3(qt_ModelViewMatrix[0].xyz, qt_ModelViewMatrix[1].xyz, qt_ModelViewMatrix[2].xyz);
    float IdotN = dot(I, N);
    float scales[6];
    vec3 C[6];
    for (int i = 0; i < 6; ++i) {
        scales[i] = (IdotN - sqrt(1.0 - coeffs(i) + coeffs(i) * (IdotN * IdotN)));
        C[i] = texture(cubeMapTex, (-I + coeffs(i) * N) * V).xyz;
    }
    vec4 refractedColor = 0.25 * vec4(C[5].x + 2.0*C[0].x + C[1].x, C[1].y + 2.0*C[2].y + C[3].y,
                          C[3].z + 2.0*C[4].z + C[5].z, 4.0);

    vec3 R = 2.0 * dot(-position, N) * N + position;
    vec4 reflectedColor = texture(cubeMapTex, R * V);

    FragColor = mix(refractedColor, reflectedColor, 0.4 + 0.6 * pow(1.0 - IdotN, 2.0));*/

    vec3 worldPos = vec3( qt_WorldMatrix * qt_Vertex );
    vec3 worldNorm = vec3( qt_NormalMatrix * qt_Normal );
    //vec3 worldView = normalize( worldCameraPosition - worldPos );
    vec3 L = normalize(qt_Light.position.xyz - worldPos);
    vec3 eyeRay = normalize(worldPos.xyz - worldCameraPositionOut);

    vec3 diffuseColor = MaterialColor.rgb * max(0.0, dot(worldNormOut.xyz, L));

    //vec3 ReflectionDir = reflect(-worldViewOut, worldNormOut).xyz;

    vec3 reflectionColor = texture(cubeMapTex, ReflectionDir).rgb;
  //  vec3 solidColor = mix(diffuseColor, reflectionColor, 0.5);

    //FragColor = vec4(solidColor, 1.0f);
    FragColor = mix(vec4(reflectionColor, 1.0), vec4(diffuseColor,1.0), 1.0);
    //FragColor = mix(vec4(solidColor,1.0), vec4(reflectionColor, 1.0), 0.5);

}
