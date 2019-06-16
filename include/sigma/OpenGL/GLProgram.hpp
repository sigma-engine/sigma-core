#pragma once

#include <sigma/Program.hpp>

#include <cstdint>
#include <memory>

class Shader;

class GLProgram : public Program
{
public:
	GLProgram();

	virtual ~GLProgram();

	void attach(std::shared_ptr<Shader> inShader);

    bool link();

    void bind() const;
private:
	uint32_t mHandle = 0;
};
