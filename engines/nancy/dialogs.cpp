/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/translation.h"

#include "gui/ThemeEval.h"

#include "engines/nancy/dialogs.h"

namespace Nancy {

NancyOptionsWidget::NancyOptionsWidget(GuiObject *boss, const Common::String &name, const Common::String &domain) :
		OptionsContainerWidget(boss, name, "NancyOptionsDialog", false, domain) {
	_playerSpeechCheckbox = new GUI::CheckboxWidget(widgetsBoss(), "NancyOptionsDialog.PlayerSpeech", _("Player Speech"), _("Enable player speech. Only works if speech is enabled in the Audio settings."));
	_characterSpeechCheckbox = new GUI::CheckboxWidget(widgetsBoss(), "NancyOptionsDialog.CharacterSpeech", _("Character Speech"), _("Enable NPC speech. Only works if speech is enabled in the Audio settings."));

	new GUI::StaticTextWidget(widgetsBoss(), "NancyOptionsDialog.SpeechSettingsLabel", _("Speech Options"));
}

void NancyOptionsWidget::load() {
	_playerSpeechCheckbox->setState(ConfMan.getBool("player_speech", _domain));
	_characterSpeechCheckbox->setState(ConfMan.getBool("character_speech", _domain));
}

bool NancyOptionsWidget::save() {
	ConfMan.setBool("player_speech", _playerSpeechCheckbox->getState(), _domain);
	ConfMan.setBool("character_speech", _characterSpeechCheckbox->getState(), _domain);

	return true;
}

void NancyOptionsWidget::defineLayout(GUI::ThemeEval &layouts, const Common::String &layoutName, const Common::String &overlayedLayout) const {
	layouts.addDialog(layoutName, overlayedLayout)
		.addLayout(GUI::ThemeLayout::kLayoutVertical)
			.addPadding(16, 16, 16, 16)
			.addWidget("SpeechSettingsLabel", "OptionsLabel")
			.addWidget("PlayerSpeech", "Checkbox")
			.addWidget("CharacterSpeech", "Checkbox")
		.closeLayout()
	.closeDialog();
}

bool NancyOptionsWidget::isInGame() const {
	return _domain.equals(ConfMan.getActiveDomainName());
}


} // End of namespace Nancy
