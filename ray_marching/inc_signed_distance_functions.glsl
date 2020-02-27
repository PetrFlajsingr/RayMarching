float sdSphere(vec3 pos, vec3 center, float radius) {
    return length(pos - center) - radius;
}

