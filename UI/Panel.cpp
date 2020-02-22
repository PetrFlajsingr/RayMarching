//
// Created by petr on 2/22/20.
//
#include "Panel.h"

void ui::Panel::setVisible(bool visible) noexcept { Panel::visible = visible; }

bool ui::Panel::isVisible() const noexcept { return visible; }