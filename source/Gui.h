#pragma once

#include "Common.h"

class Gui;
typedef std::unique_ptr<Gui> upGui;

class Gui : private Uncopyable
{
public:
	// Initialization
	Gui();
	virtual ~Gui();

public:
	// Messages
	void reset();
	void showMessage(const std::string& message, const std::string& secondaryMessage);

private:
	// Fields
	spTextField m_primaryMessage;
	spTextField m_secondaryMessage;
	spSprite m_overlay;
};
