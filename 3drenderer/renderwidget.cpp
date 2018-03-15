#include "renderwidget.h"

#include <QImage>
#include <QGLWidget>
#include <QMouseEvent>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "mesh.h"

#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

RenderWidget::RenderWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , program(nullptr)
{
  this->setFocusPolicy(Qt::TabFocus);
  this->isMovingCamera = false;

  this->camera = new Camera(
    glm::vec3(5.0f, 5.0f, -10.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    60.0f,
    0.1f,
    100.0f,
    this->width(),
    this->height()
    );

  this->hasWireframe = false;
  this->hasOcclusion = true;
}


RenderWidget::~RenderWidget()
{
  delete this->program;
  delete this->camera;

  this->glDeleteVertexArrays(1, &VAO);
  this->glDeleteBuffers(1, &VBO);
  this->glDeleteBuffers(1, &EBO);
}


void RenderWidget::initializeGL()
{
  this->initializeOpenGLFunctions();

  this->glEnable(GL_DEPTH_TEST);

  this->glClearColor(0, 0, 0, 1);
  this->glViewport(0, 0, width(), height());

  this->program = new QOpenGLShaderProgram();
  this->program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexshader.glsl");
  this->program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/geometryshader.glsl");
  this->program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragmentshader.glsl");
  this->program->link();

  this->createCube();
  this->createVBO();
}


void RenderWidget::paintGL()
{
  this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  this->glBindVertexArray(VAO);

  this->program->bind();

  this->view = this->camera->getViewMatrix();
  this->proj = this->camera->getProjectionMatrix();
  this->model = glm::mat4();
  this->model = glm::scale(this->model, glm::vec3(1.0f, 1.0f, 1.0f));

  QMatrix4x4 m(glm::value_ptr(glm::transpose(this->model)));
  QMatrix4x4 v(glm::value_ptr(glm::transpose(this->view)));
  QMatrix4x4 p(glm::value_ptr(glm::transpose(this->proj)));

  QMatrix4x4 mv = v * m;
  QMatrix4x4 mvp = p * mv;
  this->program->setUniformValue("mv", mv);
  this->program->setUniformValue("mv_ti", mv.inverted().transposed());
  this->program->setUniformValue("mvp", mvp);

  this->program->setUniformValue("hasWireframe", this->hasWireframe);
  this->program->setUniformValue("hasOcclusion", this->hasOcclusion);

  this->glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
//  this->glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
}


void RenderWidget::resizeGL(int width, int height)
{
  this->glViewport(0, 0, width, height);
  this->camera->updateWH(width, height);
}


void RenderWidget::keyPressEvent(QKeyEvent *event)
{
  glm::vec3 movementUnitDirection;
  bool hasMoved = true;
  bool keyPressed = true;
  switch(event->key())
  {
    case Qt::Key_W:
    case Qt::Key_Up:
      movementUnitDirection = this->camera->getForwardUnitVector();
      break;
    case Qt::Key_S:
    case Qt::Key_Down:
      movementUnitDirection = this->camera->getBackwardUnitVector();
      break;
    case Qt::Key_A:
    case Qt::Key_Left:
      movementUnitDirection = this->camera->getLeftUnitVector();
      break;
    case Qt::Key_D:
    case Qt::Key_Right:
      movementUnitDirection = this->camera->getRightUnitVector();
      break;
    case Qt::Key_Space:
      movementUnitDirection = this->camera->getAboveUnitVector();
      break;
    case Qt::Key_Z:
      movementUnitDirection = this->camera->getBelowUnitVector();
      break;
    case Qt::Key_1:
      this->hasWireframe = !this->hasWireframe;
      hasMoved = false;
      break;
    case Qt::Key_2:
      this->hasOcclusion = !this->hasOcclusion;
      hasMoved = false;
      break;
    default:
      hasMoved = false;
      keyPressed = false;
      break;
  }

  if(keyPressed || hasMoved)
  {
    this->camera->moveDelta(movementUnitDirection * 0.3 * (event->modifiers() & Qt::ShiftModifier ? 2.0f : 1.0f));
    this->update();
  }
}

void RenderWidget::keyReleaseEvent(QKeyEvent *event)
{
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
  this->isMovingCamera = true;

  this->screenCoordinatesOnMouseDown = glm::vec2(event->x(), event->y());

  QCursor cursor = this->cursor();
  cursor.setShape(Qt::BlankCursor);
  this->setCursor(cursor);
}


void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
  this->isMovingCamera = false;

  QCursor cursor = this->cursor();
  cursor.setShape(Qt::ArrowCursor);
  this->setCursor(cursor);
}


