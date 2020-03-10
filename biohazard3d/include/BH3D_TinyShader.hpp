#pragma once

#ifndef _BH3D_TINY_SHADER_H_
#define _BH3D_TINY_SHADER_H_

/// <summary>
/// Basic shader used by default in TinyEngine  (Engine example using Biohasard3D)
/// </summary>
namespace bh3d
{
	namespace TinyShader
	{
		inline constexpr const char * DEFAULT_VERTEX() {
			return
				"																																		\n \
				#version 330 core\n																														\n \
																																						\n \
				layout(location = 0) in vec3 in_Position;		// the position variable has attribute position 0										\n \
				layout(location = 1) in vec4 in_Color;			// the color variable has attribute position 1											\n \
				layout(location = 2) in vec2 in_Coord0;			// the texture variable has attribute position 2										\n \
																																						\n \
				out vec4  vert_color;							// specify a color output to the fragment shader										\n \
				out vec2  vert_texcoord;						// specify a color output to the fragment shader										\n \
																																						\n \
				uniform mat4 proj_view_transform;				//Projection * modelview * transform matrix												\n \
																																						\n \
				void main()																																\n \
				{																																		\n \
					gl_Position = proj_view_transform * vec4(in_Position, 1.0);					// vertex projection on the screen						\n \
					vert_color = in_Color;														// forward color vertex									\n \
					vert_texcoord = in_Coord0;													// forward texture vertex								\n \
				}																																		\n \
			";
		}


		inline constexpr const char * DEFAULT_FRAGMENT() {
			return
				"																										\n \
				#version 330 core 																						\n \
				out vec4 FragColor;																						\n \
																														\n \
				in vec4 vert_color; // the input variable from the vertex shader (same name and same type)  			\n \
				in vec2 vert_texcoord;																					\n \
																														\n \
				uniform sampler2D textureSampler;																		\n \
																														\n \
				void main()																								\n \
				{																										\n \
					vec4 tex_color = texture2D(textureSampler, vert_texcoord);											\n \
					FragColor = (vert_color * tex_color);																\n \
				}																										\n \
			";
		}

		inline constexpr const char * DEFAULT_GREY_FRAGMENT() {
			return
				"																										\n \
				#version 330 core 																						\n \
				out vec4 FragColor;																						\n \
																														\n \
				in vec4 vert_color; // the input variable from the vertex shader (same name and same type)  			\n \
				in vec2 vert_texcoord;																					\n \
																														\n \
				uniform sampler2D textureSampler;																		\n \
																														\n \
				void main()																								\n \
				{																										\n \
					vec4 tex_color = texture2D(textureSampler, vert_texcoord);											\n \
					FragColor = (vert_color * tex_color.r);																\n \
				}																										\n \
			";
		}

		inline constexpr const char * COLOR_VERTEX() {
			return
				"																																		\n \
				#version 330 core\n																														\n \
																																						\n \
				layout(location = 0) in vec3 in_Position;		// the position variable has attribute position 0										\n \
				layout(location = 1) in vec4 in_Color;			// the color variable has attribute position 1											\n \
																																						\n \
				out vec4  vert_color;							// specify a color output to the fragment shader										\n \
																																						\n \
				uniform mat4 proj_view_transform;				//Projection * modelview * transform matrix												\n \
																																						\n \
				void main()																																\n \
				{																																		\n \
					gl_Position = proj_view_transform * vec4(in_Position, 1.0);					// vertex projection on the screen						\n \
					vert_color = in_Color;														// forward color vertex									\n \
				}																																		\n \
			";
		}

		inline constexpr const char * COLOR_FRAGMENT() {
			return
				"																										\n \
				#version 330 core 																						\n \
				out vec4 FragColor;																						\n \
				in vec4 vert_color; // the input variable from the vertex shader (same name and same type)  			\n \
				void main()																								\n \
				{																										\n \
					FragColor = vert_color;																				\n \
				}																										\n \
			";
		}

		inline constexpr const char * TEXTURE_VERTEX() {
			return
				"																																		\n \
				#version 330 core\n																														\n \
																																						\n \
				layout(location = 0) in vec3 in_Position;		// the position variable has attribute position 0										\n \
				layout(location = 2) in vec2 in_Coord0;			// the texture variable has attribute position 2										\n \
																																						\n \
				out vec2  vert_texcoord;						// specify a color output to the fragment shader										\n \
																																						\n \
				uniform mat4 proj_view_transform;				//Projection * modelview * transform matrix												\n \
																																						\n \
				void main()																																\n \
				{																																		\n \
					gl_Position = proj_view_transform * vec4(in_Position, 1.0);					// vertex projection on the screen						\n \
					vert_texcoord = in_Coord0;													// forward texture vertex								\n \
				}																																		\n \
			";
		}

		inline constexpr const char * TEXTURE_FRAGMENT() {
			return
				"																										\n \
				#version 330 core 																						\n \
				out vec4 FragColor;																						\n \
				in vec2 vert_texcoord;																					\n \
				uniform sampler2D textureSampler;																		\n \
				void main()																								\n \
				{																										\n \
					FragColor = texture2D(textureSampler, vert_texcoord);												\n \
				}																										\n \
			";
		}

		inline constexpr const char * TEXTURE_GREY_FRAGMENT() {
			return
				"																										\n \
				#version 330 core 																						\n \
				out vec4 FragColor;																						\n \
				in vec2 vert_texcoord;																					\n \
				uniform sampler2D textureSampler;																		\n \
				void main()																								\n \
				{																										\n \
					vec4 color = texture2D( textureSampler, vert_texcoord );											\n \
					FragColor = vec4(color.r, color.r, color.r, color.r);											    \n \
				}																										\n \
			";
		}

		inline constexpr const char * FONT_VERTEX() {
			return
				"																					\n \
				#version 330 core																	\n \
																									\n \
				layout(location = 0) in vec2 in_Vertex;												\n \
				layout(location = 2) in vec2 in_Coord0;												\n \
																									\n \
				out vec2 texCoord;																	\n \
																									\n \
				uniform mat4 projection;															\n \
				uniform ivec2 offset;																\n \
				void main()																			\n \
				{																					\n \
					texCoord = in_Coord0;															\n \
					gl_Position = projection * vec4(in_Vertex + offset, 1.0, 1.0);					\n \
				}																					\n \
			";
		}

		inline constexpr const char * FONT_FRAGMENT() {
			return
				"																					\n \
				#version 330 core																	\n \
																									\n \
				in vec2 texCoord;																	\n \
				out vec4 out_Color;																	\n \
																									\n \
				uniform sampler2D textureSampler;													\n \
				uniform vec4 uColor;																\n \
																									\n \
				void main()																			\n \
				{																					\n \
					vec4 color_t = texture2D(textureSampler, texCoord);								\n \
					out_Color = color_t * uColor;													\n \
				}																					\n \
			";
		}

	}

}  // Namespace bh3d

#endif
