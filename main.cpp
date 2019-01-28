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
            buttons.emplace_back(Rect2i(left, top, maxWidth, height), false);
            for (auto &button : buttons) {
                button.first.width = maxWidth;
            }
        } else {
            buttons.emplace_back(Rect2i(left, top, maxWidth, height), false);
        }
        top += height;
        for (size_t i = 0; i != buttons.size(); i++) {
        }
        buttonNames.push_back(std::move(buttonName));
    }

    void imshow(const String& winname, InputOutputArray canvas){
        auto mouse_callBack = [](int event, int x, int y, int flags, void *userdata) {
            if (event == EVENT_LBUTTONDOWN) {
                for (auto &[box,state] : *static_cast<std::vector<std::pair<Rect2i, bool>>*>(userdata)) {
                    if (x > box.x and x < box.x + box.width and y > box.y and y < box.y + box.height) {
                        state = !state;
                        fmt::print("Changed state\n");
                    }
                }
            }
        };

        for (size_t i = 0; i != buttons.size(); i++) {
            rectangle(canvas, buttons[i].first, {255, 0, 0}, 2);
            putText(canvas, buttonNames[i], {buttons[i].first.x, buttons[i].first.y + height - baseline}, fontFace, fontScale, 0, thickness);
        }
        namedWindow(winname);
        setMouseCallback(winname, mouse_callBack, static_cast<void *>(&buttons));
        imshow(winname, canvas);
    }

  private:
    std::vector<std::pair<Rect2i, bool>> buttons;
    std::vector<String> buttonNames;
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
    buttonArray.imshow("debug", canvas);
    waitKey(0);
}
