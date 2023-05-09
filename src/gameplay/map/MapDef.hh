#ifndef GAMEPLAY_MAP_MAPDEF
#define GAMEPLAY_MAP_MAPDEF

#include <string>
#include <list>
#include <vector>
#include <set>
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

class MapObject;

class MapObject
{
public:
    std::string id;
    // std::set<std::string> classNames; -- Currently unused
    MapObjectType type;
    int tickScript; // Compiled version
    MapObject *relTarget = nullptr;
    std::string relTargetName;
    int player;              // Belongs to player
    double genTime, hitTime; // In seconds, compilation should be done during creating
    void *nativeObj;         // Bound generated native object. Currently it's of type 'TickObject*'
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

class CameraObject : public MapObject
{
public:
    double fov;
};

struct GameMap
{
    bool verified = false;
    MapMeta meta;                     // Map length
    std::vector<MapObject *> objects; // Objects (non-unique ref)
    std::map<std::string, MapObject *> namedObjects;
};

#endif /* GAMEPLAY_MAP_MAPDEF */
