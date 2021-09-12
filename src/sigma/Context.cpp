#include <sigma/Context.hpp>


bool Context::initialize(std::shared_ptr<Engine> inEngine, std::shared_ptr<Device> inDevice)
{
	mEngine = inEngine;
	mDevice = inDevice;

	return true;
}