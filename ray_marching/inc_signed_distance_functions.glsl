// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

float sdSphere(vec3 cameraPos, vec3 center, float radius) {
    return distance(cameraPos, center) - radius;
}

float sdBox(vec3 cameraPos, vec3 start, vec3 dimensions) {
    const vec3 dist = distance(cameraPos, start);
    const vec3 q = abs(dist) - dimensions;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdRoundBox(vec3 cameraPos, vec3 start, vec3 dimensions, float radius) {
    const vec3 dist = distance(cameraPos, start);
    const vec3 q = abs(dist) - dimensions;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0) - radius;
}

float sdPlane(vec3 cameraPos, vec3 position, vec4 normal) {
    return dot(position, n.xyz) + n.w;
}

float sdHexPrism(vec3 cameraPos, vec3 position, vec2 dimensions) {
    const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
    position = abs(distance(cameraPos, position));
    position.xy -= 2.0 * min(dot(k.xy, position.xy), 0.0) * k.xy;
    const vec2 d = vec2(
    length(position.xy - vec2(clamp(position.x, -k.z*dimensions.x, k.z * dimensions.x), dimensions.x)) * sign(position.y - dimensions.x),
    position.z-dimensions.y);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdTriPrism(vec3 cameraPos, vec3 position, vec2 dimensions) {
    position = distance (cameraPos, position);
    const float k = sqrt(3.0);
    dimensions.x *= 0.5 * k;
    position.xy /= dimensions.x;
    position.x = abs(position.x) - 1.0;
    position.y = position.y + 1.0 / k;
    if (position.x + k * position.y > 0.0){
        position.xy=vec2(position.x - k * position.y, -k * position.x - position.y) / 2.0;
    }
    position.x -= clamp(position.x, -2.0, 0.0);
    float d1 = length(position.xy) * sign(-position.y) * dimensions.x;
    float d2 = abs(position.z) - dimensions.y;
    return length(max(vec2(d1, d2), 0.0)) + min(max(d1, d2), 0.);
}

float sdCapsule(vec3 cameraPos, vec3 position, vec3 a, vec3 b, float radius) {
    position = distance(cameraPos, position);
    const vec3 pa = position - a, ba = b - a;
    const float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h) - radius;
}

float sdCylinder(vec3 cameraPos, vec3 position, float h, float radius) {
    position = distance(cameraPos, position);
    const vec2 d = abs(vec2(length(position.xz), position.y)) - vec2(h, radius);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdCone(vec3 cameraPos, vec3 position, vec3 c){
    position = distance(cameraPos, position);
    const vec2 q = vec2(length(position.xz), position.y);
    const float d1 = -q.y - c.z;
    const float d2 = max(dot(q, c.xy), q.y);
    return length(max(vec2(d1, d2), 0.0)) + min(max(d1, d2), 0.);
}

float sdCappedCone(vec3 cameraPos, vec3 position, float h, float r1, float r2) {
    position = distance(cameraPos, position);
    const vec2 q = vec2(length(position.xz), position.y);
    const vec2 k1 = vec2(r2, h);
    const vec2 k2 = vec2(r2 - r1, 2.0 * h);
    const vec2 ca = vec2(q.x - min(q.x, (q.y < 0.0) ? r1 : r2), abs(q.y) - h);
    const vec2 cb = q - k1 + k2 * clamp(dot(k1 - q, k2) / dot2(k2), 0.0, 1.0);
    const float s = (cb.x < 0.0 && ca.y < 0.0) ? -1.0 : 1.0;
    return s * sqrt(min(dot2(ca), dot2(cb)));
}

float sdSolidAngle(vec3 cameraPos, vec3 position, vec2 c, float ra) {
    position = distance(cameraPos, position);
    // c is the sin/cos of the angle
    const vec2 q = vec2(length(position.xz), position.y);
    const float l = length(q) - ra;
    const float m = length(q - c * clamp(dot(q, c), 0.0, ra));
    return max(l, m * sign(c.y * q.x - c.x * q.y));
}

float sdEllipsoid(vec3 cameraPos, vec3 position, vec3 radius) {
    position = distance(cameraPos, position);
    const float k0 = length(position / radius);
    const float k1 = length(position / (radius * radius));
    return k0 * (k0 - 1.0) / k1;
}

float sdTorus(vec3 cameraPos, vec3 position, float radius, float thickness) {
    position = distance(cameraPos, position);
    const vec2 q = vec2(length(position.xz) - radius, position.y);
    return length(q) - thickness;
}

vec4 sdJointSphere(vec3 cameraPos, vec3 position, float len, float bend, float width)
{
    position = distance(cameraPos, position);
    // if perfectly straight
    if (abs(bend) < 0.001) return vec4(length(position - vec3(0, clamp(position.y, 0.0, len), 0)) - width, position);

    // parameters
    vec2  sc = vec2(sin(bend), cos(bend));
    float ra = 0.5 * len / bend;

    // recenter
    position.x -= ra;

    // reflect
    vec2 q = position.xy - 2.0 * sc * max(0.0, dot(sc, position.xy));

    float u = abs(ra) - length(q);
    float d2 = (q.y < 0.0) ? dot(q + vec2(ra, 0.0)) : u * u;
    float s = sign(bend);
    return vec4(sqrt(d2 + position.z * position.z) - width,
    (position.y > 0.0) ? s * u : s * sign(-position.x) * (q.x+ra),
    (position.y > 0.0) ? atan(s * position.y, -s * position.x) * ra : (s * position.x < 0.0) ? position.y : len - position.y,
    position.z);
}

vec4 sdJointFlat(vec3 cameraPos, vec3 position, float len, float bend, float width)
{
    position = distance(cameraPos, position);
    // if perfectly straight
    if (abs(bend) < 0.001)
    {
        vec3 q = position; q.y -= 0.5 * len;
        q = abs(q) - vec3(width, len * 0.5, width);
        return vec4(min(max(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0)), position);
    }

    // parameters
    vec2  sc = vec2(sin(bend), cos(bend));
    float ra = 0.5*len/bend;

    // recenter
    position.x -= ra;

    // reflect
    vec2 q = position.xy - 2.0 * sc * max(0.0, dot(sc, position.xy));

    // distance
    float u = abs(ra) - length(q);
    float d = max(length(vec2(q.x + ra - clamp(q.x + ra, -width, width), q.y)) * sign(-q.y), abs(u) - width);

    // parametrization (optional)
    float s = sign(bend);
    float v = ra * atan(s * position.y, -s * position.x);
    u = u * s;

    // square profile
    q = vec2(d, abs(position.z) - width);

    d = min(max(q.x, q.y), 0.0) + length(max(q, 0.0));


    return vec4(d, u, v, position.z);
}

float sdLink(vec3 cameraPos, vec3 position, float len, float radius1, float radius2) {
    vec3 q = vec3(position.x, max(abs(position.y) - len, 0.0), position.z);
    return length(vec2(length(q.xy) - radius1, q.z)) - radius2;
}