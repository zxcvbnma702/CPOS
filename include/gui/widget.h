#ifndef __CPOS__GUI__WIDGET_H
#define __CPOS__GUI__WIDGET_H

#include "common/types.h"
#include "common/graphicscontext.h"
#include "drivers/keyboard.h"

namespace cpos {
    namespace gui {
        class Widget : public drivers::KeyboardEventHandler {
        public:
            Widget(Widget* parent, cpos::common::int32_t x, cpos::common::int32_t y,
                cpos::common::int32_t w, cpos::common::int32_t h, cpos::common::uint8_t r, 
                cpos::common::uint8_t g, cpos::common::uint8_t b);
            ~Widget();

            //获取焦点
            virtual void GetFocus(Widget* widget);
            //窗口坐标计算
            virtual void ModelToScreen(cpos::common::int32_t &x, cpos::common::int32_t &y);
            //点击点是否在框内
            virtual bool ContainsCoordinate(cpos::common::int32_t x, cpos::common::int32_t y);

            virtual void Draw(cpos::common::GraphicsContext* gc);
            virtual void OnMouseDown(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::uint8_t button);
            virtual void OnMouseUp(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::uint8_t button);
            virtual void OnMouseMove(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::int32_t nx, cpos::common::int32_t ny);
        protected:
            Widget* parent;
            cpos::common::int32_t x, y, w, h;
            cpos::common::uint8_t r, g, b;
            bool Focussable;
        };

        class CompositeWidget : public Widget {
        public:
            CompositeWidget(Widget* parent, cpos::common::int32_t x, cpos::common::int32_t y,
                cpos::common::int32_t w, cpos::common::int32_t h, cpos::common::uint8_t r, 
                cpos::common::uint8_t g, cpos::common::uint8_t b);
            ~CompositeWidget();

            virtual void GetFocus(Widget* widget);
            virtual bool AddChild(Widget* child);
            // virtual void ModelToScreen(cpos::common::int32_t &x, cpos::common::int32_t &y);

            virtual void Draw(cpos::common::GraphicsContext* gc) override;
            virtual void OnMouseDown(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::uint8_t button) override;
            virtual void OnMouseUp(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::uint8_t button) override;
            virtual void OnMouseMove(cpos::common::int32_t x, cpos::common::int32_t y, cpos::common::int32_t nx, cpos::common::int32_t ny) override;

            virtual void OnKeyDown(char x) override;
            virtual void OnKeyUp(char x) override;
        private:
            Widget* children[100];
            cpos::common::int32_t numClidren;
            Widget* focussedChild;
        };
    }
}

#endif