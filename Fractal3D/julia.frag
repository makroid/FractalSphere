/*uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}*/

uniform sampler2D qt_Texture0
attribute highp vec4 qt_Vertex;
//uniform vec2 center;
//uniform float scale;
//uniform int iter;

void main() {
    vec2 z, c, t;

    c.x = 1.3333 * (qt_Vertex[0] - 0.5) * 2.2 - 0;
    c.y = (qt_Vertex[1] - 0.5) * 2.2 - 0;

    int i;
    z = c;
    for(i=0; i<50; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }
    t.x = 0;
    t.y = (50==i ? 0.0 : float(i)) / 100.0;
    gl_FragColor = texture2D(qt_Texture0, t);
//	gl_FragColor = texture1D(tex, (i == iter ? 0.0 : float(i)) / 100.0);
}

/*varying vec3 vertex_color;
void main()
{
    gl_FragColor = vec4(vertex_color,1.0);
}*/
