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

#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <iostream>

#include "robot2D/Graphics/RenderTarget.h"
#include "game/Level.hpp"


std::map<int, robot2D::Color> tile_colors = {
        {1, robot2D::Color::from_gl(0.8f, 0.8f, 0.7f, 0.1f)},
        {2, robot2D::Color::from_gl(0.2f, 0.6f, 1.0f, 0.1f)},
        {3, robot2D::Color::from_gl(0.0f, 0.7f, 0.0f, 0.1f)},
        {4, robot2D::Color::from_gl(0.8f, 0.8f, 0.4f, 0.1f)},
        {5, robot2D::Color::from_gl(1.0f, 0.5f, 0.0, 0.1f)},
};


Level::Level() :
        m_tiles(),
        m_size(),
        rw(0),
        rh(0) {}

bool Level::loadLevel(const std::string &path, const robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &handler,
                      const robot2D::vec2f &size, const robot2D::vec2f &offset) {

    std::ifstream file(path.c_str());
    if (!file.is_open())
        return false;

    std::string line;
    std::vector<std::vector<unsigned int>> rows;
    while (std::getline(file, line)) {
        std::vector<unsigned int> row;
        line.erase(std::remove(line.begin(), line.end(), ' '),
                   line.end());
        for (auto &c: line)
            row.push_back((unsigned int) (c - '0'));
        rows.push_back(row);
    }
    file.close();

    if (rows.empty())
        return false;

    robot2D::vec2f tile_sz;
    rh = rows.size();
    rw = rows[0].size();

    tile_sz.x = static_cast<float>((size.x - offset.x) / rw);
    tile_sz.y = static_cast<float>((size.y - offset.y) / rh);

    m_size = size;

    for (int y = 0; y < rh; ++y) {
        for (int x = 0; x < rw; ++x) {
            auto &c = rows[y][x];
            if (c < 1 || c > 5)
                continue;

            LevelBlock object;
            object.block_id = rh - y;
            if (c == 1) {
                object.m_solid = true;
                object.m_sprite.setTexture(handler.get(ResourceIDs::Solid));
            } else {
                object.m_sprite.setTexture(handler.get(ResourceIDs::Block));
            }

            robot2D::vec2f pos(offset.x + tile_sz.x * x,
                               offset.y + tile_sz.y * y);

            object.m_pos = pos;
            object.m_size = tile_sz;
            // incapsulate //
            object.m_sprite.setPosition(pos);
            object.m_sprite.setScale(tile_sz);
            object.m_sprite.setColor(tile_colors[c]);

            m_tiles.push_back(object);
        }
    }

    return true;
}

void Level::update(float dt) {
    m_tiles.erase(std::remove_if(m_tiles.begin(), m_tiles.end(),
                                 [](const GameObject &object) {
                                     return object.m_destroyed;
                                 }), m_tiles.end());
}

void Level::draw(robot2D::RenderTarget &target, robot2D::RenderStates states) const {
    for (auto &it: m_tiles) {
        if (it.m_destroyed)
            continue;

        target.draw(it, states);
    }
}

bool Level::destroyed() const {
    bool res = true;

    for (auto &it: m_tiles)
        res *= it.m_destroyed;

    return res;
}

std::vector<LevelBlock> &Level::getTiles() {
    return m_tiles;
}

void Level::onResize(const robot2D::vec2f &size) {
    if (m_size == size)
        return;

    //new tile_sz, for example == 1000 / 15, 800 / 8
    robot2D::vec2f tile_sz(size.x / rw,
                           size.y / rh);

    for (auto &it: m_tiles) {
        auto old_pos = it.m_pos;
        std::cout << old_pos.x << ":" << old_pos.y << std::endl;
        it.setSize(tile_sz);

        if (old_pos == robot2D::vec2f())
            continue;

        int it_x = int(size.x / old_pos.x);
        int it_y = 0;
        if (old_pos.y != 0)
            it_y = int(size.y / old_pos.y);


        it.setPos(robot2D::vec2f(tile_sz.x * it_y,
                                 tile_sz.y * it_x));
    }

    m_size = size;
}


