#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
  
    virtual void keyPressEvent (QKeyEvent *event);

    // funció per comprovar si una nova posició de la serp és vàlida
    virtual bool checkPosition(glm::vec3 pos);
 
    virtual void TerraTransform();

    virtual void initializeGL();

    virtual void paintGL();
   
    virtual void creaBuffersModels();

    virtual void iniEscena();

    virtual void iniCamera();

    virtual void MarbleTransform();

    virtual void PipeTransform(glm::vec3 pos);
    virtual void SnakeHeadTransform();
    virtual void SnakeBodyTransform(glm::vec3 pos);
    virtual void SnakeTailTransform();
    virtual void updateSnakeGame();
    //virtual void iniCameraOrto();
    virtual void viewTransform();
    virtual void projectTransform();
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    
    float horitz = 0;
    float vert = 0;

    float distancia, zNear, zFar, angleh, anglet;

    float girPsi, girTheta, left, right, bottom, top, raw;

    bool perspectiva = true;

    int xClick, yClick;

  public slots:


  private:
  
    int printOglError(const char file[], int line, const char func[]);


};
