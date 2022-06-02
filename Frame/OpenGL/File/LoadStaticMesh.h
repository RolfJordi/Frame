#pragma once

#include <memory>
#include <string>

#include "Frame/File/Obj.h"
#include "Frame/LevelInterface.h"
#include "Frame/NodeStaticMesh.h"
#include "Frame/StaticMeshInterface.h"

namespace frame::opengl::file {

/**
 * @brief Load static meshes from file.
 * @param level: The level in which you want to load the mesh.
 * @param file: The file name of the mesh.
 * @param name: The name of the mesh.
 * @param material_name: The material that is used.
 * @param skip_material_file: Should you skip the material that are in the file?
 * @return The entity id of the mesh in the level.
 */
std::optional<std::vector<EntityId>> LoadStaticMeshesFromFile(LevelInterface* level,
                                                              const std::string& file,
                                                              const std::string& name,
                                                              const std::string& material_name = "",
                                                              bool skip_material_file = false);

}  // namespace frame::opengl::file
