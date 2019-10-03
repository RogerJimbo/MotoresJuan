#ifndef __GUI_ELEMENT_H__
#define __GUI_ELEMENT_H__

#include <string>
#include "ImGui/imgui.h"
#include "Application.h"

class Application;

class GUI_Element
{
public:
	GUI_Element(Application* app, bool active = true):App(app), show(active) {}
	~GUI_Element(){}

	virtual void Init() {}
	virtual void Draw() {}

public:
	std::string name = "No name";
	Application* App = nullptr;
	bool show = true;
};
#endif
