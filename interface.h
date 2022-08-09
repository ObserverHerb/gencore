#pragma once

namespace Interface
{
	class Drawable
	{
	public:
		virtual const int Rotation() const=0;
		virtual const SDL_Texture& Texture() const=0;
	};

	class Movable
	{
	public:
		virtual SDL_Point Position() const=0;
	};
}
