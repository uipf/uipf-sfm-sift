// http://stackoverflow.com/a/478960/1106908

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

namespace uipf {
	namespace sfm {

		std::string exec(const char* cmd) {
			std::array<char, 128> buffer;
			std::string result;
			std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
			if (!pipe) {
				throw std::runtime_error("popen() failed!");
			}
			while (!feof(pipe.get())) {
				if (fgets(buffer.data(), 128, pipe.get()) != NULL)
					result += buffer.data();
			}
			return result;
		}

	}
}
