#pragma once

#include "Application.h"

#ifdef SG_PLATFORM_WINDOWS

extern Sanguine::Application* Sanguine::CreateApplication();

int main(int argc, char** argv)
{
	Sanguine::Log::Init();

	SG_PROFILER_BEGIN_SESSION("Application Startup", "SanguineProfiler-Startup.json");
	auto app = Sanguine::CreateApplication();
	SG_PROFILER_END_SESSION();

	SG_PROFILER_BEGIN_SESSION("Application Runtime", "SanguineProfiler-Runtime.json");
	app->Run();
	SG_PROFILER_END_SESSION();

	SG_PROFILER_BEGIN_SESSION("Application Shutdown", "SanguineProfiler-Shutdown.json");
	delete app;
	SG_PROFILER_END_SESSION();
}

#endif