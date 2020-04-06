vec2 opMinus(vec2 d1, vec2 d2)
{
    return vec2(max(-d2.x, d1.x));
}

vec2 opUnion(vec2 d1, vec2 d2)
{
    return (d1.x < d2.x) ? d1 : d2;
}

vec2 opSubstraction(vec2 d2, vec2 d1) { return vec2(max(-d1.x, d2.x)); }

float sminCubic(float a, float b, float k)
{
    float h = max(k-abs(a-b), 0.0);
    return min(a, b) - h*h*h/(6.0*k*k);
}

vec2 opBlend(vec2 d1, vec2 d2, float k)
{
    float d = sminCubic(d1.x, d2.x, k);
    float m = mix(d1.y, d2.y, clamp(d1.x-d, 0.0, 1.0));
    return vec2(d, m);
}

vec3 opRep(vec3 p, vec3 domain)
{
    return mod(p+0.5*domain, domain)-0.5*domain;
}

vec3 opRepLim(vec3 p, vec3 domain, vec3 limit)
{
    return p-domain*clamp(round(p/domain), -limit, limit);
}

float opRepeat(float d, float domain)
{
    return mod(d, domain)-domain/2.0;
}

vec3 opStretch(vec3 p, vec3 mul){ return p * mul; }

vec2 opIntersection(vec2 d1, vec2 d2) { return vec2(max(d1.x, d2.x)); }

vec2 opSmoothSubtraction(vec2 d1, vec2 d2, float k) {
    float h = clamp(0.5 - 0.5*(d2.x+d1.x)/k, 0.0, 1.0);
    return vec2(mix(d2.x, -d1.x, h) + k*h*(1.0-h)); }