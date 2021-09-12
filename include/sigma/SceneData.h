struct FieldData
{
    DataType type;
    uint64_t count;
    uint8_t data;
};

struct ComponentData
{
    uint64_t id;
    uint64_t count;
    FieldData fields[];
};

struct EntityData
{
    uint64_t id;
    uint64_t count;
    Component components[];
};

struct SceneData
{
    uint64_t count;
    EntityData entites[];
};

// struct MeshData
// {
    
// };