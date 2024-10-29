#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;
in vec3 fposFocus;
in vec3 fcar1;
in vec3 fcar2;
in vec3 encendre;


vec3 llumAmbient = vec3(0.1, 0.1, 0.1);
vec3 colorEscena = vec3(0.8, 0.8, 0.8);
vec3 colorModel = vec3(0.6, 0.0, 0.6);

out vec4 FragColor;

vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
  vec3 escenaLSCO = normalize(fposFocus.xyz - fvertex);
  vec3 car1SCO = normalize(fcar1.xyz - fvertex);
  vec3 car2SCO = normalize(fcar2.xyz - fvertex);
  vec3 normal = normalize(fnormal);

  vec3 fcolor = vec3(0);
  fcolor += Ambient() + Difus(normal, car1SCO, colorModel) + Especular(normal, car1SCO, fvertex, colorModel);
  fcolor += Difus(normal, car2SCO, colorModel) + Especular(normal, car2SCO, fvertex, colorModel);

  if(encendre.x == 1) fcolor += Difus(normal, escenaLSCO, colorEscena) + Especular(normal, escenaLSCO, fvertex, colorEscena);
  FragColor =  vec4(fcolor, 1);
}
