#ifndef ARCUNLIM_APPLE_H
#define ARCUNLIM_APPLE_H

#include <GameEngine/Objects/Target.h>

class Apple : public DragonObject {

public:
    Apple(float x_pos, float y_pos);

    ~Apple();

    void update() override;

protected:
    void _updateAnimation() override;
};

#endif //ARCUNLIM_APPLE_H
