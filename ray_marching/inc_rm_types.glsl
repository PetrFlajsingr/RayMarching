struct Material {
    vec4 color;
    int type;
    float reflectivity;
    float refractiveIndex;
    float refractiveFactor;
    float scatterDensity;
    float _padding[3];
};
// enum of materials
const int MatNormal = 0;
const int MatReflective = 1;
const int MatTransparent = 2;
const int MatScatter = 3;
struct RenderResult {
    vec4 color;
    vec3 normal;
    vec3 endPos;
    float fragmentDepth;
    int rmSteps;
};
struct IntersectionResult {
    float distance;
    int material;
    int rmSteps;
    vec4 color;
    vec3 normal;
    bool isInShadow;
    bool isSelfShadowed;
};
struct ShadowResult {
    float shadow;
    bool isSelfShadowed;
    int rmSteps;
};
struct ShadingResult {
    bool isInShadow;
    bool isSelfShadowed;
    vec4 color;
    int rmSteps;
};
struct MapResult {
    float distance;
    int materialIndex;
};
struct Ray {
    vec3 origin;
    vec3 direction;
};
struct RayHitCheckResult {
    bool hit;
    float distance;
    int rmSteps;
};
struct CSGNode {
    uint categoryType;// fist 2 bits  - 0 operation, 1 warp operation, 2 shape, rest type of shape/operation
    uint paramOffset;
    uint childIndices;
};