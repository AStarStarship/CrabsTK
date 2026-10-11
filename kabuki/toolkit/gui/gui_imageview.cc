/*
    /kabuki/toolkit/gui/imageview.cpp -- Widget used to display images.

    The image view widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "t_imageview.h"
#include <cmath>

namespace _ {

std::vector<std::string> tokenize(const std::string &string,
								  const std::string &delim = "\n",
								  bool includeEmpty = false) {
	std::string::size_type lastPos = 0, pos = string.find_first_of(delim, lastPos);
	std::vector<std::string> tokens;

	while (lastPos != std::string::npos) {
		std::string substr = string.substr(lastPos, pos - lastPos);
		if (!substr.empty() || includeEmpty)
			tokens.push_back(std::move(substr));
		lastPos = pos;
		if (lastPos != std::string::npos) {
			lastPos += 1;
			pos = string.find_first_of(delim, lastPos);
		}
	}

	return tokens;
}

constexpr char const *const defaultImageViewVertexShader =
	R"(#version 330
	uniform vec2 scaleFactor;
	uniform vec2 position;
	in vec2 vertex;
	out vec2 uv;
	void main() {
		uv = vertex;
		vec2 scaledVertex = (vertex * scaleFactor) + position;
		gl_Position  = vec4(2.0*scaledVertex.x - 1.0,
							1.0 - 2.0*scaledVertex.y,
							0.0, 1.0);

	})";

constexpr char const *const defaultImageViewFragmentShader =
	R"(#version 330
	uniform sampler2D image;
	out vec4 color;
	in vec2 uv;
	void main() {
		color = texture(image, uv);
	})";

}  // namespace _

} //< namespace _
