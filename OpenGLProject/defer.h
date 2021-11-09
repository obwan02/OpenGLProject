#pragma once

#define SCOPE_DEFER_ANON_VAR(line_num) _scope_defer_anon_var_ ## line_num ## _
#define SCOPE_DEFER(x) auto DEFER_ANON_VAR = ::ogl::ScopeDeferObject(x)

namespace ogl {
	
	template<typename F>
	struct ScopeDeferObject {
		
		ScopeDeferObject(F&& func) : m_Func(func) {}
		~ScopeDeferObject() { m_Func(); }

	private:
		F m_Func;
	};
}
