#ifndef DRAGONBONES_HEADERS_H
#define DRAGONBONES_HEADERS_H

// core
#include "dragonBones/core/DragonBones.h"
#include "dragonBones/core/BaseObject.h"

// geom
#include "dragonBones/geom/ColorTransform.h"
#include "dragonBones/geom/Matrix.h"
#include "dragonBones/geom/Point.h"
#include "dragonBones/geom/Rectangle.h"
#include "dragonBones/geom/Transform.h"

// textures
#include "dragonBones/textures/TextureData.h"

// model
#include "dragonBones/model/FrameData.h"
#include "dragonBones/model/DragonBonesData.h"
#include "dragonBones/model/ArmatureData.h"
#include "dragonBones/model/AnimationData.h"
#include "dragonBones/model/TimelineData.h"

// parsers
#include "dragonBones/parsers/DataParser.h"
#include "dragonBones/parsers/JSONDataParser.h"

// factories
#include "dragonBones/factories/BaseFactory.h"

// armature
#include "dragonBones/armature/IArmatureDisplay.h"
#include "dragonBones/armature/Armature.h"
#include "dragonBones/armature/TransformObject.h"
#include "dragonBones/armature/Bone.h"
#include "dragonBones/armature/Slot.h"

// animation
#include "dragonBones/animation/IAnimateble.h"
#include "dragonBones/animation/WorldClock.h"
#include "dragonBones/animation/Animation.h"
#include "dragonBones/animation/AnimationState.h"
#include "dragonBones/animation/BaseTimelineState.h"
#include "dragonBones/animation/TimelineState.h"

// events
#include "dragonBones/events/EventObject.h"

#endif // DRAGONBONES_HEADERS_H