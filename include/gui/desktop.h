#ifndef __CPOS__GUI_DESKTOP_H
#define __CPOS__GUI_DESKTOP_H

#include "gui/widget.h"
#include "drivers/mouse.h"

namespace cpos {
    namespace gui {
        class Desktop : public CompositeWidget, public drivers::MouseEventHandler {
        public:
            Desktop(cpos::common::int32_t w, cpos::common::int32_t h, cpos::common::uint8_t r, cpos::common::uint8_t g, cpos::common::uint8_t b);
            ~Desktop();

            void Draw(cpos::common::GraphicsContext* gc) override;
            void OnMouseDown(cpos::common::uint8_t button) override;
            void OnMouseUp(cpos::common::uint8_t button) override;
            void OnMouseMove(cpos::common::int8_t x, cpos::common::int8_t y) override;
        private:
            cpos::common::uint32_t MouseX, MouseY;
        };
    }
}

#endif 