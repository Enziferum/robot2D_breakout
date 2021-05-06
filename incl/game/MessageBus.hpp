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

#include <vector>
#include "Message.hpp"

static constexpr std::size_t MessageSize = sizeof(Message);


class MessageBus final {
public:
    MessageBus();

    ~MessageBus() = default;

    const Message &poll();

    template<typename T>
    T *post(Message::ID id);

    std::size_t bus_size() const;

    bool empty();

private:
    //out buffers
    std::vector<char> in_buffer;
    std::vector<char> out_buffer;

    char *in_ptr;
    char *out_ptr;

    unsigned int wait_count;
    unsigned int current_count;
};

template<typename T>
T *MessageBus::post(Message::ID id) {
    //alloc buffer
    auto dataSize = sizeof(T);

    assert(dataSize < 128);
    //alloc not from buffer
    auto msg = new(in_ptr)Message();
    in_ptr += MessageSize;
    //need to store in memory block
    msg->id = id;
    msg->m_buffer_sz = dataSize;
    msg->m_buffer = new(in_ptr)T();
    in_ptr += dataSize;
    wait_count++;
    return static_cast<T *>(msg->m_buffer);
}
