float sdSphere(vec3 pos, vec3 center, float radius) {
    return distance(pos, center) - radius;
}

float sdBox(vec3 pos, vec3 start, vec3 dimensions) {
    const vec3 dist = distance(pos, start);
    const vec3 q = abs(dist) - dimensions;
    return length(max(dimensions, 0.0)) + min(max(dimensions.x, max(dimensions.y, dimensions.z)), 0.0);
}