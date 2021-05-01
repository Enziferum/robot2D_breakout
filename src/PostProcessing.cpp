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

#include <robot2D/Graphics/GL.h>
#include <robot2D/Graphics/RenderTarget.h>
#include <robot2D/Util/Logger.h>

#include "game/PostProcessing.hpp"

PostProcessing::PostProcessing():
m_size(800, 600),
m_shake(false),
m_chaos(false),
m_confuse(false){
    setup_GL();
}


void PostProcessing::update(float dt) {
    m_dt = dt;
    if(m_shake && m_shake_time > 0.f) {
        m_shake_time -= dt;
        if(m_shake_time <= 0.0f)
            m_shake = false;
    }
}

void PostProcessing::draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const {
    m_effectShader.use();
    m_effectShader.set_parameter("shake", m_shake);
    m_effectShader.set_parameter("chaos", m_chaos);
    m_effectShader.set_parameter("confuse", m_confuse);
    m_effectShader.set_parameter("time", m_dt);

    states.texture = &m_texture;
    states.customVao = &VAO;
    states.shader = const_cast<robot2D::ShaderHandler*>(&m_effectShader);
    target.draw(states);
}

//rewrite this function
void PostProcessing::setup_GL() {
    if(!m_effectShader.createShader(robot2D::shaderType::vertex,"res/shaders/effects.vs")) {
        LOG_ERROR("Can't process % ", "res/shaders/effects.vs")
        return;
    }

    if(!m_effectShader.createShader(robot2D::shaderType::fragment,"res/shaders/effects.fs")) {
        LOG_ERROR("Can't process % ", "res/shaders/effects.fs")
        return;
    }

    glGenFramebuffers(1, &MSFBO);
    glGenFramebuffers(1, &FBO);
    glGenRenderbuffers(1, &RBO);
    // initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)

    glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    // allocate storage for render buffer object
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, m_size.x, m_size.y);
    // attach MS render buffer object to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR_E( "ERROR::POSTPROCESSOR: Failed to initialize MSFBO")

    // also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    m_texture.generate(m_size, nullptr);
    // attach texture to framebuffer as its color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.get_id(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR_E("ERROR::POSTPROCESSOR: Failed to initialize FBO")
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // initialize render data and uniforms
    m_effectShader.use();
    m_effectShader.set_parameter("scene", 0);

    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
            { -offset,  offset  },  // top-left
            {  0.0f,    offset  },  // top-center
            {  offset,  offset  },  // top-right
            { -offset,  0.0f    },  // center-left
            {  0.0f,    0.0f    },  // center-center
            {  offset,  0.0f    },  // center - right
            { -offset, -offset  },  // bottom-left
            {  0.0f,   -offset  },  // bottom-center
            {  offset, -offset  }   // bottom-right
    };

    //allow shaders to set directly

    glUniform2fv(glGetUniformLocation(m_effectShader.getProgram(), "offsets"), 9, (float*)offsets);
    int edge_kernel[9] = {
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(m_effectShader.getProgram(), "edge_kernel"), 9, edge_kernel);
    float blur_kernel[9] = {
            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
            2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(m_effectShader.getProgram(), "blur_kernel"), 9, blur_kernel);


    init_RenderData();
}

void PostProcessing::preRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
    glClearColor(0.0, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessing::afterRender() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
    glBlitFramebuffer(0, 0, m_size.x, m_size.y, 0, 0, m_size.x, m_size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer
}

void PostProcessing::set_size(const robot2D::vec2u& size) {
    m_size = size;
}

//why we need custom VBO ??
void PostProcessing::init_RenderData() {

    unsigned int VBO;
    float vertices[] = {
            // pos        // tex
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PostProcessing::setValue(const std::string& id, const bool& value) {
    if(id == "chaos")
        m_chaos =  value;
    if(id == "confuse")
        m_confuse =  value;
    if(id == "shake") {
        m_shake = value;
        m_shake_time = 0.05f;
    }
}
