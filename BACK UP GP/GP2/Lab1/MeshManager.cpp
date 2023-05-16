#include "MeshManager.h"
#include <vector>


MeshManager::MeshManager()
{
	// init variables
	drawCounter = NULL;
	VAObject = NULL;
}

MeshManager::~MeshManager()
{
	glDeleteVertexArrays(1, &VAObject); // delete arrays
}

void MeshManager::Intitalize(Vertex* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount)
{
	// reference to mesh
	IndexedModel model;

	// loops through all vertices 
	for (unsigned int i = 0; i < verticesCount; i++)
	{
		// reads in pos and texture coords 
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}

	// loops through indices and assigns them to model
	for (unsigned int i = 0; i < indicesCount; i++)
		model.indices.push_back(indices[i]);

	IntitalizeModel(model);
}

void MeshManager::IntitalizeModel(const IndexedModel& model)
{

	drawCounter = model.indices.size();

	glGenVertexArrays(1, &VAObject); //creates vertex array and stores it in VAO
	glBindVertexArray(VAObject); //bins the VAO

	glGenBuffers(4, VABuffer); //creates buffer

	glBindBuffer(GL_ARRAY_BUFFER, VABuffer[0]); //binds the buffer
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW); //creates a buffer objects data store
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // defines an array of generic vertex attribute data

	glBindBuffer(GL_ARRAY_BUFFER, VABuffer[1]); 
	glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW); 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VABuffer[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VABuffer[3]); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW); 

	glBindVertexArray(0); // unbind our VAO
}

void MeshManager::ModelLoader(const std::string& filename)
{
	// loads model 
	IndexedModel model = OBJModel(filename).ToIndexedModel();
	IntitalizeModel(model);
}

// draws object
void MeshManager::Draw()
{
	glBindVertexArray(VAObject); // binds VAO

	glDrawElements(GL_TRIANGLES, drawCounter, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

// updates col pos and scales
void MeshManager::UpdateColData(glm::vec3 pos, float rad)
{
	colMesh.SetPos(pos);
	colMesh.SetRad(rad);
}