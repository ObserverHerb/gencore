#pragma once

#include <SDL.h>
#include <json/json.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>

using Frames=std::vector<SDL_Texture*>;

class Texture
{
public:
	Texture(const std::string &name);
	~Texture();
	Texture(const Texture &other) = delete;
	const Frames& Fetch() const;
protected:
	static std::unordered_map<std::string,Frames> cache;
	static Json::Value filenames;
	std::string name;
};

namespace Animation
{
	using Interval=std::chrono::duration<long long,std::nano>;
	using Timestamp=std::chrono::time_point<std::chrono::steady_clock>;

	class Adapter
	{
	public:
		Adapter(const Frames &frames,const Interval &interval);
		virtual SDL_Texture* operator()()=0;
	protected:
		const Frames &frames;
		Frames::const_iterator current;
		Timestamp lastUpdate;
		Interval interval;
	};

	class LoopAdapter : public Adapter
	{
	public:
		LoopAdapter(const Frames &frames,const Interval &interval) : Adapter(frames,interval) { }
		SDL_Texture* operator()();
	};

	class PingPongAdapter : public Adapter
	{
	public:
		PingPongAdapter(const Frames &frames,const Interval &interval);
		SDL_Texture* operator()();
	protected:
		Frames::const_reverse_iterator currentReverse;
	};
}
