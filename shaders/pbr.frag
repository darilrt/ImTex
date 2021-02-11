#version 400
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform vec3 albedoColor;
uniform float albedoValue;
uniform float normalValue;
uniform float metallicValue;
uniform float roughnessValue;
uniform float aoValue;

in vec2 textcoord;
in vec3 normal;
in vec3 worldPos;

in mat4 modelMatrix;

const float PI = 3.14159265359;

vec3 getNormalFromMap() {
    vec3 tangentNormal = (normalValue == -1 ? texture(normalMap, textcoord).xyz : vec3(0.5, 0.5, 1.0)) * 2.0 - 1.0;    
    vec3 Q1  = dFdx(worldPos);
    
    vec3 Q2  = dFdy(worldPos);
    vec2 st1 = dFdx(textcoord);
    vec2 st2 = dFdy(textcoord);

    vec3 N   = normalize(normal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    vec3 camPos = vec3(0, 0, -3);
    
    vec3 lightPositions[4] = {
        (vec4( 4,  4, -4, 1.0) * modelMatrix).xyz,
        (vec4(-10, 5, -10, 1.0)).xyz,
        (vec4(-4, -4, -4, 1.0) * modelMatrix).xyz,
        (vec4( 4, -4, -4, 1.0) * modelMatrix).xyz,
    };
    
    vec3 lightColors[4] = {
        vec3(1.0, 1.0, 1.0),
        vec3(1.0, 1.0, 1.0) * 5,
        vec3(1.0, 1.0, 1.0),
        vec3(1.0, 1.0, 1.0),
    };
    
    vec3 albedo     = albedoValue    == -1 ? pow(texture(albedoMap, textcoord).rgb, vec3(2.2)) : albedoColor;
    float metallic  = metallicValue  == -1 ? texture(roughnessMap, textcoord).r : metallicValue;
    float roughness = roughnessValue == -1 ? texture(roughnessMap, textcoord).r : roughnessValue;
    float ao        = aoValue        == -1 ? texture(aoMap, textcoord).r : aoValue;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - worldPos);
    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        vec3 L = normalize(lightPositions[i] - worldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - worldPos);
        float attenuation = 1.0 / (distance / 4.0);
        vec3 radiance = lightColors[i] * attenuation;

        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = nominator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;     

        float NdotL = max(dot(N, L), 0.0);        

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }   

    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    
    gl_FragColor = vec4(color, 1.0);
}