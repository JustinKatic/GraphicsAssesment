// A simple shader for flat colors
#version 410

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 Ka;  //The lambient color of the model's material
uniform vec3 Kd; //the diffuse color of the model's material
uniform vec3 Ks; //the specular color of the model's material
uniform float Ns;//the specular power of the model's material




uniform vec3 AmbientColor; // Ambient color of the light
uniform vec3 LightColor; // color of the light
uniform vec3 LightDirection; 

uniform vec3 CameraPosition; // Position of the cam viewport for specular calcualtions

out vec4 FragColor;

void main()
 {
    //make sure the normal and the light directions have been normalized
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    //now we can calculate the lambert term, negative the light direction
    float lambertTerm = max(0,min(1,dot(N,-L)));

    //Calculate the view vector ad the reflection vector
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L,N);

    //determine the value of the specular term
    float specularTerm = pow(max(0,dot(R,V)), 32);

    //Determine the value of the ambient
    vec3 ambient = AmbientColor * Ka;
    //Determine the value of the diffuse
    vec3 diffuse = LightColor * Kd * lambertTerm;
    //Determine the value of the specular
    vec3 specular = LightColor * Ks * specularTerm;

    //output the final color
    FragColor = vec4(ambient + diffuse + specular, 1);
}