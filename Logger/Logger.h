#pragma once
#include <vector>
#include <string>
#include <map>

#ifdef LOGGER_EXPORTS
#define LOGGER_API __declspec(dllexport)
#else
#define LOGGER_API __declspec(dllimport)
#endif

class LOGGER_API Logger
{
public:
	Logger() = default;

	enum class ELogCategories : uint8_t
	{
		None = 0,
		Verbose		= 1 << 0,
		Success		= 1 << 1,
		Warning		= 1 << 2,
		Error		= 1 << 3,
		Critical	= 1 << 4,
		Custom1		= 1 << 5,
		Custom2		= 1 << 6,
		Custom3		= 1 << 7
	};

	static std::map<ELogCategories, std::string> ColorMap;

	inline static void Log(ELogCategories LogLevel, std::string Message, std::string SystemTag);
	inline static void Clear();

private:
	static void WriteToLog(std::string SystemTag, const std::string& Content);

	static std::string GetPreciseTimeStamp();
};