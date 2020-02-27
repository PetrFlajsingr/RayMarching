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