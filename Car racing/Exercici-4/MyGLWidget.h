// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
 

  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

  public slots:
    void funciona_timer();

    
    
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformTerra ();
    virtual void modelTransformRoad (glm::vec3 pos, float angle);
    virtual void modelTransformPipe ();
    virtual void modelTransformCar (float radi, float angle);
    virtual void paintGL ( );
    virtual void carregaShaders();
    virtual void initializeGL();
        virtual void iniCamera();
    virtual void iniEscena();
    virtual void resizeGL(int x, int y);
    virtual void iniMaterialTerra();
    
    QTimer timer;
    
    
    virtual void projectTransform();
    
    virtual void viewTransform();

    

  private:
  
    GLuint colorLoc, car1Loc, car2Loc, posFocusLoc;
    GLint ample, alt;
    glm::vec3 posFocus = glm::vec3(1,1,1);
    int printOglError(const char file[], int line, const char func[]);
    glm::vec3 punts[4] = {glm::vec3(5, 0.01, -5), glm::vec3(5, 0.01, 5), glm::vec3(-5, 0.01, 5), glm::vec3(-5, 0.01, -5)};
    //glm:: vec3 punts = glm::vec3 (5, 0, -5);
    float angles[4] = {0.0, -M_PI/2, M_PI, M_PI/2};
    bool timer_activat = false;
    GLuint encendreLoc;
    glm::vec3 encendre = glm::vec3(1,0,0);
    int angle_primer_cotxe = 0.0;
    int angle_segon_cotxe = 0.0;
    glm::vec3 centreEsc;
    float ra, radiEsc, angleY;
    glm::vec3 color= glm::vec3(1.0, 1.0, 1.0);
    
};
