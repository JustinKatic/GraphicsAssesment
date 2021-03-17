// A simple shader for flat colors
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ka;  //The lambient color of the model's material
uniform vec3 Kd; //the diffuse color of the model's material
uniform vec3 Ks; //the specular color of the model's material
uniform float Ns;//the specular power of the model's material




uniform vec3 AmbientColor; // Ambient color of the light
uniform vec3 LightColor; // color of the light
uniform vec3 LightDirection;
uniform vec3 LightColor1; // color of the light
uniform vec3 LightDirection1;



uniform vec3 CameraPosition; // Position of the cam viewport for specular calcualtions

out vec4 FragColor;

void main()
 {
    //make sure the normal and the light directions have been normalized
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    vec3 L1 = normalize(LightDirection1);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);

    mat3 TBN = mat3(T,B,N);

    vec3 texDiffuse = texture(diffuseTexture,vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture,vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture,vTexCoord).rgb;

    N = TBN * (texNormal * 2 -1);

    //now we can calculate the lambert term, negative the light direction
    vec3 lambertTerm = max(0,min(1,dot(N,-L))) * LightColor;
    vec3 lambertTerm1 = max(0,min(1,dot(N,-L1))) * LightColor1;


    //Calculate the view vector ad the reflection vector
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L,N);
    vec3 R1 = reflect(L1,N);


    //determine the value of the specular term
    vec3 specularTerm = pow(max(0,dot(R,V)), 32) * LightColor;
    vec3 specularTerm1= pow(max(0,dot(R1,V)), 32)* LightColor1;

    vec3 totalSpecular = specularTerm + specularTerm1;
    vec3 totalLambert = lambertTerm + lambertTerm1;


    //Determine the value of the ambient
    vec3 ambient = AmbientColor * Ka * texDiffuse;
    //Determine the value of the diffuse
    vec3 diffuse =  Kd * texDiffuse * totalLambert;
    //Determine the value of the specular
    vec3 specular =  Ks * texSpecular * totalSpecular;

    //show the texture normals, turn off: N = TBN * (texNormal * 2 -1)
    //FragColor = vec4(N, 1);

    //show the normals
   // FragColor = vec4(N, 1);

    //Show Texture normals
   //FragColor = vec4(texNormal,1);

   //Show without texture
   //FragColor = vec4(ambient + lambertTerm + specular,1);

    //output the final color
    FragColor = vec4(ambient + diffuse + specular,1);

}