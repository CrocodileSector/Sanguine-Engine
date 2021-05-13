#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Sanguine
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define SG_CORE_TRACE(...)		::Sanguine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SG_CORE_INFO(...)		::Sanguine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SG_CORE_WARN(...)		::Sanguine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SG_CORE_ERROR(...)		::Sanguine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SG_CORE_CRITICAL(...)	::Sanguine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define SG_TRACE(...)			::Sanguine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SG_INFO(...)			::Sanguine::Log::GetClientLogger()->info(__VA_ARGS__)
#define SG_WARN(...)			::Sanguine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SG_ERROR(...)			::Sanguine::Log::GetClientLogger()->error(__VA_ARGS__)
#define SG_CRITICAL(...)		::Sanguine::Log::GetClientLogger()->critical(__VA_ARGS__)

#define SG_ASSERT(x, ...) { if(!x) {SG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define SG_CORE_ASSERT(x, ...) { if(!x) {SG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }