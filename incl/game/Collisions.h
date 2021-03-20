/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
robot2D - Zlib license.
This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.
2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#pragma once
#include <tuple>
#include "GameObject.h"

enum class Direction{
    up,
    down,
    left,
    right
};

using Collision = std::tuple<bool, Direction, robot2D::vec2f>;

float length(const robot2D::vec2f& vec);
robot2D::vec2f normalize(const robot2D::vec2f& vec);

bool checkCollision(const GameObject& lob, const GameObject& rob);
Collision checkCollision(const BallObject& lob, const GameObject& rob);