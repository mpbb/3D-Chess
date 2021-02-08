#include <vector>
#include <fstream>
#include <string>

#include <glm/glm.hpp>

#include "objloader.h"

void loadOBJ(std::string path, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals)
{
	std::vector<int> vertexIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;

	std::ifstream objFile(path);
	std::string header;

	while (objFile >> header)
	{

		if (!header.compare("v"))
		{
			glm::vec3 vertex;
			objFile >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (!header.compare("vn"))
		{
			glm::vec3 normal;
			objFile >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (!header.compare("f"))
		{

			for (int i = 0; i < 3; i++)
			{
				int v, vn;
				objFile >> v;
				objFile.ignore();
				objFile.ignore(256, '/');
				objFile >> vn;

				vertexIndices.push_back(v);
				normalIndices.push_back(vn);
			}
		}
		else
		{
			objFile.ignore(256, '\n');
		}

	}

	for (int i = 0; i < vertexIndices.size(); i++)
	{
		glm::vec3 vertex = temp_vertices[vertexIndices[i] - 1];
		glm::vec3 normal = temp_normals[normalIndices[i] - 1];

		vertices.push_back(vertex);
		normals.push_back(normal);
	}

	objFile.close();

	return;
}