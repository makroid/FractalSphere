/*
uniform sampler1D tex;
varying vec3 vertex_color;
uniform vec4 qt_Vertex;
varying vec4 qt_TexCoord0;
uniform vec2 center;
uniform float scale;
uniform int iter;

void main() {
    vec2 z, c;

    c.x = 1.3333 * (qt_TexCoord0.x - 0.5) * scale - center.x;
    c.y = (qt_TexCoord0.y - 0.5) * scale - center.y;

    int i;
    z = c;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }
    gl_FragColor = texture1D(tex, (i==iter ? 0.1 : float(i)/100.0));
}*/

uniform vec4 qt_Color;
uniform sampler2D qt_Texture0;
uniform vec2 c;
uniform int iter;
varying vec4 qt_TexCoord0;

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

varying vec3 worldNormOut;
varying vec3 worldPosOut;

void main() {
        vec2 z, t;
        z.x = 3.0 * (qt_TexCoord0.x - 0.5);
        z.y = 2.0 * (qt_TexCoord0.y - 0.5);

        int i;
        for(i=0; i<iter; i++) {
                float x = (z.x * z.x - z.y * z.y) + c.x; //c.x
                float y = (z.y * z.x + z.x * z.y) + c.y; //c.y

                if((x * x + y * y) > 4.0) break;
                z.x = x;
                z.y = y;
        }
        t.x = (i == iter ? 0.0 : float(i)) / 50.0;
        t.y = 0.5;
        vec3 L = normalize(qt_Light.position.xyz - worldPosOut);
        vec4 pixel_color = vec4(qt_Color.a) * texture2D(qt_Texture0, t) + vec4(1.0-qt_Color.a)*qt_Color;
        gl_FragColor = pixel_color * max(0.0, dot(worldNormOut.xyz, -L));
}
