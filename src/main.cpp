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

#if defined(WIN32)
    #include <windows.h>
#endif

#define CONSOLE

#include "game/Runner.hpp"

#ifdef CONSOLE
int main()
#elif WIN32
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow)
#endif
{
    Runner runner;
    try {
        runner.run();
    }
    catch (...) {

    }
    return 0;
}

//#include <iostream>
//
//const std::string resourceDir = "res/";
//const std::string files = "/*";
//
//#include "FileSystem.hpp"
//
//int main() {
//    robot2D::FileSystem manager;
//    //folder of .exe file
//    auto curdir = manager.getCurrentDir();
//    std::cout << curdir << std::endl;
//    auto fullpath = curdir + resourceDir + files;
//
//    auto fi = manager.listFiles(fullpath);
//    for(auto& f: fi) {
//        auto p = curdir + resourceDir + f;
//        if(manager.isDir(p))
//            std::cout << f <<std::endl;
//    }
//
//    //path = cd + resourseDir + folder + filename
//    // getCurrentDir() + path from configuration + filename
//
//
//    const std::string currentDir = manager.getCurrentDir();
//    const std::string textureDir = "res/textures";
//    const std::string filename = "icon.png";
//    const std::string slash = "/";
//
//    // five features
//    auto fullResPath = currentDir + textureDir + filename;
//
//
//    return 0;
//}