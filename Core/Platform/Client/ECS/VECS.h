#pragma once 
#include <unordered_map>
#include <variant>

export module VECS;

export namespace VECS
{
	using EntityId = std::uint64_t;

	template <typename...Cs>
	using Component = std::variant<C...>;

}