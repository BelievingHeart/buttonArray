#include "opencv2/opencv.hpp"
#include "src/timer.hpp"
#include <fmt/printf.h>

using namespace cv;

struct ButtonArray {
    ButtonArray(double fontScale, int thickness, int fontFace = FONT_HERSHEY_SIMPLEX) : fontScale(fontScale), thickness(thickness), fontFace(fontFace) {}
    void addButton(String buttonName) {
        const Size sz = getTextSize(buttonName, fontFace, fontScale, thickness, &baseline);
        // initialize height for the first push_back
        if (buttons.empty()) {
            height = baseline + sz.height + thickness;
        }
        // if new box is the widest, updates all boxes' width
        if (sz.width > maxWidth) {
            maxWidth = sz.width;
            buttons.emplace_back(left, top, maxWidth, height);
            for (auto &button : buttons) {
                button.width = maxWidth;
            }
        } else {
            buttons.emplace_back(left, top, maxWidth, height);
        }
        top += height;
        for (size_t i = 0; i != buttons.size(); i++) {
        }
        buttonNames.push_back(std::move(buttonName));
        states.push_back(false);
    }

    void draw(InputOutputArray canvas){
        for (size_t i = 0; i != buttons.size(); i++) {
            rectangle(canvas, buttons[i], {255, 0, 0}, 2);
            putText(canvas, buttonNames[i], {buttons[i].x, buttons[i].y + height - baseline}, fontFace, fontScale, 0, thickness);
        }
    }

  private:
    std::vector<Rect2i> buttons;
    std::vector<String> buttonNames;
    std::vector<bool> states;
    double fontScale;
    int thickness, fontFace, baseline = 0, height = 0;
    int maxWidth = -1;
    int top = 5, left = 5;
};

int main() {
    Mat canvas(480, 640, CV_8UC3, {255, 255, 255});
    ButtonArray buttonArray(1, 2);
    buttonArray.addButton("hello");
    buttonArray.addButton("afterburner");
    buttonArray.addButton("world");
    buttonArray.draw(canvas);
    imshow("debug", canvas);
    waitKey(0);
}
