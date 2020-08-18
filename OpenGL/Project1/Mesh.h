#ifndef _MESH_H_
#define _MESH_H_

#include <glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Shader.h"

#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);
	
	void Draw(Shader& shader);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;

private:
	void _SetUpMesh();

	unsigned int VBO, EBO;
};

#endif