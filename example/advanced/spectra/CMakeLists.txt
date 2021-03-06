#  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_custom_target(oglplus-advanced-examples ALL)

if(wxWidgets_FOUND)
	add_definitions(-DOGLPLUS_LINK_LIBRARY=1)

	include_directories(${wxWidgets_INCLUDE_DIRS})
	include(${wxWidgets_USE_FILE})

	add_custom_target(oglplus-advanced-example-spectra)

	add_resource_directory(glsl oglplus-advanced-example-spectra)

	add_executable(
		spectra
		EXCLUDE_FROM_ALL
			spectra_app.cpp
			shared_objects.cpp
			main_frame.cpp
			coroutine.cpp
			document_frame.cpp
			document_canvas.cpp
			document_view.cpp
			document.cpp
			visualisation.cpp
			renderer.cpp
			default_renderer.cpp
			calculator.cpp
	)

	target_link_libraries(spectra ${wxWidgets_LIBRARIES})
	add_dependencies(oglplus-advanced-example-spectra spectra)
	add_dependencies(oglplus-advanced-examples oglplus-advanced-example-spectra)
endif()
