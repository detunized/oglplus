/*
 *  .file advanced/spectra/shared_objects.cpp
 *  .brief Implements a class wrapping shared (GL) objects.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/opt/list_init.hpp>
#include <oglplus/opt/resources.hpp>

#include <oglplus/shapes/grid.hpp>
#include <oglplus/shapes/plane.hpp>

#include "shared_objects.hpp"

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cctype>
#include <stdexcept>

oglplus::Program SpectraSharedObjects::BuildProgram(const char* prog_name)
{
	using namespace oglplus;

	Program prog = Program(ObjectDesc(prog_name));

	std::ifstream prog_file;
	OpenResourceFile(prog_file, "glsl", prog_name, ".txt");

	const std::size_t linesize = 1024;
	char line[1024] = {'\0'};

	int line_no = 0;
	while(prog_file.getline(line, linesize).good())
	{
		std::size_t line_len = prog_file.gcount();
		++line_no;
		auto types = EnumValueRange<ShaderType>();
		ShaderType type;
		unsigned name_len = 0;
		while(!types.Empty())
		{
			type = types.Front();
			StrLit name = EnumValueName(type);
			name_len = name.size();
			if(std::strncmp(line, name.c_str(), name.size()) == 0)
				break;
			types.Next();
		}
		if(types.Empty())
		{
			wxString message = wxString::Format(
				wxGetTranslation(wxT(
					"Invalid shader type in program '%s' "\
					"on line %d: '%s'"
				)),
				wxString(prog_name, wxConvUTF8).c_str(),
				line_no,
				wxString(line, wxConvUTF8).c_str()
			);
			throw std::runtime_error(
				(const char*)message.mb_str(wxConvUTF8)
			);
		}

		// rtrim the name
		for(std::size_t i = name_len+1; i!=line_len; ++i)
		{
			if(std::isspace(line[i]))
			{
				line[i] = '\0';
				break;
			}
		}

		std::ifstream shader_file;
		OpenResourceFile(shader_file, "glsl", line+name_len+1, ".glsl");

		Shader shader(type);
		shader.Source(GLSLSource::FromStream(shader_file));
		shader.Compile();

		prog.AttachShader(shader);
	}

	prog.Link().Use();
	return std::move(prog);
}

SpectraSharedObjects::SpectraSharedObjects(void)
 : ground_unit_grid(
	oglplus::List("Position").Get(),
	oglplus::shapes::Grid(
		oglplus::Vec3f(0.5f, 0.0f, 0.5f),
		oglplus::Vec3f(0.5f, 0.0f, 0.0f),
		oglplus::Vec3f(0.0f, 0.0f, 0.5f),
		10,
		10
	)
), wall_unit_grid(
	oglplus::List("Position").Get(),
	oglplus::shapes::Grid(
		oglplus::Vec3f(0.5f, 0.5f, 0.0f),
		oglplus::Vec3f(0.5f, 0.0f, 0.0f),
		oglplus::Vec3f(0.0f, 0.5f, 0.0f),
		10,
		10
	)
)
{
}

const oglplus::shapes::ShapeWrapper& SpectraSharedObjects::GroundUnitGrid(void)
{
	return ground_unit_grid;
}

const oglplus::shapes::ShapeWrapper& SpectraSharedObjects::WallUnitGrid(void)
{
	return wall_unit_grid;
}

const oglplus::shapes::ShapeWrapper& SpectraSharedObjects::SpectrumPlane(
	GLuint samples_per_sec,
	GLuint spectrum_size
)
{
	std::pair<GLuint, GLuint> key(samples_per_sec, spectrum_size);

	spectrum_plane_map::iterator pos = spectrum_planes.find(key);
	if(pos == spectrum_planes.end())
	{
		pos = spectrum_planes.insert(
			spectrum_plane_map::value_type(
				key,
				oglplus::shapes::ShapeWrapper(
					oglplus::List("Position")("TexCoord").Get(),
					oglplus::shapes::Plane(
						oglplus::Vec3f(0.5f, 0.0f, 0.5f),
						oglplus::Vec3f(0.0f, 0.0f, 0.5f),
						oglplus::Vec3f(0.5f, 0.0f, 0.0f),
						spectrum_size-1,
						samples_per_sec-1
					)
				)
			)
		).first;
	}
	return pos->second;
}

