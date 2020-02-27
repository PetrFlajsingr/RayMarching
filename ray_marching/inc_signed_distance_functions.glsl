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
    return length(max(q, 0.0)) + min(max(q.x, max(q.y,q.z)), 0.0) - radius;
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
    position.z-dimensions.y );
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdTriPrism(vec3 cameraPos, vec3 position, vec2 dimensions) {
    position = distance (cameraPos, position);
    const float k = sqrt(3.0);
    dimensions.x *= 0.5 * k;
    position.xy /= dimensions.x;
    position.x = abs(position.x) - 1.0;
    position.y = position.y + 1.0 / k;
    if(position.x + k * position.y > 0.0){
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