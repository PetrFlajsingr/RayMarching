//
// Created by petr on 10/30/19.
//

#include "gl_utils.h"
bool checkProgramLinkStatus(GLuint program) {
  GLint isLinked;
  ge::gl::glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    int length;
    std::string log;
    ge::gl::glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    log.resize(static_cast<unsigned long>(length));
    ge::gl::glGetProgramInfoLog(program, length, &length, &log[0]);

    ge::gl::glDeleteProgram(program);
    std::cerr << "cannot Link: " << log << std::endl;
    return false;
  }
  return true;
}
std::pair<uint, uint> getGPUMemoryUsage() {
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
  GLint total_mem_kb = 0;
  ge::gl::glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb);

  GLint cur_avail_mem_kb = 0;
  ge::gl::glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);
  return {cur_avail_mem_kb, total_mem_kb};
}

ScopedShaderProgramUsage::ScopedShaderProgramUsage(ge::gl::Program &program) : program(program) {
  program.use();
}

ScopedShaderProgramUsage::~ScopedShaderProgramUsage() {
  GLint id;
  ge::gl::glGetIntegerv(GL_CURRENT_PROGRAM, &id);
  if (id == program.getId()) {
    ge::gl::glUseProgram(0);
  }
}
