#pragma once

namespace rat
{
	enum class VerticalAlignment
	{
		Center,
		Upper,
		Lower
	};

	enum class HorizontalAlignment
	{
		Center,
		Left,
		Right
	};

	struct TextAlignment
	{
		VerticalAlignment vertical;
		HorizontalAlignment horizontal;
	};
}
