#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;
in vec3 car1;
in vec3 car2;
in vec3 color_c;
in vec3 posFocus;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3  fvertex;
out vec3  fnormal;
out vec3 fcar1;
out vec3 fcar2;
out vec3 fposFocus;


void main()
{	
    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff * color_c;
    fmatspec = matspec;
    fmatshin = matshin;
    fcar1 = car1;
    fcar2 = car2;
    fvertex = vertex; // CALCULEU CORRECTAMENT
    fnormal = normal; // CALCULEU CORRECTAMENT
    fposFocus = posFocus;

    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
