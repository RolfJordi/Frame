#pragma once

#include <cinttypes>
#include <memory>
#include <utility>

#include "frame/device_interface.h"
#include "frame/level_interface.h"
#include "frame/logger.h"

namespace frame {

/**
 * @class Level
 * @brief This is the level where everything is gathered and organized to be rendered. It is
 * deriving from LevelInterface so you could eventually create a better structure (entity based
 * component).
 */
class Level : public LevelInterface {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~Level() override;
    /**
     * @brief Set default quad static mesh id.
     * @param id: Id of the default quad static mesh.
     */
    void SetDefaultStaticMeshQuadId(EntityId id) final { quad_id_ = id; }
    /**
     * @brief Set default cube static mesh id.
     * @param id: Id of the default cube static mesh.
     */
    void SetDefaultStaticMeshCubeId(EntityId id) final { cube_id_ = id; }
    /**
     * @brief Will get the scene node from an id.
     * @param id: The id to get the scene node from.
     * @return A pointer to the node or null.
     */
    NodeInterface* GetSceneNodeFromId(EntityId id) const override {
        return id_scene_node_map_.at(id).get();
    }
    /**
     * @brief Will get the texture from an id.
     * @param id: The id to get the texture from.
     * @return A pointer to the texture or null.
     */
    TextureInterface* GetTextureFromId(EntityId id) const override {
        return id_texture_map_.at(id).get();
    }
    /**
     * @brief Will get the program from an id.
     * @param id: The id to get the program from.
     * @return A pointer to the program or null.
     */
    ProgramInterface* GetProgramFromId(EntityId id) const override {
        return id_program_map_.at(id).get();
    }
    /**
     * @brief Will get a material from an id.
     * @param id: The id to get the material from.
     * @return A pointer to a material or null.
     */
    MaterialInterface* GetMaterialFromId(EntityId id) const override {
        return id_material_map_.at(id).get();
    }
    /**
     * @brief Will get a buffer from an id.
     * @param id: The id to get the buffer from.
     * @return A pointer to a buffer or null.
     */
    BufferInterface* GetBufferFromId(EntityId id) const override {
        return id_buffer_map_.at(id).get();
    }
    /**
     * @brief Will get a static mesh from an id.
     * @param id: The id to get the static mesh from.
     * @return A pointer to a static mesh or null.
     */
    StaticMeshInterface* GetStaticMeshFromId(EntityId id) const override {
        return id_static_mesh_map_.at(id).get();
    }
    /**
     * @brief Will get a stream from an id.
     * @param id: The id to get the stream from.
     * @return A pointer to a stream or null.
     */
    StreamInterface<float>* GetBufferStreamFromId(EntityId id) const override {
        return id_buffer_stream_map_.at(id);
    }
    /**
     * @brief Will get a uniform from an id.
     * @param id: The id to get the uniform from.
     * @return A pointer to a stream or null.
     */
    StreamInterface<float>* GetUniformFloatStreamFromId(EntityId id) const override {
        return id_uniform_float_stream_map_.at(id);
    }
    /**
     * @brief Will get a uniform from an id.
     * @param id: The id to get the uniform from.
     * @return A pointer to a stream or null.
     */
    StreamInterface<std::int32_t>* GetUniformIntStreamFromId(EntityId id) const override {
        return id_uniform_int_stream_map_.at(id);
    }
    /**
     * @brief Will get a stream from an id.
     * @param id: The id to get the stream from.
     * @return A pointer to a stream or null.
     */
    StreamInterface<std::uint8_t>* GetTextureStreamFromId(EntityId id) const override {
        return id_texture_stream_map_.at(id);
    }
    /**
     * @brief Get a vector of static mesh id and corresponding material id.
     * @return Vector of static mesh id and corresponding material id.
     */
    std::vector<std::pair<EntityId, EntityId>> GetStaticMeshMaterialIds() const override {
        return mesh_material_ids_;
    }
    /**
     * @brief Get the default output texture id.
     * @return Id of the default output texture.
     */
    EntityId GetDefaultOutputTextureId() const override {
        return GetIdFromName(default_texture_name_);
    }
    /**
     * @brief Set the default camera name (used during loading as the camera is loaded after).
     * @param name: Name of the camera to be loaded.
     */
    void SetDefaultCameraName(const std::string& name) override { default_camera_name_ = name; }
    /**
     * @brief Get default root scene node id (this is the root of the scene tree).
     * @return An id or an error.
     */
    EntityId GetDefaultRootSceneNodeId() const override {
        return GetIdFromName(default_root_scene_node_name_);
    }
    /**
     * @brief Set the default scene root name (used during loading as the root node is not loaded in
     * a deterministic order).
     * @param name: Name of the scene root.
     */
    void SetDefaultRootSceneNodeName(const std::string& name) override {
        default_root_scene_node_name_ = name;
    }
    /**
     * @brief Get the default camera id, using the name that was stored during loading.
     * @return An id or an error.
     */
    EntityId GetDefaultCameraId() const override { return GetIdFromName(default_camera_name_); }
    /**
     * @brief Add a mesh and a material id (used for rendering by mesh later on).
     * @param node_id: Mesh node id.
     * @param material_id: Material id.
     */
    void AddMeshMaterialId(EntityId node_id, EntityId material_id) override {
        mesh_material_ids_.emplace_back(node_id, material_id);
    }

