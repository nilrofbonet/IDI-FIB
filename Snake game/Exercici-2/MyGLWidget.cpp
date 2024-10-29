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

void MyGLWidget::initializeGL(){
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  LL2GLWidget:: carregaShaders();
  creaBuffersModels();
  LL2GLWidget:: creaBuffersTerra();
  iniEscena();
  iniCamera();
}

bool MyGLWidget::checkPosition(glm::vec3 pos)
{
	bool valid = true;
  glm:: vec3 posi = pos ;
        if (posi.x > 13.5 or posi.x < -13.5 or posi.z > 13.5 or posi.z < -13.5) valid = false;
        for (unsigned int i = 0; i < bodyPos.size(); i++)
        {  
	          if (posi == bodyPos[i]) valid = false;
            
        }
        // Aquí cal que completeu el codi de manera adient

	return valid; 
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      //horitz -= 1;
      //if (horitz < -14) horitz = -14;
      if (not perspectiva) direction = glm:: vec3(0, 0 ,-1);
      else direction = glm:: vec3(0, 0 , 1);
      updateSnakeGame();
      break;
    }
    case Qt::Key_Down: { 
      //horitz += 1;
      //if (horitz > 14) horitz = 14;
      if (not perspectiva) direction = glm:: vec3(0, 0 , 1);
      else direction = glm:: vec3(0, 0 , -1);
      updateSnakeGame();
      break;
        }
    case Qt::Key_Left: { 
      //vert -= 1;
      //if (vert < -14) vert = -14;
      direction = glm:: vec3(-1, 0 ,0);
      updateSnakeGame();
      break;
        }  
    case Qt::Key_Right: { 
      //vert += 1;
      //if (vert > 14) vert = 14;
      direction = glm:: vec3(1, 0 ,0);
      updateSnakeGame();
      break;
        }
    case Qt::Key_C: { 
      if (perspectiva) perspectiva = false;
      else perspectiva = true;
      projectTransform();
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

void MyGLWidget::TerraTransform ()
{
  glm::mat4 TG(1.0f);
  glm::vec3 TransformaTerra(3,0,3);
  TG = glm::scale(TG, TransformaTerra);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::iniEscena()
{
  radiEscena = sqrt(80);
  
  centreEscena = glm::vec3(0,0,0);

  headPos = glm::vec3(0,0,0);
  headAngle = 90;
  
  bodyPos.clear();	    
  bodyPos.push_back(glm::vec3(-1,0,0));    	
  tailPos = glm::vec3(-2,0,0);
  tailAngle = 0;

  marblePos = glm::vec3(10, 0, 0);
  direction = glm::vec3(1,0,0);
  girPsi = M_PI/4;
  girTheta = -M_PI;
}

void MyGLWidget::iniCamera(){
  distancia = 2*radiEscena;
  vrp = centreEscena;
  //obs = vrp + distancia * glm:: vec3(0,1,0);
  obs = glm::vec3(0, 30,0);
  //vrp = glm::vec3(0, 0, 0);
  up = glm::vec3(0, 1, 1);
  fov = M_PI/3;
  zNear =  distancia - radiEscena - 10;
  zFar  = distancia + radiEscena;
  raw = 1.0;
  
  left = -22.5; 
  right = 22.5;
  bottom = -22.5;
  top = 22.5;
  projectTransform();
  viewTransform();
}

void MyGLWidget:: projectTransform(){
  glm:: mat4 Proj(1.0f);
  if (perspectiva){
    Proj = glm:: perspective(fov, raw, zNear, zFar);
  } 
  else Proj = glm:: ortho(left, right, bottom, top, zNear, zFar);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform(){
    // Matriu de posició i orientació de l'observador
  glm::mat4 Views(1.0f);
  if (perspectiva){
    Views = glm::translate(Views, glm::vec3(0, 0, -45));
    Views = glm::rotate(Views, float(M_PI), glm::vec3(0,0,1));
    Views = glm::rotate(Views, -girPsi, glm::vec3(1,0,0));
    Views = glm::rotate(Views, girTheta, glm::vec3(0,1,0));
  }
  else {
    up = glm::vec3(0, 0, -1);
    obs = vrp + distancia * glm:: vec3(0,1,0);
    Views = glm::lookAt (obs, vrp, up);
  }
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &Views[0][0]);
}

void MyGLWidget::paintGL ()
{

  // descomentar per canviar paràmetres
   //glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //projectTransform();
  viewTransform();
  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pipe
  for (int i = 0; i < 30; ++i){
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(-14.5 + i,0, -14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }

  for (int i = 0; i < 30; ++i){
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(-14.5 ,0, -14.5 + i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }

  for (int i = 0; i < 30; ++i){
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(-14.5 + i,0, 14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }

  for (int i = 0; i < 30; ++i){
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(14.5 ,0, -14.5 + i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }

  // Marble
  glBindVertexArray (VAO_models[MARBLE]);
  MarbleTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MARBLE].faces().size()*3);

   // SnakeHead
  //headPos = glm::vec3(0 + vert,0,0 + horitz);
  glBindVertexArray (VAO_models[SNAKE_HEAD]);
  SnakeHeadTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_HEAD].faces().size()*3);
  
  // SnakeBody
  glBindVertexArray (VAO_models[SNAKE_BODY]);
  for (unsigned int i = 0; i < bodyPos.size(); i++)
  {  
	  SnakeBodyTransform(bodyPos[i]);
	  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_BODY].faces().size()*3);
  }

  // SnakeTail
  glBindVertexArray (VAO_models[SNAKE_TAIL]);
  SnakeTailTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_TAIL].faces().size()*3);

  

  glBindVertexArray (0);
}

