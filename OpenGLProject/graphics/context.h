#pragma once

#include <cstdint>

namespace ogl {

	namespace intern {
		struct GLContext {
			const int majorVersion, minorVersion;
		};
	}

	// GraphicContext contains all relavent information about the current graphics context
	// of the target window. Fields marked with 'const' do not change over the lifetime of
	// the program.
	struct GraphicsContext {
		const intern::GLContext GL;
		const int maxTotalTextureSlots;
		const int maxFragmentTextureSlots;
		int frameBufferWidth, frameBufferHeight;
	};
}