void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
  if(this->isMovingCamera)
  {
    glm::vec2 currentScreenCoordinates = glm::vec2(event->x(), event->y());

    glm::vec2 deltaScreenCoordinates = currentScreenCoordinates - this->screenCoordinatesOnMouseDown;
    deltaScreenCoordinates.x /= this->width();
    deltaScreenCoordinates.y /= this->height();

    this->camera->lookRotateHorizontal(deltaScreenCoordinates.x * -360.0f);
    this->camera->lookRotateVertical(deltaScreenCoordinates.y * -360.0f);

    QCursor cursor = this->cursor();
    cursor.setPos(this->mapToGlobal(QPoint(this->screenCoordinatesOnMouseDown.x, this->screenCoordinatesOnMouseDown.y)));
    this->setCursor(cursor);

    this->update();
  }
}


void RenderWidget::wheelEvent(QWheelEvent *event)
{
  this->camera->increaseZoomBy(0.005 * event->delta());
  this->update();
}


void RenderWidget::createCube()
{
//  vertices = {
//      { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 },
//      { +1, -1, -1 }, { +1, -1, -1 }, { +1, -1, -1 },
//      { +1, -1, +1 }, { +1, -1, +1 }, { +1, -1, +1 },
//      { -1, -1, +1 }, { -1, -1, +1 }, { -1, -1, +1 },
//      { -1, +1, -1 }, { -1, +1, -1 }, { -1, +1, -1 },
//      { +1, +1, -1 }, { +1, +1, -1 }, { +1, +1, -1 },
//      { +1, +1, +1 }, { +1, +1, +1 }, { +1, +1, +1 },
//      { -1, +1, +1 }, { -1, +1, +1 }, { -1, +1, +1 }
//  };

//  normals = {
//      {  0, -1,  0 }, { -1,  0,  0 }, {  0,  0, -1 },
//      {  0, -1,  0 }, { +1,  0,  0 }, {  0,  0, -1 },
//      {  0, -1,  0 }, { +1,  0,  0 }, {  0,  0, +1 },
//      {  0, -1,  0 }, { -1,  0,  0 }, {  0,  0, +1 },
//      { -1,  0,  0 }, {  0,  0, -1 }, {  0, +1,  0 },
//      { +1,  0,  0 }, {  0,  0, -1 }, {  0, +1,  0 },
//      { +1,  0,  0 }, {  0,  0, +1 }, {  0, +1,  0 },
//      { -1,  0,  0 }, {  0,  0, +1 }, {  0, +1,  0 }
//  };

//  indices = {
//      0,   3,  6, //normal: (  0, -1,  0 )
//      0,   6,  9, //normal: (  0, -1,  0 )
//      12,  1, 10, //normal: ( -1,  0,  0 )
//      12, 10, 21, //normal: ( -1,  0,  0 )
//      18,  7,  4, //normal: ( +1,  0,  0 )
//      18,  4, 15, //normal: ( +1,  0,  0 )
//      22, 11,  8, //normal: (  0,  0, +1 )
//      22,  8, 19, //normal: (  0,  0, +1 )
//      16,  5,  2, //normal: (  0,  0, -1 )
//      16,  2, 13, //normal: (  0,  0, -1 )
//      23, 20, 17, //normal: (  0, +1,  0 )
//      23, 17, 14  //normal: (  0, +1,  0 )
//  };

  Mesh mesh;
  mesh.loadObj("C:\\Users\\Ian Albuquerque\\Desktop\\mesh_processing\\3drenderer\\models\\venus_100759.obj");
//  mesh.loadPyramid();
//  mesh.getTriangles(&(this->vertices), &(this->normals), &(this->indices));
//  for(int i=0; i<this->vertices.size(); i++)
//  {
//    this->vertices[i] *= -2.0f;
//    this->normals[i] *= -2.0f;
//  }
  mesh.getTriangles(&(this->vertices), &(this->normals), &(this->indices));

//  vertices = {
//    {0, 0, 0}, {1, 0, 0}, {0, 1, 0},
//    {0, 0, 1}, {1, 0, 1}, {0, 1, 1}
//   };

//  normals = {
//    {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
//    {0, 0, 1}, {0, 0, 1}, {0, 0, 1}
//  };
}

void RenderWidget::createVBO()
{
  struct vertex
  {
      glm::vec3 pos;
      glm::vec3 normal;
  };

  std::vector<vertex> vbo;
  vbo.reserve(this->vertices.size());
  for (unsigned int i = 0; i < vertices.size(); i++)
  {
    vbo.push_back({vertices[i], normals[i]});
  }

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(vertex), &vbo[0], GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  glBindBuffer( GL_ARRAY_BUFFER, VBO );

  // vertexPos
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0 );

  // vertexNormal
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)sizeof(glm::vec3) );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

