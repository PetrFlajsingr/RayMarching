const float FLT_MAX = intBitsToFloat(2139095039);

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float logNormalize(float enteredValue, float minEntry, float maxEntry, float normalizedMin, float normalizedMax) {
    const float mx = (log((enteredValue-minEntry))/(log(maxEntry-minEntry)));
    const float preshiftNormalized = mx*(normalizedMax-normalizedMin);
    const float shiftedNormalized = preshiftNormalized + normalizedMin;
    return shiftedNormalized;
}

vec4 blend(vec4 fg, vec4 bg) {
    float a = 1.- fg.a;
    return fg + bg * a;
}


float hash(float n) {
    return fract(sin(n)*687.3123);
}

float noise(in vec2 x) {
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*157.0;
    return mix(mix(hash(n+  0.0), hash(n+  1.0), f.x),
    mix(hash(n+157.0), hash(n+158.0), f.x), f.y);
}

float noise(vec3 p) {
    vec3 ip=floor(p);
    p-=ip;
    vec3 s=vec3(7, 157, 113);
    vec4 h=vec4(0., s.yz, s.y+s.z)+dot(ip, s);
    p=p*p*(3.-2.*p);
    h=mix(fract(sin(h)*43758.5), fract(sin(h+s.x)*43758.5), p.x);
    h.xy=mix(h.xz, h.yw, p.y);
    return mix(h.x, h.y, p.z);
}

float noise(vec3 p, float time) {
    return noise(p + 0.2 * time);
}


vec4 permute(vec4 x){ return mod(((x*34.0)+1.0)*x, 289.0); }
vec4 taylorInvSqrt(vec4 r){ return 1.79284291400159 - 0.85373472095314 * r; }
vec3 fade(vec3 t) { return t*t*t*(t*(t*6.0-15.0)+10.0); }

float cnoise(vec3 P){
    vec3 Pi0 = floor(P);// Integer part for indexing
    vec3 Pi1 = Pi0 + vec3(1.0);// Integer part + 1
    Pi0 = mod(Pi0, 289.0);
    Pi1 = mod(Pi1, 289.0);
    vec3 Pf0 = fract(P);// Fractional part for interpolation
    vec3 Pf1 = Pf0 - vec3(1.0);// Fractional part - 1.0
    vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    vec4 iy = vec4(Pi0.yy, Pi1.yy);
    vec4 iz0 = Pi0.zzzz;
    vec4 iz1 = Pi1.zzzz;

    vec4 ixy = permute(permute(ix) + iy);
    vec4 ixy0 = permute(ixy + iz0);
    vec4 ixy1 = permute(ixy + iz1);

    vec4 gx0 = ixy0 / 7.0;
    vec4 gy0 = fract(floor(gx0) / 7.0) - 0.5;
    gx0 = fract(gx0);
    vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
    vec4 sz0 = step(gz0, vec4(0.0));
    gx0 -= sz0 * (step(0.0, gx0) - 0.5);
    gy0 -= sz0 * (step(0.0, gy0) - 0.5);

    vec4 gx1 = ixy1 / 7.0;
    vec4 gy1 = fract(floor(gx1) / 7.0) - 0.5;
    gx1 = fract(gx1);
    vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
    vec4 sz1 = step(gz1, vec4(0.0));
    gx1 -= sz1 * (step(0.0, gx1) - 0.5);
    gy1 -= sz1 * (step(0.0, gy1) - 0.5);

    vec3 g000 = vec3(gx0.x, gy0.x, gz0.x);
    vec3 g100 = vec3(gx0.y, gy0.y, gz0.y);
    vec3 g010 = vec3(gx0.z, gy0.z, gz0.z);
    vec3 g110 = vec3(gx0.w, gy0.w, gz0.w);
    vec3 g001 = vec3(gx1.x, gy1.x, gz1.x);
    vec3 g101 = vec3(gx1.y, gy1.y, gz1.y);
    vec3 g011 = vec3(gx1.z, gy1.z, gz1.z);
    vec3 g111 = vec3(gx1.w, gy1.w, gz1.w);

    vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
    g000 *= norm0.x;
    g010 *= norm0.y;
    g100 *= norm0.z;
    g110 *= norm0.w;
    vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
    g001 *= norm1.x;
    g011 *= norm1.y;
    g101 *= norm1.z;
    g111 *= norm1.w;

    float n000 = dot(g000, Pf0);
    float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
    float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
    float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
    float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
    float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
    float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
    float n111 = dot(g111, Pf1);

    vec3 fade_xyz = fade(Pf0);
    vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
    vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
    float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
    return 2.2 * n_xyz;
}


    #define MOD2 vec2(.16632, .17369)
    #define MOD3 vec3(.16532, .17369, .15787)
float tri(in float x){ return abs(fract(x)-.5); }

float hash12(vec2 p)
{
    p  = fract(p * MOD2);
    p += dot(p.xy, p.yx+19.19);
    return fract(p.x * p.y);
}
float vine(vec3 p, in float c, in float h)
{
    p.y += sin(p.z*.5625+1.3)*3.5-.5;
    p.x += cos(p.z*2.)*1.;
    vec2 q = vec2(mod(p.x, c)-c/2., p.y);
    return length(q) - h*1.4 -sin(p.z*3.+sin(p.x*7.)*0.5)*0.1;
}

vec3 tri3(in vec3 p){ return vec3(tri(p.z+tri(p.y)), tri(p.z+tri(p.x)), tri(p.y+tri(p.x))); }
float Hash(vec3 p)
{
    p  = fract(p * MOD3);
    p += dot(p.xyz, p.yzx + 19.19);
    return fract(p.x * p.y * p.z);
}
vec4 quad(in vec4 p){ return abs(fract(p.yzwx+p.wzxy)-.5); }

float Noise3d(in vec3 q, float time)
{
    float z=1.4;
    vec4 p = vec4(q, time*.1);
    float rz = 0.;
    vec4 bp = p;
    for (float i=0.; i<= 2.; i++)
    {
        vec4 dg = quad(bp);
        p += (dg);

        z *= 1.5;
        p *= 1.3;

        rz+= (tri(p.z+tri(p.w+tri(p.y+tri(p.x)))))/z;

        bp = bp.yxzw*2.0+.14;
    }
    return rz;
}