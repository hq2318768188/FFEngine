#include "source.h"

namespace ff {
	Source::Ptr Source::create()
	{
		return std::make_shared <Source>();
	}

	Source::Source() noexcept = default;

	Source::~Source() noexcept = default;
}
