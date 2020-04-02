//
// Created by petr on 2/22/20.
//
#include "Panel.h"

auto ui::Panel::setVisible(bool visible) noexcept -> void { Panel::visible = visible; }

auto ui::Panel::isVisible() const noexcept -> bool { return visible; }