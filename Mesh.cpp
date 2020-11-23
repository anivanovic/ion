#include "Mesh.h"
#include <vector>
#include <iostream>

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, GLenum mode) {
	m_drawCount = numIndices;
	m_mode = mode;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoor;

	positions.reserve(numVertices);
	texCoor.reserve(numVertices);
	for (auto i = 0; i < numVertices; i++) {
		positions.push_back(*vertices[i].GetPos());
		texCoor.push_back(*vertices[i].GetTexCoor());
	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(NUM_BUFFERS, m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEX_COORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texCoor[0]), &texCoor[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	// glBindVertexArray(0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::Draw() {
	glBindVertexArray(m_VAO);
	glDrawElements(m_mode, m_drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}