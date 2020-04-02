// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

float dot2(vec3 v) { return dot(v, v); }

float sdSphere(vec3 cameraPos, vec3 center, float radius) {
    return distance(cameraPos, center) - radius;
}

float sdBox(vec3 cameraPos, vec3 start, vec3 dimensions) {
    const vec3 dist = cameraPos - start;
    const vec3 q = abs(dist) - dimensions;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdRoundBox(vec3 cameraPos, vec3 start, vec3 dimensions, float radius) {
    const vec3 dist = cameraPos - start;
    const vec3 q = abs(dist) - dimensions;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0) - radius;
}

float sdPlane(vec3 cameraPos, vec3 position, vec4 normal) {
    return dot(position, normal.xyz) + normal.w;
}

float sdHexPrism(vec3 cameraPos, vec3 position, vec2 dimensions) {
    const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
    position = abs(cameraPos - position);
    position.xy -= 2.0 * min(dot(k.xy, position.xy), 0.0) * k.xy;
    const vec2 d = vec2(
    length(position.xy - vec2(clamp(position.x, -k.z*dimensions.x, k.z * dimensions.x), dimensions.x)) * sign(position.y - dimensions.x),
    position.z-dimensions.y);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdTriPrism(vec3 cameraPos, vec3 position, vec2 dimensions) {
    position = cameraPos - position;
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
    position = cameraPos - position;
    const vec3 pa = position - a, ba = b - a;
    const float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h) - radius;
}

float sdCylinder(vec3 cameraPos, vec3 position, float h, float radius) {
    position = cameraPos - position;
    const vec2 d = abs(vec2(length(position.xz), position.y)) - vec2(h, radius);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdCone(vec3 cameraPos, vec3 position, vec3 c){
    position = cameraPos - position;
    const vec2 q = vec2(length(position.xz), position.y);
    const float d1 = -q.y - c.z;
    const float d2 = max(dot(q, c.xy), q.y);
    return length(max(vec2(d1, d2), 0.0)) + min(max(d1, d2), 0.);
}

float sdSolidAngle(vec3 cameraPos, vec3 position, vec2 c, float ra) {
    position = cameraPos - position;
    // c is the sin/cos of the angle
    const vec2 q = vec2(length(position.xz), position.y);
    const float l = length(q) - ra;
    const float m = length(q - c * clamp(dot(q, c), 0.0, ra));
    return max(l, m * sign(c.y * q.x - c.x * q.y));
}

float sdEllipsoid(vec3 cameraPos, vec3 position, vec3 radius) {
    position = cameraPos - position;
    const float k0 = length(position / radius);
    const float k1 = length(position / (radius * radius));
    return k0 * (k0 - 1.0) / k1;
}

float sdTorus(vec3 cameraPos, vec3 position, float radius, float thickness) {
    position = cameraPos - position;
    const vec2 q = vec2(length(position.xz) - radius, position.y);
    return length(q) - thickness;
}

float sdLink(vec3 cameraPos, vec3 position, float len, float radius1, float radius2) {
    vec3 q = vec3(position.x, max(abs(position.y) - len, 0.0), position.z);
    return length(vec2(length(q.xy) - radius1, q.z)) - radius2;
}

float sdOctahedron(vec3 cameraPos, vec3 position, float roundness) {
    position = abs(cameraPos - position);
    const float m = position.x + position.y + position.z - roundness;
    vec3 q = vec3(0);
    if (3.0*position.x < m) q = position.xyz;
    else if (3.0*position.y < m) q = position.yzx;
    else if (3.0*position.z < m) q = position.zxy;
    else return m*0.57735027;

    const float k = clamp(0.5 * (q.z - q.y + roundness), 0.0, roundness);
    return length(vec3(q.x, q.y - roundness + k, q.z - k));
}

float sdPyramid(vec3 cameraPos, vec3 position, float h) {
    position = cameraPos - position;
    float m2 = h*h + 0.25;

    position.xz = abs(position.xz);
    position.xz = (position.z>position.x) ? position.zx : position.xz;
    position.xz -= 0.5;

    vec3 q = vec3(position.z, h*position.y - 0.5*position.x, h*position.x + 0.5*position.y);

    float s = max(-q.x, 0.0);
    float t = clamp((q.y-0.5*position.z)/(m2+0.25), 0.0, 1.0);

    float a = m2*(q.x+s)*(q.x+s) + q.y*q.y;
    float b = m2*(q.x+0.5*t)*(q.x+0.5*t) + (q.y-m2*t)*(q.y-m2*t);

    float d2 = min(q.y, -q.x*m2-q.y*0.5) > 0.0 ? 0.0 : min(a, b);

    return sqrt((d2+q.z*q.z)/m2) * sign(max(q.z, -position.y));
}

float sdTriangle(vec3 cameraPos, vec3 position, vec3 a, vec3 b, vec3 c) {
    position = cameraPos - position;
    vec3 ba = b - a; vec3 pa = position - a;
    vec3 cb = c - b; vec3 pb = position - b;
    vec3 ac = a - c; vec3 pc = position - c;
    vec3 nor = cross(ba, ac);

    return sqrt(
    (sign(dot(cross(ba, nor), pa)) +
    sign(dot(cross(cb, nor), pb)) +
    sign(dot(cross(ac, nor), pc))<2.0)
    ?
    min(min(
    dot2(ba*clamp(dot(ba, pa)/dot2(ba), 0.0, 1.0)-pa),
    dot2(cb*clamp(dot(cb, pb)/dot2(cb), 0.0, 1.0)-pb)),
    dot2(ac*clamp(dot(ac, pc)/dot2(ac), 0.0, 1.0)-pc))
    :
    dot(nor, pa)*dot(nor, pa)/dot2(nor));
}

float sdQuad(vec3 cameraPos, vec3 position, vec3 a, vec3 b, vec3 c, vec3 d)
{
    position = cameraPos - position;
    vec3 ba = b - a; vec3 pa = position - a;
    vec3 cb = c - b; vec3 pb = position - b;
    vec3 dc = d - c; vec3 pc = position - c;
    vec3 ad = a - d; vec3 pd = position - d;
    vec3 nor = cross(ba, ad);

    return sqrt(
    (sign(dot(cross(ba, nor), pa)) +
    sign(dot(cross(cb, nor), pb)) +
    sign(dot(cross(dc, nor), pc)) +
    sign(dot(cross(ad, nor), pd))<3.0)
    ?
    min(min(min(
    dot2(ba*clamp(dot(ba, pa)/dot2(ba), 0.0, 1.0)-pa),
    dot2(cb*clamp(dot(cb, pb)/dot2(cb), 0.0, 1.0)-pb)),
    dot2(dc*clamp(dot(dc, pc)/dot2(dc), 0.0, 1.0)-pc)),
    dot2(ad*clamp(dot(ad, pd)/dot2(ad), 0.0, 1.0)-pd))
    :
    dot(nor, pa)*dot(nor, pa)/dot2(nor));
}