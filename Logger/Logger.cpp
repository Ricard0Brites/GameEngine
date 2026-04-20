#include "Logger.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::map<Logger::ELogCategories, std::string> Logger::ColorMap =
{
	{ELogCategories::Verbose, "\x1b[97m"},	// White Text
	{ELogCategories::Success, "\x1b[92m"},	// Green Text
	{ELogCategories::Warning, "\x1b[93m"},	// Yellow Text
	{ELogCategories::Error, "\x1b[91m"},	// Red Text
	{ELogCategories::Critical, "\x1b[41m\x1b[97m"}, // Red BG && White Text
	{ELogCategories::Custom1, "\x1b[96m"},	// Cyan Text
	{ELogCategories::Custom2, "\x1b[94m"},	// Blue Text
	{ELogCategories::Custom3, "\x1b[95m"}	// Magenta Text
};

void Logger::Log(ELogCategories LogLevel, std::string Message, std::string SystemTag)
{
	std::cout << ColorMap[LogLevel]  << GetPreciseTimeStamp().c_str() << Message << "\x1b[0m" << std::endl;
	WriteToLog(SystemTag, Message);
}

void Logger::Clear()
{
	std::cout << "\033[2J";
}

void Logger::WriteToLog(std::string SystemTag, const std::string& Content)
{

	#ifdef NDEBUG | WRITETOLOG
	
	using namespace std;
	filesystem::path p = filesystem::current_path();
	p += "\\Logs\\";

	// Verify/Create Directory
	if (!filesystem::exists(p))
		if (!filesystem::create_directory(p))
			return;

	p += SystemTag;
	p += ".log";

	// Create File and write to it
	fstream f(p, iostream::app);

	if (!f)
		cout << "\x1b[91m" << "Log Failed to open: " << SystemTag << "\x1b[0m" << endl;

	f << GetPreciseTimeStamp() << Content << endl;
	f.close();

	#endif // NDEBUG
}

std::string Logger::GetPreciseTimeStamp()
{
	auto now = std::chrono::system_clock::now();
	auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds).count();

	std::time_t tt = std::chrono::system_clock::to_time_t(seconds);

	std::tm bt{};
	localtime_s(&bt, &tt);

	std::ostringstream oss;
	oss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << ms << " -> ";

	return oss.str();
}
