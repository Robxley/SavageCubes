#include "SavageCubeMatrix.h"

#include "BH3D_Cube.hpp"
#include "BH3D_SDLTextureManager.hpp"
#include "BH3D_TinyShader.hpp"

#include <random>

namespace
{
	std::vector<std::string> GetCubeTextures()
	{
		static std::vector <std::string> vTextures =
		{
			"data3d/textures/floor.png",
			"data3d/textures/metal.png",
			"data3d/textures/metal2.png",
			"data3d/textures/tnt2.png",
			"data3d/textures/tnt3.png"
		};
		return vTextures;
	}
}

void SavageCubeMatrix::Init(int rows, int cols)
{
	
	this->m_rows = rows;
	this->m_cols = cols;

	if (!m_shader.IsValid())
	{
		m_shader.LoadRaw(bh3d::TinyShader::TEXTURE_VERTEX(), bh3d::TinyShader::TEXTURE_FRAGMENT());
		assert(m_shader.IsValid());
	}

	if (!m_mesh.IsValid())
	{
		bh3d::Cube::AddSubMesh(m_mesh, m_cube_size);
		m_mesh.ComputeMesh();
		assert(m_mesh.IsValid());
	}

	if (m_vTextures.empty())
	{
		auto vTexturePaths = GetCubeTextures();
		for (auto& path : vTexturePaths)
		{
			auto tex = BH3D_LoadTexture(path);
			if(tex.IsValid())
				m_vTextures.emplace_back(tex);
		}

		if(!m_vTextures.empty())
			m_mesh.SetTexture(m_vTextures[0]);
	}
	assert(!m_vTextures.empty());

	m_vCubeLogics.clear();
	m_vCubeLogics.reserve((std::size_t)m_cols * m_rows);


	std::random_device rd;			//Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd());			//Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dist(0, (int)m_vTextures.size()-1);


	int k = 0;
	for (int i = 0; i < m_cols; i++)
	{
		for (int j = 0; j < m_rows; j++)
		{
			m_vCubeLogics.emplace_back(CubeLogic{
				k,
				dist(gen),
				{i,j,k},
				glm::translate(glm::mat4(1.0f), {i, 0, j})
			});

			k++;
		}
	}

	m_rotationAnimation.reset();
	m_translationAnimation.reset();

}
