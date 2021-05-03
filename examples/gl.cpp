/**
  * This file is part of gltoolbox
  *
  * MIT License
  *
  * Copyright (c) 2021 Georges Nader
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  * 
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */

#include <iostream>

#include <GLFW/glfw3.h>
#include <gltoolbox/gltoolbox.h>

std::string vert = "#version 450 core \n in vec2 vtx_pos; \n void main(void) { gl_Position = vec4(vtx_pos.x, vtx_pos.y, 0., 1.0); }";
std::string frag = "#version 450 core \n uniform float grey; \n out vec4 colour; \n void main(void) { colour = vec4(grey, grey, grey, 1.0); }";

int main(int argc, char **argv)
{
  std::vector<float> pos = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
  std::vector<unsigned int> indices = {0, 3, 2, 0, 2, 1};
  float grey = 0.5;

  if (!glfwInit())
    return 1;

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  GLFWwindow *window = glfwCreateWindow(640, 480, "gltoolbox demo", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  // glfwHideWindow(window);
  glfwMakeContextCurrent(window);
  gltoolbox::GL::initilize(glfwGetProcAddress);
  glfwSwapInterval(0);

  std::cout << "OpenGL version: " << gltoolbox::GL::gl_version() << std::endl;
  std::cout << "GLSL version: " << gltoolbox::GL::glsl_version() << std::endl;

  gltoolbox::Program prg;
  prg.attach_shader(std::move(gltoolbox::Shader(vert, GL_VERTEX_SHADER)));
  prg.attach_shader(std::move(gltoolbox::Shader(frag, GL_FRAGMENT_SHADER)));
  prg.link();

  std::cout << prg.get_shader(GL_VERTEX_SHADER).compile_status() << std::endl;
  std::cout << prg.get_shader(GL_VERTEX_SHADER).is_valid() << std::endl;

  std::cout << prg.get_shader(GL_FRAGMENT_SHADER).compile_status() << std::endl;
  std::cout << prg.get_shader(GL_FRAGMENT_SHADER).is_valid() << std::endl;

  std::cout << prg.is_valid() << std::endl;
  std::cout << prg.link_status() << std::endl;

  prg.add_uniform<float>("grey", &grey);
  prg.add_attribute("vtx_pos");

  std::cout << prg.num_active_attributes() << std::endl;
  std::cout << prg.has_attribute("vtx_pos") << std::endl;

  // std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

  gltoolbox::VertexArray vao;
  vao.set_index_buffer<unsigned int>(GL_TRIANGLES, indices.data(), indices.size(), GL_STATIC_DRAW);
  vao.add_attribute<float>("vtx_pos", pos.data(), pos.size(), 2, GL_FLOAT, 0, 0, GL_STATIC_DRAW);

  std::cout << vao.has_index_buffer() << std::endl;
  std::cout << vao.has_attribute("vtx_pos") << std::endl;

  auto wp = vao.attribute_buffer("vtx_pos");
  std::cout << wp.lock()->memory_size() << std::endl;

  int width, height;

  while (!glfwWindowShouldClose(window))
  {
    glfwMakeContextCurrent(window);
    glbinding::Binding::useCurrentContext();

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.f, 1.f, 1.f, 1.f);

    prg.use();

    vao.bind();
    vao.enable_attribute(prg.attributes());
    vao.drawElements();
    vao.disable_attribute(prg.attributes());
    vao.unbind();

    prg.unuse();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  std::cout << "done." << std::endl;

  return 0;
}