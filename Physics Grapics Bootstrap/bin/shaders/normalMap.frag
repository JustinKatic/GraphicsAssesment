#version 410
in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;
 
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
 
uniform vec3 Ka; // The ambient color of the model's Material
uniform vec3 Kd; // the diffuse color of the model's Material
uniform vec3 Ks; // The specular color of the model's Material
uniform float Ns; //The specular power of the model's Material
 
uniform vec3 AmbientColor; // Ambient color of the light
uniform vec3 LightColor; //Color of the Light
uniform vec3 LightDirection;
 
uniform vec3 LightColor1; //Color of the Light1
uniform vec3 LightDirection1;
 
uniform vec3 CameraPosition; // Position of the viewport camera for specular calculations
 
out vec4 FragColor;
void main() {
 
    //Make sure the normal and the light directions have been normalised
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    vec3 L1 = normalize(LightDirection1);
 
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
 
    mat3 TBN = mat3(T,B,N);
 
    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;
 
    N = TBN * (texNormal * 2 - 1);
 
    // Now we can calculate the lambert term, negative the light direction
    float lambertTerm = max(0, min(1, dot(N,-L)));
    float lambertTerm1 = max(0, min(1, dot(N,-L1)));
 
    //Calculate the view vector and the reflection vector
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L,N);
    vec3 R1 = reflect(L1,N);
 
    // Determining the value of the specular term
    float specularTerm = pow(max(0,dot(R, V)), Ns) * lambertTerm;
    float specularTerm1 = pow(max(0,dot(R1, V)), Ns) * lambertTerm1;
 
    //Determine the value of the ambient
    vec3 ambient = AmbientColor * Ka * texDiffuse;
 
    //Deteremine the valuse of the diffuse
    vec3 diffuse = LightColor * Kd * texDiffuse * lambertTerm;
    vec3 diffuse1 = LightColor1 * Kd * texDiffuse * lambertTerm1;
 
    //Determine the value of the specular
    vec3 specular = LightColor * Ks * texSpecular * specularTerm;
    vec3 specular1 = LightColor1 * Ks * texSpecular * specularTerm1;
 
    //Show the standard normals, turn off: N = TBN * (texNormal * 2 - 1);
    //FragColor = vec4(N,1);
 
    //Show the standard normals
 
    //FragColor = vec4(N,1);
 
    //Show texture normals
    //FragColor = vec4(texNormal, 1);
 
    //Show without texture
    //FragColor = vec4(ambient + lambertTerm + specular, 1);
 
    //Output the final color
 
    FragColor = vec4(ambient + (diffuse + diffuse1) + (specular + specular1 ), 1);
}