   public:
    /**
     * @brief Get the default quad static mesh id.
     * @return The id of the quad static mesh id or error.
     */
    EntityId GetDefaultStaticMeshQuadId() const final;
    /**
     * @brief Get the default cube static mesh id.
     * @return The id of the cube static mesh id or error.
     */
    EntityId GetDefaultStaticMeshCubeId() const final;
    /**
     * @brief Get the id of an element from a name string.
     * @param name: The name string of the element.
     * @return Id of the element or error.
     */
    EntityId GetIdFromName(const std::string& name) const override;
    /**
     * @brief Get the name of an element given an id.
     * @param id: Id of the element to get the name.
     * @return Name of the element.
     */
    std::optional<std::string> GetNameFromId(EntityId id) const override;
    /**
     * @brief Add scene node to the scene tree.
     * @param scene_node: Move a scene node to the scene tree.
     * @return Assigned entity id or error.
     */
    EntityId AddSceneNode(std::unique_ptr<NodeInterface>&& scene_node) override;
    /**
     * @brief Add a texture to the level.
     * @param texture: Move a texture in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddTexture(std::unique_ptr<TextureInterface>&& texture) override;
    /**
     * @brief Add a program to the level.
     * @param program: Move a program in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddProgram(std::unique_ptr<ProgramInterface>&& program) override;
    /**
     * @brief Add a material to the level.
     * @param material: Move a material in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddMaterial(std::unique_ptr<MaterialInterface>&& material) override;
    /**
     * @brief Add a buffer to the level.
     * @param buffer: Move a buffer in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddBuffer(std::unique_ptr<BufferInterface>&& buffer) override;
    /**
     * @brief Remove a buffer from the level.
     * @param buffer: The buffer id to be removed.
     */
    void RemoveBuffer(EntityId buffer) override;
    /**
     * @brief Add a static mesh to the level.
     * @param static_mesh: Move a buffer in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddStaticMesh(std::unique_ptr<StaticMeshInterface>&& static_mesh) override;
    /**
     * @brief Add a stream to the level.
     * @param stream: Move a stream in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddBufferStream(StreamInterface<float>* stream) override;
    /**
     * @brief Add a uniform to the level.
     * @param stream: Move a uniform in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddUniformFloatStream(StreamInterface<float>* stream) override;
    /**
     * @brief Add a uniform to the level.
     * @param stream: Move a uniform in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddUniformIntStream(StreamInterface<std::int32_t>* stream) override;
    /**
     * @brief Add a stream to the level.
     * @param stream: Move a stream in the level.
     * @return Assigned entity id or error.
     */
    EntityId AddTextureStream(StreamInterface<std::uint8_t>* stream) override;
    /**
     * @brief Get the list of children from an id in the node list.
     * @param id: The node id you want to get the children.
     * @return The node id children id(s).
     */
    std::optional<std::vector<EntityId>> GetChildList(EntityId id) const override;
    /**
     * @brief Get the parent of a given node id.
     * @param id: The current node we are searching for the parent.
     * @return Parent node id.
     */
    EntityId GetParentId(EntityId id) const override;
    /**
     * @brief Extract a texture (move it) from the level to outside (used in special cases).
     * @warning This will invalidate this entry!
     * @param id: The id of the texture to be extracted.
     * @return A unique pointer to the texture interface.
     */
    std::unique_ptr<TextureInterface> ExtractTexture(EntityId id) override;
    /**
     * @brief Get the default camera from the level.
     * @return A pointer to the default camera.
     */
    Camera* GetDefaultCamera() override;
    /**
     * @brief This add a correspondence between a stream and an texture id.
     * @param stream_id: Stream id.
     * @param texture_id: Texture id.
     */
    void AddStreamTextureCorrespondence(EntityId stream_id, EntityId texture_id) override;
    /**
     * @brief This add a correspondence between a stream and an buffer id.
     * @param stream_id: Stream id.
     * @param buffer_id: Buffer id.
     */
    void AddStreamBufferCorrespondence(EntityId stream_id, EntityId buffer_id) override;
    /**
     * @brief Return a vector of entity id that are part of the uniform stream.
     * @return The list of ids of uniform stream.
     */
    std::vector<EntityId> GetUniformFloatStreamIds() const override;
    /**
     * @brief Return a vector of entity id that are part of the uniform stream.
     * @return The list of ids of uniform stream.
     */
    std::vector<EntityId> GetUniformIntStreamIds() const override;
    /**
     * @brief Update all the structure that have to be updated by the system (mainly streams).
     */
    void Update(DeviceInterface* device, ProgramInterface* program, double dt = 0.0);

