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
#include <game/MessageBus.hpp>

namespace {
    //11 mb of memory
    //
    // const auto memory_sz = 1048576u;
    const auto memory_sz = 10485u;
}

MessageBus::MessageBus() :
        in_buffer(memory_sz),
        out_buffer(memory_sz),
        in_ptr(out_buffer.data()),
        out_ptr(in_buffer.data()),
        current_count(0),
        wait_count(0) {

}

const Message &MessageBus::poll() {
    const Message &msg = *reinterpret_cast<Message *>(out_ptr);
    //move pointer to next message
    out_ptr += (MessageSize + msg.m_buffer_sz);
    current_count--;
    return msg;
}

std::size_t MessageBus::bus_size() const {
    return wait_count;
}

bool MessageBus::empty() {
    if (current_count == 0) {
        in_buffer.swap(out_buffer);
        in_ptr = out_buffer.data();
        out_ptr = in_buffer.data();
        current_count = wait_count;
        wait_count = 0;
        return true;
    }
    return false;
}
