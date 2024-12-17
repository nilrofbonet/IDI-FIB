// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/glm.hpp"

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

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

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {
    connect(&timer, SIGNAL(timeout()), this, SLOT(timertriggered()));
}

void MyGLWidget::initializeGL() {
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  carregaShaders();
  
  LL4GLWidget::creaBuffersMorty();
  LL4GLWidget::creaBuffersFantasma();
  LL4GLWidget::creaBuffersDiscoBall();
  LL4GLWidget::creaBuffersTerraIParets();

  LL4GLWidget:: iniEscena();
  LL4GLWidget:: iniCamera();

  colFocusEsc = glm::vec3(0.4, 0.4, 0.4);
  glUniform3fv(colFocusLocEsc, 1, &colFocusEsc[0]);

  colFocusD0 = glm::vec3(0.4, 0, 0);
  glUniform3fv(colFocusLocD0, 1, &colFocusD0[0]); 
  
  colFocusD1 = glm::vec3(0, 0.4, 0);
  glUniform3fv(colFocusLocD1, 1, &colFocusD1[0]); 
  
  colFocusD2 = glm::vec3(0, 0, 0.4);
  glUniform3fv(colFocusLocD2, 1, &colFocusD2[0]); 
  
  colFocusD3 = glm::vec3(0.4, 0.4, 0);
  glUniform3fv(colFocusLocD3, 1, &colFocusD3[0]); 
}

void MyGLWidget::timertriggered() {
  makeCurrent();   
  modificarllums();
  angleBola += glm::radians(5.0f);
  update();
}

void MyGLWidget::paintGL() {
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA esquerra
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  
  // FANTASMA dreta
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (9.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformDiscoBall() {
  TGBolaDisco = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TGBolaDisco = glm::scale(TGBolaDisco, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TGBolaDisco = glm::rotate(TGBolaDisco, angleBola, glm::vec3(0,1,0));
  TGBolaDisco = glm::translate(TGBolaDisco, -centreBaseDiscoBall);

  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TGBolaDisco));
  glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, &NormalMatrix[0][0]);
  modificarposiciollums();
  
  posFocusEsc = glm::vec3(View*glm::vec4(5,10,5,1));
  glUniform3fv(posFocusLocEsc, 1, &posFocusEsc[0]);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGBolaDisco[0][0]);
}

void MyGLWidget::modelTransformMorty() {
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::rotate(TG, angleMorty, glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseMorty);
  
  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, &NormalMatrix[0][0]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma(float posX) {
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::rotate(TG, angleMorty, glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, &NormalMatrix[0][0]);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra() {
  glm::mat4 TG = glm::mat4(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, &NormalMatrix[0][0]);

}

void MyGLWidget::modificarposiciollums() {
  posFocusD0 = glm::vec3(View*TGBolaDisco*glm::vec4(1.2,0,0,1));
  glUniform3fv(posFocusLocD0, 1, &posFocusD0[0]);
  
  posFocusD1 = glm::vec3(View*TGBolaDisco*glm::vec4(-1.2,0,0, 1));
  glUniform3fv(posFocusLocD1, 1, &posFocusD1[0]);
  
  posFocusD2 = glm::vec3(View*TGBolaDisco*glm::vec4(0,0,1.2,1));
  glUniform3fv(posFocusLocD2, 1, &posFocusD2[0]);
  
  posFocusD3 = glm::vec3(View*TGBolaDisco*glm::vec4(0,0,-1.2,1));
  glUniform3fv(posFocusLocD3, 1, &posFocusD3[0]);
}

void MyGLWidget::modificarllums() {
    if (iteracionegativa) {
      if (iteracio == 0) colFocusD0 = glm::vec3(0.4, 0, 0);
      else if (iteracio == 1) colFocusD1 = glm::vec3(0, 0.4, 0);
      else if (iteracio == 2) colFocusD2 = glm::vec3(0, 0, 0.4);
      else colFocusD3 = glm::vec3(0.4, 0.4, 0);

    }

    else {
      if (iteracio == 0) colFocusD0 = glm::vec3(0, 0, 0);
      else if (iteracio == 1) colFocusD1 = glm::vec3(0, 0, 0);
      else if (iteracio == 2) colFocusD2 = glm::vec3(0, 0, 0);
      else colFocusD3 = glm::vec3(0, 0, 0);
    }

    ++iteracio; 
    if (iteracio == 4) {
      iteracionegativa = ! iteracionegativa;
      iteracio = 0; 
    }

    glUniform3fv(colFocusLocD0, 1, &colFocusD0[0]); 
    glUniform3fv(colFocusLocD1, 1, &colFocusD1[0]); 
    glUniform3fv(colFocusLocD2, 1, &colFocusD2[0]); 
    glUniform3fv(colFocusLocD3, 1, &colFocusD3[0]); 
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

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
    angleMorty += glm::radians(45.0f);
    break;
	}
  case Qt::Key_D: {
    angleMorty -= glm::radians(45.0f);
    break;
	}
  case Qt::Key_E: {
	  boolfocusescena = ! boolfocusescena;
    if (boolfocusescena) colFocusEsc = glm::vec3(0.4, 0.4, 0.4);
    else colFocusEsc = glm::vec3(0,0,0);
    glUniform3fv(colFocusLocEsc, 1, &colFocusEsc[0]);
    break;
  }	
  case Qt::Key_B: {
    modificarllums();
    break;
  }
  case Qt::Key_R: {
    angleBola += glm::radians(5.0f);
    break;
	}
  case Qt::Key_S: {
    discomode = ! discomode;
    if (discomode) timer.start(100);
    else timer.stop();
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::carregaShaders() {
  LL4GLWidget::carregaShaders();

  NormalLoc = glGetUniformLocation(program->programId(), "NormalMatrix");

  posFocusLocEsc = glGetUniformLocation(program->programId(), "posFocusEsc"); 
  colFocusLocEsc = glGetUniformLocation(program->programId(), "colFocusEsc"); 
  
  posFocusLocD0 = glGetUniformLocation(program->programId(), "posFocusD0"); 
  colFocusLocD0 = glGetUniformLocation(program->programId(), "colFocusD0"); 
  
  posFocusLocD1 = glGetUniformLocation(program->programId(), "posFocusD1"); 
  colFocusLocD1 = glGetUniformLocation(program->programId(), "colFocusD1"); 
  
  posFocusLocD2 = glGetUniformLocation(program->programId(), "posFocusD2"); 
  colFocusLocD2 = glGetUniformLocation(program->programId(), "colFocusD2"); 

  posFocusLocD3 = glGetUniformLocation(program->programId(), "posFocusD3"); 
  colFocusLocD3 = glGetUniformLocation(program->programId(), "colFocusD3"); 
}
















