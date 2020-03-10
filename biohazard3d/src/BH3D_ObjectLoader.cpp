
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "BH3D_ObjectLoader.hpp"

struct compare_vec3
{
	bool operator()(const glm::vec3 & v1, const glm::vec3 & v2) const 
	{
		for (int i = 0; i < glm::vec3::length(); i++) {
			if (v1[i] < v2[i]) return true;
			if (v1[i] > v2[i]) return false;
 		}
		return false;
	}
};

namespace bh3d
{
#pragma pack(push, 1)		//Ensure that no padding are added to the structure
	struct STLTriangleFormat
	{
		glm::vec3 m_normal;		//! 3 components of the normal vector to the triangle
		glm::vec3 m_points[3];	//! 3 coordinates of the three vertices of the triangle
		char misc_data[2];			//! misc data for each triangle (see STL format description)
		constexpr static auto byte_size() { 
			constexpr auto data_size = sizeof(glm::vec3) * 4 + 2;
			static_assert(data_size == sizeof(STLTriangleFormat)); //Checking of no padding
			return data_size;
		}
	};
#pragma pack(pop)



	bool ObjectLoader::LoadBinary(Mesh & mesh) const
	{

		//Binary STL file format based on the following description :
		//https://en.wikipedia.org/wiki/STL_(file_format)


		assert(std::filesystem::exists(this->m_filepath));
		assert(m_filepath.extension() == ".stl" || m_filepath.extension() == ".STL");

		std::ifstream file;

		file.open(m_filepath, std::ifstream::in | std::ifstream::binary);
		assert(!file.fail());
		if (file.fail())
		{
			BH3D_LOGGER_WARNING("ERROR: STL file could not be opened! - "<< m_filepath);
			return false;
		}

		std::string stl_name(81, 0);
		file.read(stl_name.data(), 80);
		if (!file) {
			BH3D_LOGGER_WARNING("Unsupported STL format - " << m_filepath);
			return false;
		}

		// position the pointer to byte number 80 (see the file format)
		file.seekg(80);

		// read the number of triangles in the STL geometry
		int triangleNumbers = 0;
		file.read((char*)&(triangleNumbers), sizeof(int));
	
		std::map<glm::vec3, std::vector<unsigned int>, compare_vec3> mapSmartVertices;
		using uint = unsigned int;
		auto AddPositions = [&, jj = uint(0)]  (const glm::vec3 & pos) mutable {
			auto & vertices = mapSmartVertices[pos];
			vertices.push_back(jj++);
		};

		std::vector<glm::vec3> vTriNormals(triangleNumbers);

		STLTriangleFormat triangle;
		for (int i = 0; i < triangleNumbers; i++) 
		{
			file.read((char*)(&triangle), STLTriangleFormat::byte_size());
			AddPositions(triangle.m_points[0]);
			AddPositions(triangle.m_points[1]);
			AddPositions(triangle.m_points[2]);
			vTriNormals[i] = triangle.m_normal;
		}

		//fill a continuous vertex array and update the surface array with the vertex ids.
		std::vector<glm::vec3> vPositions;
		std::vector<glm::vec3> vNormals;

		std::vector<Face> vFaces(triangleNumbers);
		vPositions.reserve(mapSmartVertices.size());
		vNormals.reserve(mapSmartVertices.size());
		int i = 0;
		for (auto & pair : mapSmartVertices) 
		{
			vPositions.emplace_back(pair.first); //fill a continuous vertex array
			
			glm::vec3 normal_smooth = { 0.0f, 0.0f, 0.0f };
			//update each linked triangle
			for (auto id : pair.second)
			{
				auto face_id = id / 3;	//Linked triangle
				auto sub_id = id % 3;	//order indice in the triangle
				vFaces[face_id].id[sub_id] = i;
				normal_smooth += vTriNormals[face_id];
			}
			vNormals.emplace_back(glm::normalize(normal_smooth));
			i++;
		}

		mesh.AddSubMesh(vFaces, vPositions, {}, vNormals);
		mesh.NormalizeData();
		mesh.CenterDataToOrigin();
		mesh.ComputeMesh();

		return true;
	}


}