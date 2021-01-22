#pragma once

#include <list>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <optional>

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

	using StringView = std::basic_string_view<char>;

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

	void Test();

}
