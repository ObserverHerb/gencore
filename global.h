#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <chrono>

inline SDL_Window *window=nullptr;
inline SDL_Renderer *renderer=nullptr;

inline bool InitializeImageSubsystem()
{
	int flags=IMG_INIT_PNG;
	if ((IMG_Init(flags)&flags) != flags) return false;
	return true;
}

namespace Measurements
{
	using Speed=double;
	using Radians=double;
	using Degrees=double;
}

namespace Time
{
	using Interval=std::chrono::duration<long long,std::nano>;
	using Timestamp=std::chrono::time_point<std::chrono::steady_clock>;

	inline Interval Capture(Timestamp &timestamp)
	{
		Timestamp now=std::chrono::steady_clock::now();
		Interval difference=now-timestamp;
		timestamp=now;
		return difference;
	}
}

namespace Convert
{
	template <typename T> inline Measurements::Speed Speed(Measurements::Speed distance,T time) requires std::convertible_to<T,Time::Interval>
	{
		return distance/std::chrono::duration_cast<Time::Interval>(time).count();
	}

	inline Measurements::Speed Distance(Measurements::Speed speed,Time::Interval time)
	{
		return speed*time.count();
	}

	inline Measurements::Radians Radians(Measurements::Degrees degrees)
	{
		return (degrees*std::numbers::pi)/180;
	}
}
