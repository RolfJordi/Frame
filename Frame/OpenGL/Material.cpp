#include "Material.h"
#include <sstream>
#include <cassert>

namespace frame::opengl {

	Material::~Material()
	{
		DisableAll();
	}

	void Material::SetLevel(std::shared_ptr<LevelInterface> level)
	{
		level_ = level;
	}

	bool Material::AddTextureId(std::uint64_t id, const std::string& name)
	{
		RemoveTextureId(id);
		return id_name_map_.insert({ id, name }).second;
	}

	bool Material::HasTextureId(std::uint64_t id) const
	{
		auto it = id_name_map_.find(id);
		return it != id_name_map_.end();
	}

	bool Material::RemoveTextureId(std::uint64_t id)
	{
		if (!HasTextureId(id)) return false;
		auto it = id_name_map_.find(id);
		id_name_map_.erase(it);
		return true;
	}

	const std::pair<std::string, int> Material::EnableTextureId(
		std::uint64_t id) const
	{
		// Check Level exist.
		if (!level_)
			throw std::runtime_error("There is no level.");
		// Check it exist.
		if (!HasTextureId(id))
			throw std::runtime_error("No texture id: " + std::to_string(id));
		// Check it is not already enabled.
		for (const auto& i : id_array_)
		{
			if (i == id)
			{
				throw std::runtime_error("Already in?");
			}
		}
		// Assign it.
		for (int i = 0; i < id_array_.size(); ++i)
		{
			if (id_array_[i] == 0)
			{
				id_array_[i] = id;
				auto texture = level_->GetTextureMap().at(id);
				texture->Bind(i);
				return { id_name_map_.at(id), i };
			}
		}
		// No free slots.
		throw std::runtime_error("No free slots!");
	}

	void Material::DisableTextureId(std::uint64_t id) const
	{
		// Check Level exist.
		if (!level_)
			throw std::runtime_error("There is no level.");
		// Check if exist.
		if (!HasTextureId(id))
			throw std::runtime_error("No texture id: " + std::to_string(id));
		// Disable it.
		for (auto& i : id_array_)
		{
			if (i == id)
			{
				i = 0;
				auto texture = level_->GetTextureMap().at(id);
				texture->UnBind();
				return;
			}
		}
		// Error not found in the enable array.
		throw std::runtime_error(
			"Texture id: " + std::to_string(id) + 
			" was not bind to any slots.");
	}

	void Material::DisableAll() const
	{
		// Check Level exist.
		if (!level_)
			throw std::runtime_error("There is no level.");
		for (int i = 0; i < 32; ++i)
		{
			if (id_array_[i])
			{
				DisableTextureId(id_array_[i]);
			}
		}
	}

	const std::vector<std::uint64_t> Material::GetIds() const
	{
		std::vector<std::uint64_t> vec;
		for (const auto& p : id_name_map_)
		{
			vec.push_back(p.first);
		}
		return vec;
	}

} // End namespace frame::opengl.
