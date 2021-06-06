#include "uuid.h"
#include "uuid.h"

#ifdef GUID_LIBUUID
#include <uuid/uuid.h>
namespace ogl {
	void uuid::Generate(uuid u) {
		uuid_generate((uuid_t)u);
	}
}
#endif

#ifdef _WIN32
#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000
#include <windows.h>
#include <rpc.h>

namespace ogl {
	void uuid::Generate(UUID_t u) {
		UuidCreate((UUID*)u);
	}
}
#endif


