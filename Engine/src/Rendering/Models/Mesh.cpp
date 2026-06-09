#include "Mesh.h"

Mesh::Mesh(std::vector<sVertex> vertices, std::vector<unsigned int> indices, EngineAssets::Material* material) {
	mVertices = vertices;
	mIndices = indices;
	mMaterial = material;

	SetupMesh();
}

void Mesh::Draw() {
	mMaterial->Activate();
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh() {
	// Create buffers
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(sVertex), &mVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	// Load vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, texCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}