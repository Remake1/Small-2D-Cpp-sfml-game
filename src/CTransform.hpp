#ifndef CPPGAME_CTRANSFORM_HPP
#define CPPGAME_CTRANSFORM_HPP

#include "Vec2.hpp"

class CTransform {
public:
    Vec2 pos = {0.0, 0.0};
    Vec2 velocity = {0.0, 0.0};
    float angle = 0;

    CTransform(const Vec2 & p, const Vec2 & v, float a)
    : pos(p), velocity(v), angle(a) {}
};


#endif //CPPGAME_CTRANSFORM_HPP
