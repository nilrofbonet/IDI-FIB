// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {

}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL ()
{
    initializeOpenGLFunctions();
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
     glUniformMatrix3fv (posFocusLoc, 1, 1, &posFocus[0]);
  
    carregaShaders();
  
    LL4GLWidget:: creaBuffersModels();
    LL4GLWidget:: creaBuffersTerra();

    iniEscena();
    iniCamera();

    connect (&timer, SIGNAL (timeout()), this, SLOT (funciona_timer()));
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::modelTransformRoad (glm::vec3 pos, float angle)
{
  //pos = glm::vec3(5, 0, -5);
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(5*escalaModels[ROAD]));
  TG = glm::rotate(TG, angle, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPipe ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(0,0,0));
  TG = glm::scale(TG, glm::vec3(5*escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCar (float radi, float angle)
{
  
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
  glm::vec4 pos = TG * glm::vec4(2.48, 0.4, -3.2, 1.0);
  if (radi == 6.5f) glUniform4fv(car1Loc, 1, &pos[0]);
  else glUniform4fv(car2Loc, 1, &pos[0]);
}

void MyGLWidget:: funciona_timer(){
      angle_primer_cotxe += 2;
      angle_segon_cotxe += 3;
      update();
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(0,0,0));
  TG = glm::scale(TG, glm::vec3(2.5, 2.5, 2.5));
  TG = glm::translate(TG, glm::vec3(-5, 0, -5));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      angle_primer_cotxe += 2;
      angle_segon_cotxe += 3;
    break;
	}
  case Qt::Key_L: {

		if(encendre.x != 0) {
      encendre.x = 0;
      glClearColor(0.3, 0.3, 0.3, 1.0);
      glUniformMatrix3fv (encendreLoc, 1, 1, &encendre[0]);
		}
		else {
      encendre.x = 1;
      glClearColor(0.5, 0.7, 1.0, 1.0);
      glUniformMatrix3fv (encendreLoc, 1, 1, &encendre[0]);
		}
		break;
	}
  case Qt::Key_S: {

    if(not timer_activat){
      timer_activat = true;
      timer.start(50);
    }
    else {
      timer_activat = false;
      timer.stop();
    }

    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("./shaders/basicLlumShader.frag");
  vs.compileSourceFile("./shaders/basicLlumShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  colorLoc = glGetAttribLocation(program->programId(), "color_c");
  encendreLoc = glGetAttribLocation(program->programId(), "encendre");

  // Demanem identificadors per als uniforms del vertex shader
  car1Loc = glGetAttribLocation(program->programId(), "car1");
  car2Loc = glGetAttribLocation(program->programId(), "car2");
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "Proj");
  viewLoc = glGetUniformLocation (program->programId(), "View");
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  

  // Road
  for (int i = 0; i < 4; ++i){
    glBindVertexArray (VAO_models[ROAD]);
    modelTransformRoad (punts[i], angles[i]);
    glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  }

  // Car
  for(int i = 0; i < 2; ++i){
    
    
    if (i == 0){
      color = glm::vec3(1,0,0);
      glBindVertexArray (VAO_models[CAR]);
      modelTransformCar (6.5, angle_primer_cotxe);
      glUniform3fv(colorLoc, 1, &color[0]);
      glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
    }
    else {
      color = glm::vec3(0,1,0);
      glBindVertexArray (VAO_models[CAR]);
      modelTransformCar (9, angle_segon_cotxe);
      glUniform3fv(colorLoc, 1, &color[0]);
      glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
      
    }
    
  }
  

  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  modelTransformPipe ();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  
  glBindVertexArray(0);
}



void MyGLWidget::iniEscena ()
{
  centreEsc = glm::vec3(0,0,0);
  radiEsc = 20;  
}

void MyGLWidget:: iniCamera(){
  angleY = 0.0;
  viewTransform ();

}

void MyGLWidget::resizeGL (int w, int h) 
{
#ifdef __APPLE__
  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
  ra = float(ample)/float(alt);

  projectTransform();
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, -15, -2*radiEsc));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.1,0.1,0.1);
  diff = glm::vec3(0.8,0.8,0.8);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(float(M_PI/3.0), ra, radiEsc, 3.0f*radiEsc);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}


