float opMinus(float d1, float d2) {
    return max(-d2, d1);
}

float opUnion(float d1, float d2) {
    return min(d1, d2);
}

float opSubstraction(float d2, float d1) { return max(-d1, d2); }

float opIntersection(float d1, float d2) { return max(d1, d2); }

float sminCubic(float a, float b, float k) {
    float h = max(k-abs(a-b), 0.0);
    return min(a, b) - h*h*h/(6.0*k*k);
}

float opBlend(float d1, float d2, float k) {
    float d = sminCubic(d1, d2, k);
    return d;
}

vec3 opRep(vec3 p, vec3 domain) {
    return mod(p + 0.5 * domain, domain) - 0.5 * domain;
}

vec3 opRepLim(vec3 p, vec3 domain, vec3 limit) {
    return p - domain * clamp(round(p / domain), -limit, limit);
}

float opRepeat(float d, float domain) {
    return mod(d, domain) - domain / 2.0;
}

vec3 opStretch(vec3 p, vec3 mul){ return p * mul; }

float opSmoothSubtraction(float d1, float d2, float k) {
    float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
    return mix(d2, -d1, h) + k * h * (1.0 - h);
}