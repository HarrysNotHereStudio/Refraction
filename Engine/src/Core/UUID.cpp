#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

#include <Core/Time.h>
#include <Core/Utilities.h>

#include "UUID.h"

static std::string IntToHex(int i, uint16_t width) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(width) << std::hex << i;
	return stream.str();
}

namespace Refraction {
	std::unordered_set<uint64_t> UUID::UUIDHistory = { 0 };

	UUID UUID::FromExisting(uint64_t id) {
		UUID newID;
		newID.mElapsedSeconds = uint16_t(id >> 48);
		newID.mElapsedMilliseconds = uint16_t(id >> 32);
		newID.mRandomFirst = uint16_t(id >> 16);
		newID.mRandomSecond = uint16_t(id);

		if (UUIDHistory.count(newID.AsInt())) return UUID::Null();
		UUIDHistory.insert(newID.AsInt());
		return newID;
	}

	UUID UUID::Deserialise(std::string serialised) {
		return UUID::FromExisting((uint64_t)std::stoull(serialised));
	}

	UUID::UUID() {
		int repeatCounter = 0;

		do {
			mElapsedSeconds = (uint16_t)Time::GetSystemSec();
			mElapsedMilliseconds = (uint16_t)Time::GetSystemMSec() + repeatCounter;

			mRandomFirst = (uint16_t)Utilities::RandomI(0xFFFF);
			mRandomSecond = (uint16_t)Utilities::RandomI(0xFFFF);

			if (UUIDHistory.empty()) break;
			// Increment repeat counter
			repeatCounter++;
		} while (UUIDHistory.count(AsInt()));

		UUIDHistory.insert(AsInt());
	}

	UUID::UUID(const UUID& other) {
		mElapsedSeconds = other.mElapsedSeconds;
		mElapsedMilliseconds = other.mElapsedMilliseconds;
		mRandomFirst = other.mRandomFirst;
		mRandomSecond = other.mRandomSecond;
	}

	std::string UUID::AsString() const {
		return IntToHex((int)mElapsedSeconds, 4) + "-" + IntToHex((int)mElapsedMilliseconds, 4) + "-" + IntToHex(mRandomFirst, 4) + "-" + IntToHex(mRandomSecond, 4);
	}

	uint64_t UUID::AsInt() const {
		uint64_t result = 0;
		result += ((uint64_t)mElapsedSeconds << 48);
		result += ((uint64_t)mElapsedMilliseconds << 32);
		result += ((uint64_t)mRandomFirst << 16);
		result += ((uint64_t)mRandomSecond);
		return result;
	}

	void UUID::Reset() {
		// Remove from history to let another UUID take this in the one-in-a-never chance it happens to regenerate
		if(AsInt() != 0) UUIDHistory.erase(AsInt());
		mElapsedSeconds = 0;
		mElapsedMilliseconds = 0;
		mRandomFirst = 0;
		mRandomSecond = 0;
	}

	std::string UUID::Serialise() const {
		return std::to_string(AsInt());
	}

	UUID::UUID(uint16_t initValue) {
		mElapsedSeconds = initValue;
		mElapsedMilliseconds = initValue;
		mRandomFirst = initValue;
		mRandomSecond = initValue;
	}

	namespace Utilities {
		std::vector<uint64_t> ToInts(std::vector<UUID> uuids) {
			std::vector<uint64_t> result;
			for (auto& uuid : uuids) {
				result.push_back(uuid.AsInt());
			}
			return result;
		}

		std::vector<UUID> FromInts(std::vector<uint64_t> uuids) {
			std::vector<UUID> result;
			for (auto& uuid : uuids) {
				result.push_back(UUID::FromExisting(uuid));
			}
			return result;
		}

		std::vector<std::string> ToStrings(std::vector<UUID> uuids) {
			std::vector<std::string> result;
			for (auto& uuid : uuids) {
				result.push_back(uuid.AsString());
			}
			return result;
		}
	}
}