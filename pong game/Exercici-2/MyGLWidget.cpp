#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
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

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      if(!pilota_activa) timer.start(100);
      pilota_activa = true;
      break;
    }
    case Qt::Key_I: { 
      timer.stop();
      pilota_activa = false;
      posPilota = glm::vec3(9.0, 0.0, 0.0); 
      break;
        }           
    case Qt::Key_Left: { 
      if (posPorter.z < 7) posPorter.z += 0.5;
      break;
        }  
    case Qt::Key_Right: { 
      if (posPorter.z > -7) posPorter.z -= 0.5;
      break;
        }
    case Qt::Key_C: { 
      if (perspectiva) perspectiva = false;
      else perspectiva = true;
      iniCamera();
      break;
    }           
    case Qt::Key_R: { 
      iniEscena();
      break;
    }  
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::initializeGL(){
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  LL2GLWidget:: carregaShaders();
  LL2GLWidget:: creaBuffersModels();
  LL2GLWidget:: creaBuffersTerra();
  LL2GLWidget:: creaBuffersCub();
  iniEscena();
  iniCamera();
}

void MyGLWidget::modelTransformTerra(){
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(3,1,2));
  TG = glm::translate(TG, centreEscena);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCub(int i){
  glm::vec3 escala1 = glm::vec3(24, 2, 0.2);
  glm::vec3 escala3 = glm::vec3(0.2, 2, 16);
  glm::vec3 centre1 = glm::vec3(0, 0, -7.9);
  glm::vec3 centre2 = glm::vec3(0, 0, 7.9);
  glm::vec3 centre3 = glm::vec3(-11.9, 0, 0);
  glm::mat4 TG1(1.0f);
  glm::mat4 TG2(1.0f);
  glm::mat4 TG3(1.0f);
    if (i == 1) {
      TG1 = glm::translate(TG1, centre1);
      TG1 = glm::scale(TG1, escala1);
      glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG1[0][0]);
    }
    if (i == 2) {
      TG2 = glm::translate(TG2, centre2);
      TG2 = glm::scale(TG2, escala1);
      glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG2[0][0]);
    }
    if (i == 3) {
      TG3 = glm::translate(TG3, centre3);
      TG3 = glm::scale(TG3, escala3);
      glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG3[0][0]);
    }

}

void MyGLWidget::modelTransformLego(){
  float escala;
  glm::vec3 centreBase;
  LL2GLWidget::calculaCapsaModel(models[LEGO], escala, 4, centreBase);

  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posPorter);
  TG = glm::rotate(TG, float(M_PI/2), glm::vec3(0,-1,0));
  TG = glm::scale(TG, glm::vec3(escala));
  TG = glm::translate(TG, -centreBase);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);  
}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  viewTransform();
  // Patricio
  LL2GLWidget::glBindVertexArray (VAO_models[LEGO]);
  modelTransformLego();
  LL2GLWidget:: glDrawArrays(GL_TRIANGLES, 0, models[LEGO].faces().size()*3);

  // Pilota
  glBindVertexArray (VAO_models[PILOTA]);
  LL2GLWidget::modelTransformPilota ();
  glDrawArrays(GL_TRIANGLES, 0, models[PILOTA].faces().size()*3);
  
  // Cub
  glBindVertexArray (VAO_Cub);
  modelTransformCub (1);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray (VAO_Cub);
  modelTransformCub (2);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray (VAO_Cub);
  modelTransformCub (3);
  glDrawArrays(GL_TRIANGLES, 0, 36);

// Terra
  LL2GLWidget::glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::iniCamera()
{
  vrp = centreEscena;
  distancia = 2*radiEscena;
  up = glm::vec3(-1, 0, 0);
  obs = glm::vec3(0,18,0);
  FOV = M_PI/3;
  zNear =  distancia - radiEscena ;
  zFar  = distancia + radiEscena + 20;
  raw = 1.0;

  left = -15;
  right = 15;
  bottom = -15;
  top = 15;
  projectTransform();
  viewTransform();
}

void MyGLWidget:: projectTransform(){
  glm:: mat4 Proj(1.0f);
  if (perspectiva) Proj = glm:: perspective(FOV, raw, zNear, zFar);
  else Proj = glm:: ortho(left, right, bottom, top, zNear, zFar);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform(){
    // Matriu de posició i orientació de l'observador
  glm::mat4 Views(1.0f);
  if (perspectiva){
    Views = glm::translate(Views, glm::vec3(0, 0, -30));
   // Views = glm::rotate(Views, float(M_PI), glm::vec3(0,0,1));
    Views = glm::rotate(Views, -girPsi, glm::vec3(1,0,0));
    Views = glm::rotate(Views, girTheta, glm::vec3(0,1,0));
  }
  else Views = glm::lookAt (obs, vrp, up);
  
  
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &Views[0][0]);
}

void MyGLWidget::iniEscena(){
  radiEscena = sqrt(80);
  centreEscena = glm::vec3(0,0,0);

  posPorter = glm::vec3(11.0, 0.0, 0.0);  // posició inicial del porter
  posPilota = glm::vec3(9.0, 0.0, 0.0); 
  LL2GLWidget::dirInicialPilota(); 
  girPsi = -M_PI/4;
  girTheta = 0;

  
  connect(&timer, &QTimer::timeout, this, &MyGLWidget::modificar_posicio);
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE)
  {
    girTheta += (e->x() - xClick) * factorAngleY;
    girPsi += (e->y() - yClick) * factorAngleX;
    update();
  }
  xClick = e->x();
  yClick = e->y();
  
}

void MyGLWidget::modificar_posicio(){
  posPilota += dirPilota;
  if (LL2GLWidget::rebotaPorter()){
    LL2GLWidget::canviaDireccio();
  }
  else if (posPilota.x > 11) posPilota.x += 100; //Gol
  else rebotaParets();
  update();
}

void MyGLWidget::rebotaParets(){
  if (posPilota.x < -11) dirPilota.x = -dirPilota.x;
  else if (posPilota.z < -7) dirPilota.z = -dirPilota.z;
  else if(posPilota.z > 7) dirPilota.z = -dirPilota.z;
}





