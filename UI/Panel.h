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

  void setVisible(bool visible) noexcept;
  [[nodiscard]] bool isVisible() const noexcept;
private:
  bool visible = true;
};
}

#endif // RAYMARCHING_PANEL_H
