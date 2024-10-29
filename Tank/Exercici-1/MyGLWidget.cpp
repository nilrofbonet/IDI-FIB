#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersQuadrat();
}

void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
  
}

void MyGLWidget::modelTransformQuadrat_Roda(glm::vec3 posicio, glm::vec3 escala, float angle)
{
  glm::mat4 TG(1.0f);
      TG = glm::translate(TG,glm:: vec3(posicio.x, posicio.y, posicio.z)); 
      TG = glm::rotate(TG, angle, glm::vec3(0,0,1));
      TG = glm::translate(TG,glm:: vec3(0.1, 0.0, 0));
      TG = glm::translate(TG,glm:: vec3(0.045, 0.025, 0));  
      TG = glm::scale(TG,escala);
      glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformQuadrat_Can(glm::vec3 posicio, glm::vec3 escala)
{
  if (angle_tanc > 2*M_PI) angle_tanc = 2*M_PI;
  if (angle_tanc < M_PI) angle_tanc = M_PI;
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);  
  TG = glm::rotate(TG, angle_tanc, glm::vec3(0,0,1)); 
  TG = glm::translate(TG,glm::vec3(0.0, 0.05, 0.0));  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
  
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Pintem un quadrat
  pintaTanc();
    
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::pintaTanc(){

  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer
  glBindVertexArray(VAOQuadrat);

  //pinta primer rectangle 
  modelTransformQuadrat(glm::vec3(0.5+tx_total, 0.125, 0.0), glm::vec3(1.0, 0.25,1.0));
  glUniform3fv(vertexCol, 1, &verd[0]);
  glDrawArrays(GL_TRIANGLES, 0, 6);    		
  

  //pinta segon rectangle
  modelTransformQuadrat(glm::vec3(0.375+tx_total, 0.375, 0.0), glm::vec3(0.5, 0.25, 1.0));
  glUniform3fv(vertexCol, 1, &verd[0]);
  glDrawArrays(GL_TRIANGLES, 0, 6);    		

  //pinta canon
  modelTransformQuadrat_Can(glm::vec3(0.125+tx_total, 0.25, 0.0), glm::vec3(0.75, 0.125, 1.0));
  glUniform3fv(vertexCol, 1, &gris[0]);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // roda

  float sum = 0.0;
  for(int j = 0; j < 4; ++j){
    for (int i = 0; i < 12; ++i){
      modelTransformQuadrat_Roda(glm::vec3(-0.375+sum+tx_total, -0.125, 0), glm::vec3(0.1, 0.05, 1.0), (i*ang) + angle_rodes);
      if (i%2 == 0) glUniform3fv(vertexCol, 1, &negre[0]);
      else glUniform3fv(vertexCol, 1, &gris[0]);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    sum += 0.25;
  }
   
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Left: 
      angle_tanc+=M_PI/18; 
    	break;
    case Qt::Key_Right:
       angle_tanc-=M_PI/18;
    	break;
    case Qt::Key_A: 
        tx_total -= 0.01;
        angle_rodes += 0.0174532925;
    	break;
    case Qt::Key_D: 
        tx_total += 0.01;
        angle_rodes -= 0.0174532925;
    	break;		
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffersQuadrat ()
{
  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  
  // minX = -1.0
  // maxX = 0.0
  // minY = -1.0
  // maxY = 0.0
  
  Vertices[0] = glm::vec3(-1.0, 0.0, 0);
  Vertices[1] = glm::vec3( 0.0, -1.0, 0);
  Vertices[2] = glm::vec3( 0.0, 0.0, 0);
  Vertices[3] = glm::vec3( 0.0, -1.0, 0);
  Vertices[4] = glm::vec3( -1.0, 0.0, 0);
  Vertices[5] = glm::vec3( -1.0, -1.0, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOQuadrat);
  glBindVertexArray(VAOQuadrat);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);


  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
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

  vertexCol = glGetUniformLocation(program->programId(), "color");

  
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG"); 
}
