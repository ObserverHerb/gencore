#pragma once

namespace Concept
{
	template <typename T> concept Drawable=requires(T m)
	{
		{ m->Rotation() }->std::same_as<int>;
		{ m->Texture() }->std::same_as<const SDL_Texture&>;
	};

	template <typename T> concept Movable=requires(T m)
	{
		{ m->Position() }->std::same_as<const SDL_Point&>;
	};
}
