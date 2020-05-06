vec4 mengerSponge(vec3 p, int iterations) {
    float d = sdBox(p, vec3(1.0));
    vec4 res = vec4(d, 1.0, 0.0, 0.0);

    float s = 1.0;
    for (int m = 0; m < iterations; m++) {
        const vec3 a = mod(p*s, 2.0)-1.0;
        s *= 3.0;
        const vec3 r = abs(1.0 - 3.0 * abs(a));
        const float da = max(r.x, r.y);
        const float db = max(r.y, r.z);
        const float dc = max(r.z, r.x);
        const float c = (min(da, min(db, dc)) - 1.0) / s;
        if (c > d) {
            d = c;
            res = vec4(d, min(res.y, 0.2 * da * db * dc), (1.0 + float(m)) / 4.0, 0.0);
        }
    }
    return res;
}



    #define COLOR_FRACT_SCALE 2.8
    #define COLOR_FRACT_MINRAD2 .25
float colorFract_minRad2 = clamp(COLOR_FRACT_MINRAD2, 1.0e-9, 1.0);
#define colorFract_scale (vec4(COLOR_FRACT_SCALE, COLOR_FRACT_SCALE, COLOR_FRACT_SCALE, abs(COLOR_FRACT_SCALE)) / colorFract_minRad2)
float colorFract_absScalem1 = abs(COLOR_FRACT_SCALE - 1.0);
float colorFract_AbsScaleRaisedTo1mIters = pow(abs(COLOR_FRACT_SCALE), float(1-10));
//----------------------------------------------------------------------------------------
float colorFract(vec3 pos)
{

    vec4 p = vec4(pos, 1);
    vec4 p0 = p;// p.w is the distance estimate

    for (int i = 0; i < 7; i++)
    {
        p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0 - p.xyz;

        float r2 = dot(p.xyz, p.xyz);
        p *= clamp(max(colorFract_minRad2/r2, colorFract_minRad2), 0.0, 1.0);

        // scale, translate
        p = p*colorFract_scale + p0;
    }
    return ((length(p.xyz) - colorFract_absScalem1) / p.w - colorFract_AbsScaleRaisedTo1mIters);
}

vec3 colorFract_surfaceColour1 = vec3(.8, .0, 0.);
vec3 colorFract_surfaceColour2 = vec3(.4, .4, 0.5);
vec3 colorFract_surfaceColour3 = vec3(.5, 0.3, 0.00);
vec3 colorFractColor(vec3 pos, float sphereR)
{
    vec3 p = pos;
    vec3 p0 = p;
    float trap = 1.0;

    for (int i = 0; i < 6; i++)
    {

        p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0 - p.xyz;
        float r2 = dot(p.xyz, p.xyz);
        p *= clamp(max(colorFract_minRad2/r2, colorFract_minRad2), 0.0, 1.0);

        p = p*colorFract_scale.xyz + p0.xyz;
        trap = min(trap, r2);
    }
    // |c.x|: log final distance (fractional iteration count)
    // |c.y|: spherical orbit trap at (0,0,0)
    vec2 c = clamp(vec2(0.3333*log(dot(p, p))-1.0, sqrt(trap)), 0.0, 1.0);

    float t = mod(length(pos), 16.0);
    colorFract_surfaceColour1 = mix(colorFract_surfaceColour1, vec3(.4, 3.0, 5.), pow(smoothstep(0.0, .3, t) * smoothstep(0.6, .3, t), 10.0));
    return mix(mix(colorFract_surfaceColour1, colorFract_surfaceColour2, c.y), colorFract_surfaceColour3, c.x);
}

const vec3 sierpinski_va = vec3(0.0, 0.57735, 0.0);
const vec3 sierpinski_vb = vec3(0.0, -1.0, 1.15470);
const vec3 sierpinski_vc = vec3(1.0, -1.0, -0.57735);
const vec3 sierpinski_vd = vec3(-1.0, -1.0, -0.57735);

vec2 sierpinski(vec3 p)
{
    float a = 0.0;
    float s = 1.0;
    float r = 1.0;
    float dm;
    vec3 v;
    for (int i=0; i<12; i++)
    {
        float d, t;
        d = dot(p-sierpinski_va, p-sierpinski_va);              v=sierpinski_va; dm=d; t=0.0;
        d = dot(p-sierpinski_vb, p-sierpinski_vb); if (d<dm) { v=sierpinski_vb; dm=d; t=1.0; }
        d = dot(p-sierpinski_vc, p-sierpinski_vc); if (d<dm) { v=sierpinski_vc; dm=d; t=2.0; }
        d = dot(p-sierpinski_vd, p-sierpinski_vd); if (d<dm) { v=sierpinski_vd; dm=d; t=3.0; }
        p = v + 2.0*(p - v); r*= 2.0;
        a = t + 4.0*a; s*= 4.0;
    }

    return vec2((sqrt(dm)-1.0)/r, a/s);
}


vec4 apollonian_orb;

float apollonian(vec3 p, float s)
{
    float scale = 1.0;

    apollonian_orb = vec4(1000.0);

    for (int i=0; i<8;i++)
    {
        p = -1.0 + 2.0*fract(0.5*p+0.5);

        float r2 = dot(p, p);

        apollonian_orb = min(apollonian_orb, vec4(abs(p), r2));

        float k = s/r2;
        p     *= k;
        scale *= k;
    }

    return 0.25*abs(p.y)/scale;
}