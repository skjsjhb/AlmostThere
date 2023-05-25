#ifndef GAMEPLAY_MAP_MAPDEF
#define GAMEPLAY_MAP_MAPDEF

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "gameplay/objs/NoteDef.hh"

struct MapMeta
{
    std::string name, id, author, mapper, diff, song;
    double duration;
};

enum PositionMethod
{
    // RUN: Right, Up, Normal as XYZ
    ABSOLUTE,    // The value is set as-is, using the world-space XYZ.
    REPROJ,      // The value is set as-is, using RUN as XYZ.
    PERSPECTIVE, // The value is added to the bound object, using RUN as XYZ
    OFFSET,      // The value is added to the bound object, using the world-space XYZ.
};

enum MapObjectType
{
    NOTE,
    SLOT,
    //  VIRTUAL,
    CAMERA,
};

class MapObject
{
public:
    std::string id;
    MapObjectType type;

    /**
     * @brief The compilation id of the tick script.
     *
     * For effeiciency, scripts are loaded and compiled in advance, this id reference
     * to the compiled script.
     */
    int tickScript;
    std::weak_ptr<MapObject> relTarget;
    std::string relTargetName;
    int player;
    double genTime, endTime;
    virtual ~MapObject() = default;
};

class SlotObject : public MapObject
{
public:
    SlotVariant slotType;
};

class NoteObject : public MapObject
{
public:
    double length; // For puresu and hashi
    NoteType noteType;
    bool isFake;
};

// Dummy impl
class CameraObject : public MapObject
{
};

struct GameMap
{
    bool verified = false;
    MapMeta meta;

    /**
     * @brief Stores all map objects.
     *
     * This vector holds the ownership of these objects. Others can only access.
     */
    std::vector<std::shared_ptr<MapObject>> objects;

    /**
     * @brief Stores map of named objects.
     *
     * This map stores the correspondence of name and its object.
     */
    std::map<std::string, std::weak_ptr<MapObject>> namedObjects;
};

#endif /* GAMEPLAY_MAP_MAPDEF */
