#include "fileio.h"

namespace ogl {

	std::optional<std::string> ReadFile(FILE* file) { 
		
		// Seek to end of file to get the size of file
		if(fseek(file, 0, SEEK_END) == -1) {
			log::Error("Failed to seek in anonymous file");
			return std::nullopt;
		}
		
		size_t fileSize = ftell(file);
		if(fseek(file, 0, SEEK_SET) == -1) { 
			log::Error("Failed to seek in anonymous file");
			return std::nullopt;
		}

		std::string data;
		data.resize(fileSize);

		size_t bytesRead = fread(&data[0], 1, fileSize, file); 
		if(bytesRead != fileSize) { 
			log::Error("Failed to fully read anonymous file");
			return std::nullopt;
		}

		return data;
	}

	std::optional<std::string> ReadFile(const char* filePath) {
		FILE* file = fopen(filePath, "r");
		if(!file) {
			log::Error("Failed to open file '%s' for reading", filePath);
			return std::nullopt;
		}
		
		// Close file before function returns
		SCOPE_DEFER([file] { fclose(file); });

		return ReadFile(file);	
	}

}
