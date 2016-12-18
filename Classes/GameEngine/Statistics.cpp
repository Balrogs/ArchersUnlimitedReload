#include "Statistics.h"

Statistics *Statistics::create() {
    Statistics *ret = new(std::nothrow) Statistics();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}

bool Statistics::init() {
    return Node::init();
}
