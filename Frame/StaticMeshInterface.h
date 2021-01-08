#pragma once

#include <memory>
#include "../Frame/BindInterface.h"

namespace frame {

	// Static mesh interface (proposal).
	// Id are in std::uint64_t to allow for more see the level interface.
	struct StaticMeshInterface : public BindInterface
	{
		virtual void SetMaterialId(std::uint64_t id) = 0;
		virtual std::uint64_t GetMaterialId() const = 0;
		virtual std::uint64_t GetPointBufferId() const = 0;
		virtual std::uint64_t GetNormalBufferId() const = 0;
		virtual std::uint64_t GetTextureBufferId() const = 0;
		virtual std::uint64_t GetIndexBufferId() const = 0;
		virtual std::size_t GetIndexSize() const = 0;
	};

} // End namespace frame.