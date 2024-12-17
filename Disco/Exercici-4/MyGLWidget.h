// MyGLWidget.h
#include "LL4GLWidget.h"

#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

  public slots:
    void timertriggered();
    
  
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformDiscoBall();
    virtual void modelTransformMorty();
    virtual void modelTransformFantasma(float posX);
    virtual void modelTransformTerra();
    virtual void paintGL();
    virtual void carregaShaders();
    virtual void initializeGL();
    virtual void modificarposiciollums();
    virtual void modificarllums();
  
  private:
    int printOglError(const char file[], int line, const char func[]);


    GLuint posFocusLocEsc, colFocusLocEsc;
    glm::vec3 posFocusEsc, colFocusEsc;
    
    GLuint posFocusLocD0, colFocusLocD0;
    glm::vec3 posFocusD0, colFocusD0;

    GLuint posFocusLocD1, colFocusLocD1;
    glm::vec3 posFocusD1, colFocusD1;

    GLuint posFocusLocD2, colFocusLocD2;
    glm::vec3 posFocusD2, colFocusD2;
    
    GLuint posFocusLocD3, colFocusLocD3;
    glm::vec3 posFocusD3, colFocusD3;

    GLuint NormalLoc;



    glm::mat3 NormalMatrix;
    glm::mat4 TG, TGBolaDisco;
    
    float angleBola = 0.0f;
    float angleMorty = 0.0f;
    int iteracio = 0;
    bool iteracionegativa = false;
    bool boolfocusescena = true;
    bool discomode = false;
    QTimer timer;

};

