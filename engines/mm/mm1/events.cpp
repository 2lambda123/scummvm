/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common/system.h"
#include "graphics/screen.h"
#include "mm/mm1/events.h"
#include "mm/mm1/mm1.h"
#include "mm/mm1/gfx/gfx.h"
#include "mm/mm1/views/dialogs.h"
#include "mm/mm1/views_enh/dialogs.h"

namespace MM {
namespace MM1 {

Events *g_events;

Events::Events(bool enhancedMode) : UIElement("Root", nullptr),
		_enhancedMode(enhancedMode) {
	g_events = this;
}

Events::~Events() {
	g_events = nullptr;
}

void Events::runGame() {
	UIElement *allViews = _enhancedMode ?
		(UIElement *)new ViewsEnh::Dialogs() :
		(UIElement *)new Views::Dialogs();
	uint currTime, nextFrameTime = 0;
	_screen = new Graphics::Screen();

	// Run the game
	// ***DEBUG*** start directly in-game
	for (uint i = 0; i < 6; ++i)
		g_globals->_party.push_back(g_globals->_roster[i]);
	g_globals->_maps.loadTown(Maps::SORPIGAL);
	msgGame(GameMessage("UPDATE"));
	//addView("Title");

	Common::Event e;
	for (;;) {
		while (g_system->getEventManager()->pollEvent(e)) {
			if (e.type == Common::EVENT_QUIT ||
					e.type == Common::EVENT_RETURN_TO_LAUNCHER)
				return;
			else
				processEvent(e);
		}

		g_system->delayMillis(10);
		if ((currTime = g_system->getMillis()) >= nextFrameTime) {
			nextFrameTime = currTime + FRAME_DELAY;
			tick();
			drawElements();
			_screen->update();
		}
	}

	delete _screen;
	delete allViews;
}

void Events::processEvent(Common::Event &ev) {
	switch (ev.type) {
	case Common::EVENT_KEYDOWN:
		if (ev.kbd.keycode < Common::KEYCODE_NUMLOCK)
			msgKeypress(KeypressMessage(ev.kbd));
		break;
	case Common::EVENT_CUSTOM_ENGINE_ACTION_START:
		msgAction(ActionMessage((KeybindingAction)ev.customType));
		break;
	case Common::EVENT_LBUTTONDOWN:
	case Common::EVENT_RBUTTONDOWN:
		//case Common::EVENT_MBUTTONDOWN:
		msgMouseDown(MouseDownMessage(ev.type, ev.mouse));
		break;
	case Common::EVENT_LBUTTONUP:
	case Common::EVENT_RBUTTONUP:
	//case Common::EVENT_MBUTTONUP:
		msgMouseUp(MouseUpMessage(ev.type, ev.mouse));
		break;
	default:
		break;
	}
}

void Events::replaceView(UIElement *ui) {
	assert(ui);
	if (!_views.empty()) {
		focusedView()->msgUnfocus(UnfocusMessage());
		_views.clear();
	}

	_views.push(ui);
	ui->redraw();
	ui->msgFocus(FocusMessage());
}

void Events::replaceView(const Common::String &name) {
	replaceView(findView(name));
}

void Events::addView(UIElement *ui) {
	assert(ui);
	if (!_views.empty())
		focusedView()->msgUnfocus(UnfocusMessage());

	_views.push(ui);
	ui->redraw();
	ui->msgFocus(FocusMessage());
}

void Events::addView(const Common::String &name) {
	addView(findView(name));
}

void Events::popView() {
	focusedView()->msgUnfocus(UnfocusMessage());
	_views.pop();

	if (!_views.empty()) {
		focusedView()->redraw();
		focusedView()->msgFocus(FocusMessage());
	}
}

/*------------------------------------------------------------------------*/

UIElement::UIElement(const Common::String &name, UIElement *uiParent) :
		_name(name), _parent(uiParent),
		_bounds(0, 0, SCREEN_W, SCREEN_H) {
	if (_parent)
		_parent->_children.push_back(this);
}

void UIElement::redraw() {
	_needsRedraw = true;

	for (size_t i = 0; i < _children.size(); ++i)
		_children[i]->redraw();
}

void UIElement::drawElements() {
	if (_needsRedraw) {
		draw();
		_needsRedraw = false;
	}

	for (size_t i = 0; i < _children.size(); ++i)
		_children[i]->drawElements();
}

UIElement *UIElement::findViewGlobally(const Common::String &name) {
	return g_events->findView(name);
}

void UIElement::focus() {
	g_engine->replaceView(this);
}

void UIElement::close() {
	assert(g_engine->focusedView() == this);
	g_engine->popView();
}

void UIElement::clearSurface() {
	Graphics::ManagedSurface s = getSurface();
	s.fillRect(Common::Rect(s.w, s.h), 0);
}

void UIElement::draw() {
	for (size_t i = 0; i < _children.size(); ++i) {
		_children[i]->draw();
	}
}

bool UIElement::tick() {
	for (size_t i = 0; i < _children.size(); ++i) {
		if (_children[i]->tick())
			return true;
	}

	return false;
}

UIElement *UIElement::findView(const Common::String &name) {
	if (_name.equalsIgnoreCase(name))
		return this;

	UIElement *result;
	for (size_t i = 0; i < _children.size(); ++i) {
		if ((result = _children[i]->findView(name)) != nullptr)
			return result;
	}

	return nullptr;
}

void UIElement::replaceView(UIElement *ui) {
	g_events->replaceView(ui);
}

void UIElement::replaceView(const Common::String &name) {
	g_events->replaceView(name);
}

void UIElement::addView(UIElement *ui) {
	g_events->addView(ui);
}

void UIElement::addView(const Common::String &name) {
	g_events->addView(name);
}

void UIElement::addView() {
	g_events->addView(this);
}

Graphics::ManagedSurface UIElement::getSurface() const {
	return Graphics::ManagedSurface(*g_events->getScreen(), _bounds);
}

} // namespace MM1
} // namespace MM