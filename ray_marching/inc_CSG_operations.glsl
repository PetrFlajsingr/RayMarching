float opMinus(float d1, float d2)
{
    return max(-d2, d1);
}

vec2 opUnion(vec2 d1, vec2 d2)
{
    return (d1.x < d2.x) ? d1 : d2;
}

float sminCubic(float a, float b, float k)
{
    float h = max(k-abs(a-b), 0.0);
    return min(a, b) - h*h*h/(6.0*k*k);
}

vec2 opBlend(vec2 d1, vec2 d2)
{
    float k = 2.0;
    float d = sminCubic(d1.x, d2.x, k);
    float m = mix(d1.y, d2.y, clamp(d1.x-d, 0.0, 1.0));
    return vec2(d, m);
}