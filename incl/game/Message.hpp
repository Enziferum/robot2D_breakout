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


#include <cassert>
#include <cstdint>

class Message final {
public:
    Message() :
            m_buffer(nullptr),
            m_buffer_sz(0) {}

    ~Message() = default;

    using ID = int32_t;

    enum Type {
        Count
    };

    ID id = -1;

    template<typename T>
    const T &unpack() const {
        assert(sizeof(T) == m_buffer_sz);
        return *static_cast<T *>(m_buffer);
    }

private:
    friend class MessageBus;

    void *m_buffer;
    unsigned int m_buffer_sz;
};
