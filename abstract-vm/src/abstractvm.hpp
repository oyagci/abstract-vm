#pragma once

#include <list>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace avm {

	template <
		class T,
		class Allocator = std::allocator<T>
	>
	using List = std::list<T, Allocator>;

	template <
		class T,
		std::size_t N
	>
	using Array = std::array<T, N>;

	template <
		class T,
		class Allocator = std::allocator<T>
	>
	using Vector = std::vector<T, Allocator>;

	using String = std::basic_string<char>;

	using StringView = std::string_view;

	template <typename T>
	using SharedPtr = std::shared_ptr<T>;

	template <typename T, class Deleter = std::default_delete<T>>
	using UniquePtr = std::unique_ptr<T, Deleter>;

	template <typename T, typename ... Args>
	constexpr SharedPtr<T> MakeShared(Args&&... args)
	{
			return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T, typename ... Args>
	constexpr UniquePtr<T> MakeUnique(Args&&... args)
	{
			return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Optional = std::optional<T>;

	inline constexpr std::nullopt_t NullOpt = std::nullopt;

	template<
		class Key,
		class T,
		class Hash = std::hash<Key>,
		class KeyEqual = std::equal_to<Key>,
		class Allocator = std::allocator<std::pair<const Key, T>>
	>
	using UnorderedMap = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

	template<
		class Key,
		class Hash = std::hash<Key>,
		class KeyEqual = std::equal_to<Key>,
		class Allocator = std::allocator<Key>
	>
	using UnorderedSet = std::unordered_set<Key, Hash, KeyEqual, Allocator>;

	template <
		class T1,
		class T2
	>
	using Pair = std::pair<T1, T2>;

	void Test();

}
