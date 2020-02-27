// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

float sdSphere(vec3 cameraPos, vec3 center, float radius) {
    return distance(cameraPos, center) - radius;
}

float sdBox(vec3 cameraPos, vec3 start, vec3 dimensions) {
    const vec3 dist = distance(cameraPos, start);
    const vec3 q = abs(dist) - dimensions;
    return length(max(dimensions, 0.0)) + min(max(dimensions.x, max(dimensions.y, dimensions.z)), 0.0);
}

float sdRoundBox(vec3 cameraPos, vec3 start, vec3 dimensions, float radius) {
    const vec3 dist = distance(cameraPos, start);
    const vec3 q = abs(dist) - dimensions;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y,q.z)), 0.0) - radius;
}

float sdPlane(vec3 cameraPos, vec3 position, vec4 normal) {
    return dot(position, n.xyz) + n.w;
}