#pragma once 

#include "BH3D_Drawable.hpp"


enum CubeStatus
{
	CUBESTATUS_EMPTY,
	CUBESTATUS_FULL,
	CUBESTATUS_MAGIC,
	CUBESTATUS_UNDESTRUCTIBLE,
	CUBESTATUS_COUNT
};

struct CubeLogic
{
	//Constant variable in time
	int m_id = 0;
	int m_status = 0;
	glm::ivec3 m_position;

	//variable in time
	glm::mat4 m_translate;
};


struct Animation
{
	float m_durations = 15.0f; //! Duration in sec of the animation
};


struct RotationAnimation : public Animation
{
	float m_angle = 0;
	float m_speed = glm::half_pi<float>() / m_durations;
	glm::vec3 m_axe = { 1.0, 0.0, 0.0 };	//! Rotation axes

	inline auto compute(float elapse_time) {
		float elapse = m_speed * elapse_time;
		m_angle += elapse;
		return glm::rotate(glm::mat4(1.0f), m_angle, m_axe);
	}

	void reset() {
		float m_angle = 0;
		m_speed = glm::half_pi<float>() / m_durations;
	}
};

struct TranslationAnimation : public Animation
{
	glm::vec3 m_position = { 0.0, 0.0, 0.0 };
	float m_speed = 1.0f / m_durations;
	glm::vec3 m_direction = { 0.0, 0.0, 1.0 };

	inline auto compute(float elapse_time) {
		float elapse = m_speed * elapse_time;
		m_position += (elapse * m_direction);
		return glm::translate(glm::mat4(1.0f), m_position);
	}

	void reset() {
		m_position = { 0.0, 0.0, 0.0 };
		float m_speed = 1.0f / m_durations;
	}
};



class SavageCubeMatrix : public bh3d::Drawable
{
	glm::vec3 m_cube_size = { 0.99f, 0.25f, 0.99f };

	int m_cols = 8;
	int m_rows = 32;

	bh3d::Texture m_texture;

	std::vector<CubeLogic> m_vCubeLogics;
	std::vector<bh3d::Texture> m_vTextures;

	RotationAnimation m_rotationAnimation;
	TranslationAnimation m_translationAnimation;

public:
	SavageCubeMatrix() {}
	SavageCubeMatrix(const glm::vec3 & cube_size) :
		m_cube_size(cube_size)
	{}

	void Init(int rows, int cols);

	void Draw(const glm::mat4& mvp) override
	{
		m_mesh.BindMaterial(0);
		m_mesh.BindVBO();
		for (auto& cube : m_vCubeLogics)
		{
			m_shader(mvp * cube.m_translate);
			m_mesh.DrawSubMeshElements(0);
		}
	}

	void DrawAnimation(const glm::mat4& mvp, float elapse_time = 1.0f / 60.0f)
	{
		auto rotation_mat = m_rotationAnimation.compute(elapse_time);
		auto translation_mat = m_translationAnimation.compute(elapse_time);

		auto anim_mat = translation_mat * rotation_mat;

		m_mesh.BindVBO();
		for (auto& cube : m_vCubeLogics)
		{
			assert(cube.m_status < m_vTextures.size());
			m_vTextures[cube.m_status].Bind();
			m_shader(mvp * cube.m_translate * anim_mat);
			m_mesh.DrawSubMeshElements(0);
		}
	}


};