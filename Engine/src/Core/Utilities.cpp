#include <random>
#include <unordered_set>

#include "Utilities.h"

namespace RMath = Refraction::Math;

using std::string, std::vector, fs::directory_iterator, fs::path;


// Needs a NULL terminator, otherwise operations like shader compilation will find junk characters at the end of the file
const char* fileTerminator = "\0";

std::random_device randDevice;
std::mt19937 randGen(randDevice());

// UUID
static std::string IntToHex(int i, uint8_t width) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(width) << std::hex << i;
	return stream.str();
}

namespace Refraction::Utilities {
	int RandomI(const int max, const int min) {
		std::uniform_int_distribution<> dist(min, max);
		return dist(randGen);
	}

	float RandomF(const float max, const float min) {
		std::uniform_real_distribution<> dist(min, max);
		return (float)dist(randGen);
	}

	// The UUID generator uses system clock and RNG along with a repetition counter to hopefully generate a random UUID
	// It is formatted as such:
	// Section 1: Seconds since epoch as 4 digit hex
	// Section 2: Milliseconds since epoch as 4 digit hex*
	// Section 3: Random number as 4 digit hex
	// Section 4: Random number as 4 digit hex
	// Section 2 is slightly different, as the repetition counter is added to enhance variation
	// The repetition counter ticks up every time the generated UUID already exists in the unordered_set, and resets every time a new UUID is requested
	///

	std::string GenerateUUID() {
		using namespace std::chrono;
		static std::unordered_set<std::string> GeneratedUUIDs = {};
		std::string result = "";
		int repeatCounter = 0;

		do {
			// Get system clock ticks since epoch
			auto duration = system_clock::now().time_since_epoch();
			auto sec = duration_cast<seconds>(duration).count();
			auto milli = duration_cast<milliseconds>(duration).count() + repeatCounter;

			// Generate UUID string
			result = IntToHex((int)sec, 4) + "-" + IntToHex((int)milli, 4) + "-" + IntToHex(Utilities::RandomI(0xFFFF), 4) + "-" + IntToHex(Utilities::RandomI(0xFFFF), 4);

			if (GeneratedUUIDs.empty()) break;
			// Increment repeat counter
			repeatCounter++;
		} while (GeneratedUUIDs.count(result));

		GeneratedUUIDs.insert(result);

		return result;
	}


	// Filesystem utilities
	///

	std::string ReadFile(const string& filename) {
		if (!fs::exists(filename)) throw std::runtime_error("File " + filename + " does not exist.");
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		};

		size_t fileSize = (size_t)file.tellg();
		vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		buffer.insert(buffer.end(), *fileTerminator);

		return std::string(buffer.begin(), buffer.end());
	};

	vector<fs::directory_entry> GetFilesInFolder(path folderPath) {
		if (!fs::exists(folderPath)) throw std::runtime_error("Path " + folderPath.string() + " does not exist.");
		vector<fs::directory_entry> files;
		for (const auto& file : directory_iterator(folderPath)) {
			files.push_back(file);
		};
		return files;
	};

	vector<fs::directory_entry> GetFilesOfExtInFolder(path folderPath, string ext) {
		if (!fs::exists(folderPath)) throw std::runtime_error("Path " + folderPath.string() + " does not exist.");
		vector<fs::directory_entry> files;
		for (const auto& file : directory_iterator(folderPath)) {
			if (file.path().extension() == ext) {
				files.push_back(file);
			};
		};
		return files;
	}
	fs::directory_entry GetFirstFileOfExtInFolder(path folderPath, string ext) {
		if (!fs::exists(folderPath)) throw std::runtime_error("Path " + folderPath.string() + " does not exist.");
		for (const auto& file : directory_iterator(folderPath)) {
			if (file.path().extension() == ext) {
				return file;
			};
		};
		return fs::directory_entry();
	};

	vector<fs::directory_entry> GetFoldersInFolder(path folderPath) {
		if (!fs::exists(folderPath)) throw std::runtime_error("Path " + folderPath.string() + " does not exist.");
		vector<fs::directory_entry> folders;
		for (const auto& folder : fs::recursive_directory_iterator(folderPath)) {
			if (!folder.is_directory()) continue;
			folders.push_back(folder);
		};
		return folders;
	};


	// Math utilities
	///

	string DeltaToRate(double deltaTime, int precision) {
		double fps = 1.0 / deltaTime;
		if (precision < 0) return std::to_string(fps);
		if (precision == 0) return std::to_string(trunc(fps));

		string fpsString = std::to_string(fps);
		return fpsString.substr(0, fpsString.find(".") + precision + 1);
	}

	RMath::Vector3 BoolToVector3(bool pX, bool nX, bool pY, bool nY, bool pZ, bool nZ) {
		return RMath::Vector3(pX - nX, pY - nY, pZ - nZ);
	}

	glm::mat3 NativeToGLMMat3(RMath::Matrix3 m) {
		return glm::mat3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);
	}

	Refraction::Math::Matrix3 GLMToNativeMat3(glm::mat3 m) {
		Refraction::Math::Matrix3 newMat;
		for (size_t row = 0; row < 3; row++) {
			for (size_t col = 0; col < 3; col++) {
				newMat[row][col] = m[(int)row][(int)col];
			}
		}
		return newMat;
	}

	glm::mat4 NativeToGLMMat4(RMath::Matrix4 m) {
		return glm::mat4(m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2], m[1][3], m[2][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1], m[3][2], m[3][3]);
	}

	Refraction::Math::Matrix4 GLMToNativeMat4(glm::mat4 m) {
		Refraction::Math::Matrix4 newMat;
		for (size_t row = 0; row < 4; row++) {
			for (size_t col = 0; col < 4; col++) {
				newMat[row][col] = m[(int)row][(int)col];
			}
		}
		return newMat;
	}
}