   protected:
    /**
     * @brief Increase the internal counter and return the value.
     * @return Current counter + 1.
     */
    EntityId GetTextureNewId() const { return ++next_id_maker_; }
    /**
     * @brief Increase the internal counter and return the value.
     * @return Current counter + 1.
     */
    EntityId GetProgramNewId() const { return ++next_id_maker_; }
    /**
     * @brief Increase the internal counter and return the value.
     * @return Current counter + 1.
     */
    EntityId GetMaterialNewId() const { return ++next_id_maker_; }
    /**
     * @brief Increase the internal counter and return the value.
     * @return Current counter + 1.
     */
    EntityId GetBufferNewId() const { return ++next_id_maker_; }
    /**
     * @brief Increase the internal counter and return the value.
     * @return Current counter + 1.
     */
    EntityId GetStaticMeshNewId() const { return ++next_id_maker_; }
    /**
     * @brief Increase the internal counter and return the value.
     * @return Current counter + 1.
     */
    EntityId GetSceneNodeNewId() const { return ++next_id_maker_; }
    /**
     * @brief Increase the internal counter and return the value.
     * @return Current counter + 1.
     */
    EntityId GetStreamNewId() const { return ++next_id_maker_; }

   protected:
    /**
     * @brief Replace a texture to a new texture given as a vector.
     * @param vector: The new vector containing the new texture.
     * @param size: Size of the new texture.
     * @param bytes_per_pixels: Byte per pixel in the new texture.
     * @param id: The id of the texture to be replaced.
     */
    void ReplaceTexture(std::vector<std::uint8_t>&& vector,
                        std::pair<std::uint32_t, std::uint32_t> size, std::uint8_t bytes_per_pixel,
                        EntityId id);
    /**
     * @brief Replace a mesh in the mesh vector in the entity management element.
     * @param mesh: A unique pointer to the mesh.
     * @param id: The id to replace the mesh.
     */
    void ReplaceMesh(std::unique_ptr<StaticMeshInterface>&& mesh, EntityId id);

   protected:
    Logger& logger_                 = Logger::GetInstance();
    mutable EntityId next_id_maker_ = NullId;
    EntityId quad_id_               = 0;
    EntityId cube_id_               = 0;
    std::string name_;
    std::string default_texture_name_;
    std::string default_root_scene_node_name_;
    std::string default_camera_name_;
	// These are storage so unique ptr interface.
    std::map<EntityId, std::unique_ptr<NodeInterface>> id_scene_node_map_        = {};
    std::map<EntityId, std::unique_ptr<TextureInterface>> id_texture_map_        = {};
    std::map<EntityId, std::unique_ptr<ProgramInterface>> id_program_map_        = {};
    std::map<EntityId, std::unique_ptr<MaterialInterface>> id_material_map_      = {};
    std::map<EntityId, std::unique_ptr<BufferInterface>> id_buffer_map_          = {};
    std::map<EntityId, std::unique_ptr<StaticMeshInterface>> id_static_mesh_map_ = {};
    // These come from external storage so this are pointers.
    std::map<EntityId, StreamInterface<float>*> id_buffer_stream_map_             = {};
    std::map<EntityId, StreamInterface<float>*> id_uniform_float_stream_map_      = {};
    std::map<EntityId, StreamInterface<std::int32_t>*> id_uniform_int_stream_map_ = {};
    std::map<EntityId, StreamInterface<std::uint8_t>*> id_texture_stream_map_     = {};
    // These are storage specifiers.
    std::set<std::string> string_set_                                  = {};
    std::map<std::string, EntityId> name_id_map_                       = {};
    std::map<EntityId, std::string> id_name_map_                       = {};
    std::map<EntityId, EntityTypeEnum> id_enum_map_                    = {};
    std::vector<std::pair<EntityId, EntityId>> mesh_material_ids_      = {};
    std::vector<std::pair<EntityId, EntityId>> stream_texture_ids_     = {};
    std::vector<std::pair<EntityId, EntityId>> stream_static_mesh_ids_ = {};
};

}  // End namespace frame.
