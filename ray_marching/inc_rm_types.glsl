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
    bool intersected;
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

const uint leftChildMask = 0x00FF;
const uint rightChildMask = 0xFF00;
const uint rightChildShift = 16;
const uint nodeCategoryMask = 0xC0000000;
const uint nodeMaterialMask = 0x3C000000;
const uint nodeMaterialShift = 26;
const uint nodeTypeMask = 0x3FFFFFF;

const uint shapeCategory = 0x0;
const uint binaryOpCategory = 0x40000000;
const uint warpOpCategory = 0x80000000;
const uint opCategory = binaryOpCategory | warpOpCategory;

#define IS_SHAPE(node) bool((node.categoryType & nodeCategoryMask) == shapeCategory)
#define IS_BINARY_OP(node) bool((node.categoryType & nodeCategoryMask) == binaryOpCategory)
#define IS_WARP_OP(node) bool((node.categoryType & nodeCategoryMask) == warpOpCategory)
#define IS_OP(node) bool((node.categoryType & nodeCategoryMask) != shapeCategory)
#define IS_COORD_LOAD(nodeIndex) bool(nodeIndex == 0xFFFFFFFF)
#define SHAPE_INDEX(node) (node.categoryType & nodeTypeMask)
#define OPERATION_INDEX(node) (node.categoryType & nodeTypeMask)
#define PARAM_OFFSET(node) node.paramOffset
#define SHAPE_MATERIAL(node) int((node.categoryType & nodeMaterialMask) >> nodeMaterialShift)

#define STACK_SIZE 20

#define STACK_TYPE_NAME(type, size) Stack##type##size
#define STACK(type, size) struct STACK_TYPE_NAME(type, size) { type data[size]; int top; }
#define MAKE_STACK_INIT(typename)       \
typename initStack_##typename()            \
{ \
 typename result;                     \
 result.top = -1;                    \
 return result;                     \
 }

#define STACK_INIT(typename) initStack_##typename##()

#define STACK_PUSH(stack, value) { ++stack.top; stack.data[stack.top] = value; }
#define STACK_TOP(stack) stack.data[stack.top]
#define STACK_POP(stack) --stack.top
#define STACK_EMPTY(stack) bool(stack.top < 0)
#define STACK_RESET(stack) stack.top = -1


subroutine ShadowResult shadowCalc(Ray ray, vec3 lightDir, vec3 normal);
subroutine float ambientOcclusionCalc(vec3 pos, vec3 nor);
subroutine vec3 shadowIntensityCalc(vec3 diffuse, float specular, float shadow);