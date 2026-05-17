#include <random>
#include <unordered_set>

#include "EngineUtilities.h"


using std::string, std::vector, fs::directory_entry, fs::directory_iterator, fs::path;


// Needs a NULL terminator, otherwise operations like shader compilation will find junk characters at the end of the file
const char* fileTerminator = "\0";

std::random_device randDevice;
std::mt19937 randGen(randDevice());


int Utilities::RandomI(const int max, const int min) {
	std::uniform_int_distribution<> dist(min, max);
	return dist(randGen);
}

float Utilities::RandomF(const float max, const float min) {
	std::uniform_real_distribution<> dist(min, max);
	return (float)dist(randGen);
}



// UUID
static std::string IntToHex(int i, uint8_t width) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(width) << std::hex << i;
	return stream.str();
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

std::string Utilities::GenerateUUID() {
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

std::string Utilities::ReadFile(const string& filename) {
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

vector<directory_entry> Utilities::GetFilesInFolder(path folderPath) {
	vector<directory_entry> files;
	for (const auto& file : directory_iterator(folderPath)) {
		files.push_back(file);
	};
	return files;
};

vector<directory_entry> Utilities::GetFilesOfExtInFolder(path folderPath, string ext) {
	vector<directory_entry> files;
	for (const auto& file : directory_iterator(folderPath)) {
		if (file.path().extension() == ext) {
			files.push_back(file);
		};
	};
	return files;
}
directory_entry Utilities::GetFirstFileOfExtInFolder(path folderPath, string ext) {
	for (const auto& file : directory_iterator(folderPath)) {
		if (file.path().extension() == ext) {
			return file;
		};
	};
	return directory_entry();
};

vector<directory_entry> Utilities::GetFoldersInFolder(path folderPath) {
	vector<directory_entry> folders;
	for (const auto& folder : fs::recursive_directory_iterator(folderPath)) {
		if (!folder.is_directory()) continue;
		folders.push_back(folder);
	};
	return folders;
};


// Math utilities
///

string Utilities::DeltaToRate(double deltaTime, int precision) {
	double fps = 1.0 / deltaTime;
	if (precision < 0) return std::to_string(fps);
	if (precision == 0) return std::to_string(trunc(fps));

	string fpsString = std::to_string(fps);
	return fpsString.substr(0, fpsString.find(".") + precision + 1);
}