void MyGLWidget::MarbleTransform(){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, marblePos);
  TG = glm::scale(TG, glm::vec3(escalaModels[MARBLE]));
  TG = glm::translate(TG, -centreBaseModels[MARBLE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PipeTransform (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE], 3*escalaModels[PIPE], escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::SnakeHeadTransform ()
{
  glm::mat4 TG(1.0f);  
  angleh = headAngle*M_PI/180;
  TG = glm::translate(TG, headPos);
  TG = glm::rotate(TG, -angleh, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_HEAD]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_HEAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::SnakeBodyTransform (glm::vec3 pos)
{
  glm:: vec3 aux = pos ;    
  //bodyPos.push_back(glm::vec3(-1 + vert,0,0 + horitz));
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, aux);
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_BODY]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_BODY]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::SnakeTailTransform ()
{
  anglet = tailAngle*M_PI/180;
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, tailPos);
  TG = glm::rotate(TG, -anglet, glm::vec3(0,1, 0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_TAIL]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_TAIL]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::updateSnakeGame()
{
	glm::vec3 newPos = headPos + direction;
	
        if (checkPosition(newPos)) // check boundary limits, check if eats himself
	{
		bodyPos.push_back(headPos);	
		headPos = newPos;
		
		if (direction == glm::vec3(0,0,-1))
			headAngle = 0;
		if (direction == glm::vec3(0,0,1))
			headAngle = -180;
		if (direction == glm::vec3(-1,0,0))
			headAngle = 90;
		if (direction == glm::vec3(1,0,0))
			headAngle = -90;			
	
		if (glm::distance(newPos,marblePos) > 0) // check if doesn't eat marble 
		{
			tailPos = bodyPos.front();
			bodyPos.erase(bodyPos.begin());	
			glm::vec3 tailMov = bodyPos.front() - tailPos;
			tailAngle = signedAngleBetweenVectors(glm::vec3(1,0,0),tailMov);
		}
		else
			computeRandomMarblePosition();
	}
}

void MyGLWidget::creaBuffersModels ()
{
  // Càrrega dels models
  models[SNAKE_HEAD].load("./models/SnakeHead.obj");
  models[SNAKE_BODY].load("./models/SnakeBody.obj");
  models[SNAKE_TAIL].load("./models/SnakeTail.obj");
  models[MARBLE].load("./models/Marble.obj");
  models[PIPE].load("./models/GreenPipe.obj");

  // Creació de VAOs i VBOs per pintar els models
  glGenVertexArrays(NUM_MODELS, &VAO_models[0]);
  
  float alcadaDesitjada[NUM_MODELS] = {1,1,1,0.75,1}; 
  
  for (int i = 0; i < NUM_MODELS; i++)
  {	
	  // Calculem la capsa contenidora del model
	  calculaCapsaModel (models[i], escalaModels[i], alcadaDesitjada[i], centreBaseModels[i]);
  
	  glBindVertexArray(VAO_models[i]);

	  GLuint VBO[2];
	  glGenBuffers(2, VBO);

	  // geometria
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_vertices(), GL_STATIC_DRAW);
	  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(vertexLoc);

	  // color
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_matdiff(), GL_STATIC_DRAW);
	  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(colorLoc);  
  }
  
  glBindVertexArray (0);
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
    
    
  }
xClick = e->x();
    yClick = e->y();
  update ();


}

