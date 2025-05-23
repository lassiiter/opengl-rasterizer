#pragma once
#include <map>

namespace nshaders
{

	// Utitlity class to load, compile and attach a vertex- and fragment shader to a program
	class Shader
	{

	private:
		unsigned int mProgramId;

		unsigned int get_compiled_shader(unsigned int shader_type, const std::string& shader_source);

		std::map<std::string, int> texture_uniform_id;
		std::map<std::string, int> texture_unit_map;


	public:
		Shader() {
			mProgramId = -1;
			texture_uniform_id["albedoTex"] = 3;
			texture_uniform_id["ormTex"] = 4;
			texture_uniform_id["emissiveTex"] = 5;
			texture_uniform_id["skyboxTex"] = 6;
			texture_uniform_id["irradianceTex"] = 7;
			texture_uniform_id["radianceTex"] = 8;
			texture_uniform_id["normalTex"] = 9;

			texture_unit_map["albedoTex"] = GL_TEXTURE3;
			texture_unit_map["ormTex"] = GL_TEXTURE4;
			texture_unit_map["emissiveTex"] = GL_TEXTURE5;
			texture_unit_map["skyboxTex"] = GL_TEXTURE6;
			texture_unit_map["irradianceTex"] = GL_TEXTURE7;
			texture_unit_map["radianceTex"] = GL_TEXTURE8;
			texture_unit_map["normalTex"] = GL_TEXTURE9;

		}

		// Load a vertex and a fragment shader from file
		bool load(const std::string& vertexshader_file, const std::string& fragmentshader_file);

		// Use the program
		void use();

		// Delete the program
		void unload();

		unsigned int get_program_id() { return mProgramId; }

		void set_mat4(const glm::mat4& mat4, const std::string& name);

		void set_i1(int v, const std::string& name);
		void set_f1(float v, const std::string& name);
		void set_f3(float a, float b, float c, const std::string& name);

		void set_vec3(const glm::vec3& vec3, const std::string& name);
		void set_vec4(const glm::vec4& vec4, const std::string& name);

		void set_tex(const std::string& filePath, const std::string& name);
		void set_tex_hdr(const std::string& filePath, const std::string& name);

	};
}

