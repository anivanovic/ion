#include "Mesh.h"
#include <vector>
#include <iostream>

Mesh::Mesh(Vertex* vertices, unsigned int numVertices) {
	m_drawCount = numVertices;
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	std::vector<glm::vec3> positions;
	// std::vector<glm::vec2> texCoor;

	positions.reserve(numVertices);
	// texCoor.reserve(numVertices);
	for (auto i = 0; i < numVertices; i++) {
		positions.push_back(*vertices[i].GetPos());
		glm::vec3* point = vertices[i].GetPos();
		// texCoor.push_back(*vertices[i].GetTexCoor());
	}

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	// glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[TEX_COORD_VB]);
	// glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texCoor[0]), &texCoor[0], GL_STATIC_DRAW);

	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// glBindVertexArray(0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw() {
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_LINE_STRIP, 0, m_drawCount);
	glBindVertexArray(0);
}