// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm


float sdSphere(vec3 p, float radius) {
    return length(p) - radius;
}

float sdPlane(vec3 p, vec4 n)
{
    return dot(p, n.xyz) + n.w;
}

float sdBox(vec3 p, vec3 b)
{
    vec3 d = abs(p) - b;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

vec2 repeatingSphere(vec3 p, vec3 c)
{
    vec3 q = mod(p, c)-0.5*c;
    return vec2(sdSphere(q, .1));
}