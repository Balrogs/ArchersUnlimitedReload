LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                                 ../../Classes/GameEngine/Player.cpp\
                                 ../../Classes/GameEngine/Global/Producer.cpp\
                                 ../../Classes/Scenes/Layers/PauseScene.cpp\
                                 ../../Classes/Scenes/PlayLayers/GameOverScene.cpp\
                                 ../../Classes/Scenes/MenuLayers/EquipmentScene.cpp\
                                 ../../Classes/Scenes/MenuLayers/SplashScene.cpp\
                                 ../../Classes/Scenes/MenuLayers/Randomizer.cpp \
                                 ../../Classes/Scenes/Layers/UI.cpp\
                                 ../../Classes/Scenes/Layers/BackgroundLayer.cpp\
                                 ../../Classes/Scenes/MenuLayers/Settings.cpp\
                                 ../../Classes/GameEngine/Objects/Stickman.cpp\
                                 ../../Classes/GameEngine/Objects/Aim/AimPreview.cpp\
                                 ../../Classes/GameEngine/Objects/Environment/Gate.cpp\
                                 ../../Classes/GameEngine/Objects/Arrows/Arrow.cpp\
                                 ../../Classes/GameEngine/Objects/Hero.cpp\
                                 ../../Classes/GameEngine/Objects/Aim/Aim.cpp\
                                 ../../Classes/GameEngine/Objects/Brains/Brain.cpp\
                                 ../../Classes/GameEngine/Global/WeaponSelector.cpp\
                                 ../../Classes/GameEngine/Global/Variables.cpp\
                                 ../../Classes/GameEngine/Objects/DragonObject.cpp\
                                 ../../Classes/AppDelegate.cpp\
                                 ../../Classes/Scenes/PlayLayers/Battle.cpp\
                                 ../../Classes/dragonBones/animation/Animation.cpp\
                                 ../../Classes/dragonBones/animation/AnimationState.cpp\
                                 ../../Classes/dragonBones/animation/TimelineState.cpp\
                                 ../../Classes/dragonBones/animation/WorldClock.cpp\
                                 ../../Classes/dragonBones/armature/Armature.cpp\
                                 ../../Classes/dragonBones/armature/Bone.cpp\
                                 ../../Classes/dragonBones/armature/Slot.cpp\
                                 ../../Classes/dragonBones/cocos2dx/CCArmatureDisplay.cpp\
                                 ../../Classes/dragonBones/cocos2dx/CCFactory.cpp\
                                 ../../Classes/dragonBones/cocos2dx/CCSlot.cpp\
                                 ../../Classes/dragonBones/cocos2dx/CCTextureData.cpp\
                                 ../../Classes/dragonBones/core/BaseObject.cpp\
                                 ../../Classes/dragonBones/events/EventObject.cpp\
                                 ../../Classes/dragonBones/factories/BaseFactory.cpp\
                                 ../../Classes/dragonBones/model/AnimationData.cpp\
                                 ../../Classes/dragonBones/model/ArmatureData.cpp\
                                 ../../Classes/dragonBones/model/DragonBonesData.cpp\
                                 ../../Classes/dragonBones/model/FrameData.cpp\
                                 ../../Classes/dragonBones/model/TimelineData.cpp\
                                 ../../Classes/dragonBones/parsers/DataParser.cpp\
                                 ../../Classes/dragonBones/parsers/JSONDataParser.cpp\
                                 ../../Classes/dragonBones/textures/TextureData.cpp\
                                 ../../Classes/GameEngine/Objects/Environment/Apple.cpp\
                                 ../../Classes/Scenes/PlayLayers/Apple/AppleBattle.cpp\
                                 ../../Classes/Scenes/PlayLayers/Duel/DuelScene.cpp\
                                 ../../Classes/Scenes/MenuLayers/MainMenu.cpp \
                                 ../../Classes/GameEngine/Objects/Environment/Ground.cpp \
                                 ../../Classes/Scenes/PlayLayers/Duel/DuelSceneMultiplayer.cpp \
                                 ../../Classes/Scenes/PlayLayers/Duel/DuelScene2P.cpp \
                                 ../../Classes/GameEngine/Global/Misc/SocketClient.cpp \
                                 ../../Classes/GameEngine/Global/Misc/JSONParser.cpp \
                                 ../../Classes/GameEngine/Global/Misc/Views.cpp \
                                 ../../Classes/GameEngine/Global/Misc/PopUp.cpp \
                                 ../../Classes/Localization/LocalizedStrings.cpp \
                                 ../../Classes/GameEngine/Objects/Environment/Box.cpp \
                                 ../../Classes/GameEngine/Statistics.cpp \
                                 ../../Classes/Scenes/PlayLayers/Apple/AppleMultiplayer.cpp \
                                 ../../Classes/Scenes/PlayLayers/BattleParent.cpp \
                                 ../../Classes/Scenes/PlayLayers/MultiplayerBattle.cpp \
                                 ../../Classes/Scenes/PlayLayers/Apple/AppleParent.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
