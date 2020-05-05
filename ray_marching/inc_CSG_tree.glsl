float getFloatCSGParam(uint paramOffset) {
    return csgParams[paramOffset];
}
vec3 getVec3CSGParam(uint paramOffset) {
    return vec3(csgParams[paramOffset], csgParams[paramOffset + 1], csgParams[paramOffset + 2]);
}
vec4 getVec4CSGParam(uint paramOffset) {
    return vec4(csgParams[paramOffset], csgParams[paramOffset + 1], csgParams[paramOffset + 2], csgParams[paramOffset + 3]);
}
struct SSBOStack {
    int top;
};
SSBOStack initSSBOStack() {
    SSBOStack result;
    result.top = int(postOrderStack[0] - 1);
    return result;
}
    #define SSBO_STACK_TOP(stack) postOrderStack[1 + stack.top]
    #define SSBO_STACK_POP(stack) stack.top--
    #define SSBO_STACK_EMPTY(stack) bool(stack.top < 0)
    #define SSBO_STACK_RESET(stack) stack.top = int(postOrderStack[0] - 1);

float distToShape(vec3 camPos, CSGNode node) {
    const uint paramOffset = PARAM_OFFSET(node);
    switch (SHAPE_INDEX(node)) {
        case 0:
        return sdBox(camPos - getVec3CSGParam(paramOffset), getVec3CSGParam(paramOffset + 3));
        case 1:
        return sdSphere(camPos - getVec3CSGParam(paramOffset), getFloatCSGParam(paramOffset + 3));
        case 2:
        return sdPlane(camPos - getVec3CSGParam(paramOffset), getVec4CSGParam(paramOffset + 3));
    }
    return FLT_MAX;
}

float applyOperation(CSGNode node, float leftOperand, float rightOperand) {
    switch (OPERATION_INDEX(node)) {
        case 0:
        return opUnion(leftOperand, rightOperand);
        case 1:
        return opSubstraction(leftOperand, rightOperand);
        case 2:
        return opBlend(leftOperand, rightOperand, getFloatCSGParam(PARAM_OFFSET(node)));
    }
    return FLT_MAX;
}
vec3 applyWarpOperation(CSGNode node, vec3 camPos) {
    switch (OPERATION_INDEX(node)) {
        case 0:
        return opRep(camPos, getVec3CSGParam(PARAM_OFFSET(node)));
        case 1:
        return opRepLim(camPos, getVec3CSGParam(PARAM_OFFSET(node)), getVec3CSGParam(PARAM_OFFSET(node) + 3));
        case 2:
        return opStretch(camPos, getVec3CSGParam(PARAM_OFFSET(node)));
    }
    return vec3(FLT_MAX);
}