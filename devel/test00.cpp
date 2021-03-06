/**
 *  .file devel/test00.cpp
 *  Development / testing file.
 *  NOTE. this file is here for feature development / testing purposes only
 *  and its source code, input, output can change witout prior notice.
 *
 *  Copyright 2008-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define OGLPLUS_CUSTOM_ERROR_HANDLING 1

#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/wicker_torus.hpp>
#include <oglplus/bound/buffer.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/images/brushed_metal.hpp>

#include <iostream>

#include <cmath>

#include "test.hpp"

namespace oglplus {
namespace images {

} // namespace images


class Test00 : public Test
{
private:
	typedef shapes::WickerTorus Shape;
	//
	Shape shape;
	shapes::DrawingInstructions shape_instr;
	Shape::IndexArray shape_indices;

	// wrapper around the current OpenGL context
	Context gl;

	VertexShader vs;

	FragmentShader fs;

	Program prog;

	// A vertex array object for the rendered shape
	VertexArray vao;

	Buffer verts;
	Buffer normals;
	Buffer tangents;
	Buffer texcoords;

	Texture texture;
public:
	Test00(void)
	 : shape(1.5, 1.0, 0.01, 48, 96)
	 , shape_instr(shape.Instructions())
	 , shape_indices(shape.Indices())
	{
		std::srand(time(0));
		vs.Source(
			"#version 330\n"
			"uniform mat4 projectionMatrix, cameraMatrix, modelMatrix;"
			"uniform vec3 CameraPosition, LightPos;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec3 Tangent;"
			"in vec2 TexCoord;"
			"out vec3 vertNormal;"
			"out vec3 vertTangent;"
			"out vec3 vertBinormal;"
			"out vec3 vertLightDir;"
			"out vec3 vertViewDir;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = "
			"		modelMatrix *"
			"		Position;"
			"	vertLightDir = LightPos - gl_Position.xyz;"
			"	vertNormal = ("
			"		modelMatrix *"
			"		vec4(Normal, 0.0)"
			"	).xyz;"
			"	vertTangent = ("
			"		modelMatrix *"
			"		vec4(Tangent, 0.0)"
			"	).xyz;"
			"	vertBinormal = cross(vertNormal, vertTangent);"
			"	vertViewDir = normalize("
			"		CameraPosition - gl_Position.xyz"
			"	);"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = "
			"		projectionMatrix *"
			"		cameraMatrix *"
			"		gl_Position;"
			"}"
		);
		vs.Compile();

		fs.Source(
			"#version 330\n"
			"uniform vec3 Color1, Color2;"
			"uniform sampler2D Texture;"
			"in vec3 vertNormal;"
			"in vec3 vertTangent;"
			"in vec3 vertBinormal;"
			"in vec3 vertLightDir;"
			"in vec3 vertViewDir;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	vec2 TexCoord = vec2(vertTexCoord.s*16, vertTexCoord.t*4);"
			"	vec3 t = texture(Texture, TexCoord).rgb;"

			"	vec3 Normal = normalize(2*vertNormal + (t.r - 0.5)*vertTangent + (t.g - 0.5)*vertBinormal);"

			"	vec3 LightRefl = reflect("
			"		-normalize(vertLightDir),"
			"		Normal"
			"	);"

			"	float Diffuse = max(dot("
			"		normalize(Normal), "
			"		normalize(vertLightDir)"
			"	), 0.0);"

			"	float Specular = pow(max(dot("
			"		normalize(LightRefl),"
			"		normalize(vertViewDir)"
			"	)+0.1, 0.0), 16+t.b*48);"

			"	float c = ("
			"		int(vertTexCoord.x*24) % 2+"
			"		int(vertTexCoord.y*24) % 2"
			"	) % 2;"
			"	float v = (1.0-c/2.0);"
			"	vec3 LtColor = vec3(1, 1, 1);"
			"	vec3 Color = mix(mix(Color1, Color2, v), LtColor, t.b);"
			"	fragColor = vec4(Color*(Diffuse+0.4) + LtColor*Specular*pow(0.6+t.b*0.8, 4.0), 1.0);"
			"}"
		);
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the shape
		vao.Bind();

		auto error_handler_func = [](const ErrorData& ed) -> bool
		{
			std::cerr << ed.Message() << std::endl;
			return true;
		};

		try
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = shape.Positions(data);
			Bind(verts, Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the vertices
			VertexAttribArray attr(prog, "Position");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}
		catch(...){ }

		try
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = shape.Normals(data);
			Bind(normals, Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the normals
			VertexAttribArray attr(prog, "Normal");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}
		catch(...){ }

		try
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = shape.Tangents(data);
			Bind(tangents, Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the normals
			VertexAttribArray attr(prog, "Tangent");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}
		catch(...){ }

		try
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = shape.TexCoordinates(data);
			Bind(texcoords, Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the normals
			VertexAttribArray attr(prog, "TexCoord");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}
		catch(...){ }

		try
		{
			Texture::Active(0);
			UniformSampler(prog, "Texture").Set(0);
			auto bound_tex = Bind(texture, Texture::Target::_2D);
			bound_tex.Image2D(images::BrushedMetalUByte(512, 512, 5120, -32, 32, 16, 32));
			bound_tex.GenerateMipmap();
			bound_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
			bound_tex.MagFilter(TextureMagFilter::Linear);
			bound_tex.WrapS(TextureWrap::Repeat);
			bound_tex.WrapT(TextureWrap::Repeat);
		}
		catch(...){ }

		Uniform<Mat4f>(prog, "projectionMatrix").Set(
			CamMatrixf::Perspective(Degrees(48), 1.25, 1, 100)
		);
		Uniform<Vec3f>(prog, "LightPos").Set(Vec3f(5.0f, 9.0f, 4.0f));

		//
		gl.ClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		//
		gl.FrontFace(shape.FaceWinding());
		gl.CullFace(Face::Back);
		gl.Enable(Capability::CullFace);
		gl.PolygonMode(Face::Back, PolygonMode::Line);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			4.5 - SineWave(time / 20.0)*1.5,
			FullCircles(time / 25.0),
			Degrees(SineWave(time / 30.0) * 80)
		);
		Uniform<Mat4f>(prog, "cameraMatrix").Set(camera);
		Uniform<Vec3f>(prog, "CameraPosition").Set(camera.Position());

		Uniform<Mat4f>(prog, "modelMatrix").Set(
//			ModelMatrixf()
/**/
			ModelMatrixf::RotationA(
				Vec3f(1.0f, 1.0f, 1.0f),
				FullCircles(time * 0.1)
			)
/**/
		);
		Uniform<Vec3f> color1(prog, "Color1");
		Uniform<Vec3f> color2(prog, "Color2");

		shape_instr.Draw(
			shape_indices,
			1,
			[&color1, &color2](GLuint phase) -> bool
			{
				if(phase < 2)
				{
					color1.Set(Vec3f(0.8, 0.8, 0.8));
					color2.Set(Vec3f(0.4, 0.4, 0.4));
				}
				else if(phase < 4)
				{
					color1.Set(Vec3f(0.1, 0.1, 0.1));
					color2.Set(Vec3f(0.1, 0.1, 0.1));
				}
				else
				{
					color1.Set(Vec3f(1.0, 0.8, 0.4));
					color2.Set(Vec3f(0.6, 0.4, 0.1));
				}
				return true;
			}
		);
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}
};

std::unique_ptr<Test> makeTest(void)
{
	return std::unique_ptr<Test>(new Test00);
}

} // namespace oglplus
