#ifndef _MESH_INCLUDED_H_
#define _MESH_INCLUDED_H_

#include <string>
#include <glm/glm.hpp>
#include <OpenGL/gl3.h>

class Vertex {
public:
    Vertex(const glm::vec3& pos, const glm::vec2& texCoor): pos(pos), texCoor(texCoor) {}

    inline glm::vec3* GetPos() { return &pos; }
    inline glm::vec2* GetTexCoor() { return &texCoor; }
protected:
private:
    glm::vec3 pos;
    glm::vec2 texCoor;
};


class Mesh {
public:
    Mesh(const std::string& fileName);
	Mesh(Vertex* vertices, unsigned int numVertices);

	void Draw();

	virtual ~Mesh();
protected:
private:
	void operator=(const Mesh& mesh) {}
	Mesh(const Mesh& mesh) {}

    enum {
        POSITION_VB,
        // TEX_COORD_VB,
        NUM_BUFFERS
    };

    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffer[NUM_BUFFERS];
    unsigned int m_drawCount;
};

#endif