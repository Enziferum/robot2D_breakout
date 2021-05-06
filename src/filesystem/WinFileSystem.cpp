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

#include <windows.h>
#include "WinFileSystem.hpp"

namespace robot2D {
    namespace priv {
        std::string WinFileSystem::getCurrentDir() {
            char path[MAX_PATH];
            GetCurrentDirectoryA(MAX_PATH, path);
            return std::string(path).append(slash);
        }

        std::vector<std::string> WinFileSystem::listFiles(const std::string &path) {
            auto tmp = path + "*";
            std::vector<std::string> res;
            WIN32_FIND_DATA data;
            HANDLE hFind = FindFirstFile(tmp.c_str(), &data);      // DIRECTORY

            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    if (std::string(data.cFileName) == "." ||
                        std::string(data.cFileName) == "..")
                        continue;
                    res.emplace_back(data.cFileName);
                } while (FindNextFile(hFind, &data));
                FindClose(hFind);
            }
            return res;
        }

        bool WinFileSystem::isDir(const std::string &path) {
            DWORD ftyp = GetFileAttributesA(path.c_str());
            if (ftyp == INVALID_FILE_ATTRIBUTES)
                return false;  //something is wrong with your path!

            if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
                return true;

            return false;
        }

        bool WinFileSystem::isFile(const std::string &path) {
            DWORD ftyp = GetFileAttributesA(path.c_str());
            if (ftyp == INVALID_FILE_ATTRIBUTES)
                return false;

            if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
                return false;

            return true;
        }
    }
}

