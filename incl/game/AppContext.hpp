//
// Created by Necromant on 29.04.2021.
//

#pragma once

template<typename T = std::string>
struct AppContext{
    AppContext();
    ~AppContext() = default;

    bool storeInBuffer(const T& key, void* buff);
    void* getBuffer(const T& key);
private:
    std::unordered_map<T, void*> m_buffer;
};

template<typename T>
AppContext<T>::AppContext(): m_buffer() {}

template<typename T>
bool AppContext<T>::storeInBuffer(const T& key, void* buff) {
    if(buff == nullptr)
        return false;
    auto found = m_buffer.find(key);
    if(found != m_buffer.end())
        return false;
    m_buffer.insert(std::pair<T, void*>(key, buff));
    return true;
}

template<typename T>
void* AppContext<T>::getBuffer(const T& key) {
    return m_buffer.at(key);
}
