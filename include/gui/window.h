#ifndef __CPOS__GUI_WINDOW_H
#define __CPOS__GUI_WINDOW_H

#include "gui/widget.h"
#include "drivers/mouse.h"

namespace cpos {
    namespace gui { 
        class Window : public CompositeWidget {
        public:
            Window(Widget* parent, cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::int32_t w, cpos::common::int32_t h,
                cpos::common::uint8_t r, cpos::common::uint8_t g, cpos::common::uint8_t b);
            ~Window();

            void OnMouseDown(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::uint8_t button) override;
            void OnMouseUp(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::uint8_t button) override;
            void OnMouseMove(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::int32_t nx, cpos::common::int32_t ny) override;
        private:
            bool Dragging;
        };
    }
}

#endif 