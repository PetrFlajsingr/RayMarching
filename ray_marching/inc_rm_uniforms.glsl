layout (location=0) subroutine uniform shadowCalc calculateShadow;
layout (location=1) subroutine uniform ambientOcclusionCalc calculateAmbientOcclusion;
layout (location=2) subroutine uniform shadowIntensityCalc calculateShadowIntensity;

uniform int physicsSphereCount;

uniform bool enableEdgeAA;
uniform ivec2 resolution;
uniform int stepLimit;
uniform int shadowStepLimit;

uniform float time;
uniform float maxDrawDistance;

uniform vec3 cameraPosition;
uniform vec3 cameraFront;

uniform vec3 lightPos;

uniform int maxReflections;
uniform float AA_size;

uniform float relaxationParameter;
uniform float pixelRadius;

uniform bool logStepCount;