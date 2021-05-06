/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
robot2D_game - Zlib license.
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

#include "robot2D/Graphics/Drawable.h"
#include "robot2D/Graphics/Shader.h"

class PostProcessing : public robot2D::Drawable {
public:
    PostProcessing();

    ~PostProcessing() override = default;

    void set_size(const robot2D::vec2u &size);

    void update(float dt);

    void preRender();

    void afterRender();

    void setValue(const std::string &id, const bool &value);

protected:
    virtual void draw(robot2D::RenderTarget &target, robot2D::RenderStates states) const override;

private:
    void setup_GL();

    void init_RenderData();

private:
    robot2D::ShaderHandler m_effectShader;
    robot2D::Texture m_texture;

    robot2D::vec2u m_size;

    //params
    float m_shake_time = 0.05f;
    bool m_shake;
    bool m_chaos;
    bool m_confuse;

    float m_dt = 0;

    //buffers//
    unsigned int MSFBO;
    unsigned int FBO;
    unsigned int RBO;
    unsigned int VAO;
};