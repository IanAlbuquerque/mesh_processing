#include "mesh.h"

#include "glm/glm.hpp"

Vertex::Vertex(float x, float y, float z)
{
  this->position = glm::vec3(x, y, z);
}

Face::Face()
{

}

Halfedge::Halfedge()
{

}

Mesh::Mesh()
{

}


void Mesh::loadPyramid()
{
  this->vertices.push_back(new Vertex(-1.0f, -1.0f, -1.0f));
  this->vertices.push_back(new Vertex(1.0f, -1.0f, -1.0f));
  this->vertices.push_back(new Vertex(1.0f, -1.0f, 1.0f));
  this->vertices.push_back(new Vertex(-1.0f, -1.0f, 1.0f));
  this->vertices.push_back(new Vertex(0.0f, 1.0f, 0.0f));
  for(int i=0; i<5; i++)
    this->faces.push_back(new Face());
  for(int i=0; i<16; i++)
    this->halfedges.push_back(new Halfedge());

  this->vertices[0]->halfedge = this->halfedges[0];
  this->vertices[1]->halfedge = this->halfedges[1];
  this->vertices[2]->halfedge = this->halfedges[2];
  this->vertices[3]->halfedge = this->halfedges[3];
  this->vertices[4]->halfedge = this->halfedges[6];

  this->faces[0]->halfedge = this->halfedges[0];
  this->faces[1]->halfedge = this->halfedges[4];
  this->faces[2]->halfedge = this->halfedges[7];
  this->faces[3]->halfedge = this->halfedges[10];
  this->faces[4]->halfedge = this->halfedges[13];

  this->halfedges[0]->vertex = this->vertices[0];
  this->halfedges[1]->vertex = this->vertices[1];
  this->halfedges[2]->vertex = this->vertices[2];
  this->halfedges[3]->vertex = this->vertices[3];
  this->halfedges[4]->vertex = this->vertices[2];
  this->halfedges[5]->vertex = this->vertices[1];
  this->halfedges[6]->vertex = this->vertices[4];
  this->halfedges[7]->vertex = this->vertices[1];
  this->halfedges[8]->vertex = this->vertices[0];
  this->halfedges[9]->vertex = this->vertices[4];
  this->halfedges[10]->vertex = this->vertices[0];
  this->halfedges[11]->vertex = this->vertices[3];
  this->halfedges[12]->vertex = this->vertices[4];
  this->halfedges[13]->vertex = this->vertices[3];
  this->halfedges[14]->vertex = this->vertices[2];
  this->halfedges[15]->vertex = this->vertices[4];

  this->halfedges[0]->face = this->faces[0];
  this->halfedges[1]->face = this->faces[0];
  this->halfedges[2]->face = this->faces[0];
  this->halfedges[3]->face = this->faces[0];
  this->halfedges[4]->face = this->faces[1];
  this->halfedges[5]->face = this->faces[1];
  this->halfedges[6]->face = this->faces[1];
  this->halfedges[7]->face = this->faces[2];
  this->halfedges[8]->face = this->faces[2];
  this->halfedges[9]->face = this->faces[2];
  this->halfedges[10]->face = this->faces[3];
  this->halfedges[11]->face = this->faces[3];
  this->halfedges[12]->face = this->faces[3];
  this->halfedges[13]->face = this->faces[4];
  this->halfedges[14]->face = this->faces[4];
  this->halfedges[15]->face = this->faces[4];

  this->halfedges[0]->next = this->halfedges[1];
  this->halfedges[1]->next = this->halfedges[2];
  this->halfedges[2]->next = this->halfedges[3];
  this->halfedges[3]->next = this->halfedges[0];
  this->halfedges[4]->next = this->halfedges[5];
  this->halfedges[5]->next = this->halfedges[6];
  this->halfedges[6]->next = this->halfedges[4];
  this->halfedges[7]->next = this->halfedges[8];
  this->halfedges[8]->next = this->halfedges[9];
  this->halfedges[9]->next = this->halfedges[7];
  this->halfedges[10]->next = this->halfedges[11];
  this->halfedges[11]->next = this->halfedges[12];
  this->halfedges[12]->next = this->halfedges[10];
  this->halfedges[13]->next = this->halfedges[14];
  this->halfedges[14]->next = this->halfedges[15];
  this->halfedges[15]->next = this->halfedges[13];

  this->halfedges[0]->prev = this->halfedges[3];
  this->halfedges[1]->prev = this->halfedges[0];
  this->halfedges[2]->prev = this->halfedges[1];
  this->halfedges[3]->prev = this->halfedges[2];
  this->halfedges[4]->prev = this->halfedges[6];
  this->halfedges[5]->prev = this->halfedges[4];
  this->halfedges[6]->prev = this->halfedges[5];
  this->halfedges[7]->prev = this->halfedges[9];
  this->halfedges[8]->prev = this->halfedges[7];
  this->halfedges[9]->prev = this->halfedges[8];
  this->halfedges[10]->prev = this->halfedges[12];
  this->halfedges[11]->prev = this->halfedges[10];
  this->halfedges[12]->prev = this->halfedges[11];
  this->halfedges[13]->prev = this->halfedges[15];
  this->halfedges[14]->prev = this->halfedges[13];
  this->halfedges[15]->prev = this->halfedges[14];

  this->halfedges[0]->opposite = this->halfedges[7];
  this->halfedges[1]->opposite = this->halfedges[4];
  this->halfedges[2]->opposite = this->halfedges[13];
  this->halfedges[3]->opposite = this->halfedges[10];
  this->halfedges[4]->opposite = this->halfedges[1];
  this->halfedges[5]->opposite = this->halfedges[9];
  this->halfedges[6]->opposite = this->halfedges[14];
  this->halfedges[7]->opposite = this->halfedges[0];
  this->halfedges[8]->opposite = this->halfedges[12];
  this->halfedges[9]->opposite = this->halfedges[5];
  this->halfedges[10]->opposite = this->halfedges[3];
  this->halfedges[11]->opposite = this->halfedges[15];
  this->halfedges[12]->opposite = this->halfedges[8];
  this->halfedges[13]->opposite = this->halfedges[2];
  this->halfedges[14]->opposite = this->halfedges[6];
  this->halfedges[15]->opposite = this->halfedges[11];
}

void Mesh::getTriangles(std::vector<glm::vec3>* vertices, std::vector<glm::vec3>* normals, std::vector<unsigned int>* indices)
{
  for(int i=0; i<this->faces.size(); i++)
  {
    Halfedge* he = this->faces[i]->halfedge;

    Vertex* v1 = he->vertex;
    Vertex* v2 = he->next->vertex;
    Vertex* v3 = he->next->next->vertex;
    glm::vec3 normal = glm::normalize(glm::cross(v2->position - v1->position, v3->position - v1->position));

    Halfedge* he_i = he;
    int numVertices = 0;
    int vertexInitialIndex = vertices->size();
    do
    {
      vertices->push_back(he_i->vertex->position);
      normals->push_back(normal);
      he_i = he_i->next;
      numVertices++;
    }
    while(he_i != he);

    for(int j=0; j<numVertices - 2; j++)
    {
      indices->push_back(vertexInitialIndex);
      indices->push_back(vertexInitialIndex+j+1);
      indices->push_back(vertexInitialIndex+j+2);
    }
  }
}
