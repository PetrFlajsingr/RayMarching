//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_PANEL_H
#define RAYMARCHING_PANEL_H

namespace ui {
class Panel {
public:
  virtual void onFrame() = 0;
  virtual ~Panel() = default;

  auto setVisible(bool visible) noexcept -> void;
  [[nodiscard]] auto isVisible() const noexcept -> bool;

private:
  bool visible = true;
};
} // namespace ui

#endif // RAYMARCHING_PANEL_H
