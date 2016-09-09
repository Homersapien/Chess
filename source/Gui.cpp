#include "Gui.h"
#include "Game.h"

Gui::Gui()
{
	// Create an overlay
	m_overlay = new Sprite();
	m_overlay->setResAnim(Game::instance()->resources().getResAnim("overlay"));
	m_overlay->setPosition(Vector2(0, 0));
	m_overlay->setScale(Vector2(core::getDisplaySize()).div(m_overlay->getSize()));
	m_overlay->setAlpha(OVERLAY_OPACITY);
	m_overlay->attachTo(getStage());
	m_overlay->setVisible(false);

	// Create a message label
	m_primaryMessage = new TextField();
	m_primaryMessage->attachTo(getStage());
	m_primaryMessage->setPosition(Vector2(core::getDisplaySize()) / 2);
	m_primaryMessage->setVisible(false);

	TextStyle primaryStyle;
	primaryStyle.font = Game::instance()->resources().getResFont("arial_huge")->getFont();
	primaryStyle.color = TEXT_COLOR_NORMAL;
	primaryStyle.vAlign = TextStyle::VALIGN_BOTTOM;
	primaryStyle.hAlign = TextStyle::HALIGN_CENTER;
	primaryStyle.fontSize = 70;
	m_primaryMessage->setStyle(primaryStyle);

	// Create a secondary label
	m_secondaryMessage = new TextField();
	m_secondaryMessage->attachTo(getStage());
	m_secondaryMessage->setPosition((Vector2(core::getDisplaySize()) + Vector2(0, 60)) / 2);
	m_secondaryMessage->setVisible(false);

	TextStyle secondaryStyle;
	secondaryStyle.font = Game::instance()->resources().getResFont("arial")->getFont();
	secondaryStyle.color = TEXT_COLOR_SUBDUED;
	secondaryStyle.vAlign = TextStyle::VALIGN_BOTTOM;
	secondaryStyle.hAlign = TextStyle::HALIGN_CENTER;
	secondaryStyle.fontSize = 20;
	m_secondaryMessage->setStyle(secondaryStyle);
}

Gui::~Gui()
{
	m_primaryMessage->detach();
	m_secondaryMessage->detach();
	m_overlay->detach();
}

void Gui::reset()
{
	m_primaryMessage->setVisible(false);
	m_secondaryMessage->setVisible(false);
	m_overlay->setVisible(false);
}

void Gui::showMessage(const std::string& message, const std::string& secondaryMessage)
{
	m_primaryMessage->setText(message);
	m_primaryMessage->setVisible(true);
	m_secondaryMessage->setText(secondaryMessage);
	m_secondaryMessage->setVisible(true);
	m_overlay->setVisible(true);
}