#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
  
  public slots:
    void modificar_posicio();

  protected:
  
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void modelTransformTerra ();
    virtual void modelTransformLego();
    virtual void paintGL();
    virtual void modelTransformCub(int i);
    virtual void iniCamera();
    virtual void viewTransform ();
    virtual void projectTransform ();
    virtual void initializeGL();
    virtual void iniEscena();
  
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    virtual void rebotaParets();
    
  private:
  
    int printOglError(const char file[], int line, const char func[]);

    glm::vec3 centreEscena;

    bool perspectiva = true;
    bool pilota_activa = false;

    float radiEscena, distancia, FOV, zNear, zFar, raw, girPsi, girTheta, left, right, bottom, top;
    glm::vec3 up, vrp, obs;

    int xClick, yClick;

    int posicioLego;
    glm::vec3 posLegoVec, posicio_bola, dire_pilota;
  


   
};
