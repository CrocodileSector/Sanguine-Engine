#pragma once

#include "sgpch.h"

#ifdef SG_PLATFORM_WINDOWS
	#ifdef SG_BUILD_DLL
		#define SANGUINE_API __declspec(dllexport)
	#else
		#define SANGUINE_API __declspec(dllimport)
	#endif
#endif

#define BIT(x) (1 << x)

#define SG_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define ZERO_MEM(a)			memset(a, 0, sizeof(a))
#define SIZEOF_ARRAY(a)		(sizeof(a) / sizeof(a[0]))
#define SAFE_FREE(p)		if (p) {delete p; p = nullptr;}
#define SAFE_FREE_ARRAY(p)	if (p) {delete[] p; p = nullptr;}

#define SET_BIT(item, bit) (item) |= (1 << (bit))
#define CLEAR_BIT(item, bit) (item) &= ~(1 << (bit))
#define IS_BIT_SET(item, bit) (((item) & (1 << (bit))) != 0)

namespace RESOURCE_PATH
{
	const std::string ROOT = "assets/";
	const std::string MODELS = ROOT + "models/";
	const std::string TEXTURES = ROOT + "textures/";
	const std::string SHADERS = ROOT + "shaders/";
}

enum MeshType
{
	Cube = 0,
	Sphere,
	Plane,
	Teapot,
	Custom,
	Invalid 
};

namespace Sanguine
{
	static std::recursive_mutex kObjectMutex;

	template<typename T>
	using Scoped = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scoped<T> CreateScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Reference = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Reference<T> CreateReference(Args&& ... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
}