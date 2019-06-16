#pragma once

#include <vector>
#include <memory>

class EventListener;

class EventEmitter
{
public:
	virtual ~EventEmitter() = default;
	
    virtual bool process(const std::vector<std::weak_ptr<EventListener>> &inListeners) = 0;
private:
};
