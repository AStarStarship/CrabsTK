#include "cvplot/cvplot.h"

#include <opencv2/opencv.hpp>

namespace demo {

void example() {
  _::figure("myplot").series("myline").addValue({1.f, 3.f, 2.f, 5.f, 4.f});
  _::figure("myplot").show();
}

void demo() {
  std::vector<std::pair<float, float>> data;
  std::vector<float> values;

  _::Window::current("cvplot demo").offset({60, 100});

  {
    auto name = "simple";
    _::setWindowTitle(name, "line and histogram");
    _::moveWindow(name, 0, 0);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    figure.series("line")
        .setValue({1.f, 2.f, 3.f, 4.f, 5.f})
        .type(_::DotLine)
        .color(_::Blue);
    figure.series("histogram")
        .setValue({1.f, 2.f, 3.f, 4.f, 5.f})
        .type(_::Histogram)
        .color(_::Red);
    figure.show(false);
  }

  {
    auto name = "math";
    _::setWindowTitle(name, "math curves");
    _::moveWindow(name, 300, 0);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    values.clear();
    for (auto i = 0; i <= 10; i++) {
      values.push_back((i - 4) * (i - 4) - 6);
    }
    figure.series("parabola")
        .setValue(values)
        .type(_::DotLine)
        .color(_::Green);
    values.clear();
    for (auto i = 0; i <= 10; i++) {
      values.push_back(sin(i / 1.5f) * 5);
    }
    figure.series("sine")
        .setValue(values)
        .type(_::DotLine)
        .color(_::Blue);
    values.clear();
    values.push_back(15);
    figure.series("threshold")
        .setValue(values)
        .type(_::Horizontal)
        .color(_::Red);
    figure.show(false);
  }

  {
    auto name = "scatter";
    _::setWindowTitle(name, "scatter plots");
    _::moveWindow(name, 600, 0);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.push_back({(rand() % 100) / 10.f, (rand() % 100) / 10.f});
    }
    figure.series("uniform").set(data).type(_::Dots).color(_::Orange);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.push_back(
          {exp((rand() % 100) / 30.f) - 1, exp((rand() % 100) / 30.f) - 1});
    }
    figure.series("exponential")
        .set(data)
        .type(_::Dots)
        .color(_::Magenta);
    figure.show(false);
  }

  {
    auto name = "histograms";
    _::setWindowTitle(name, "multiple histograms");
    _::moveWindow(name, 0, 300);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    figure.series("1")
        .setValue({1.f, 2.f, 3.f, 4.f, 5.f})
        .type(_::Histogram)
        .color(_::Blue.alpha(201));
    figure.series("2")
        .setValue({6.f, 5.f, 4.f, 3.f, 2.f, 1.f})
        .type(_::Histogram)
        .color(_::Green.alpha(201));
    figure.series("3")
        .setValue({3.f, 1.f, -1.f, 1.f, 3.f, 7.f})
        .type(_::Histogram)
        .color(_::Red.alpha(201));
    figure.show(false);
  }

  {
    auto name = "parametric";
    _::setWindowTitle(name, "parametric plots");
    _::moveWindow(name, 0, 600);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    figure.square(true);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.push_back({cos(i * .0628f + 4) * 2, sin(i * .0628f + 4) * 2});
    }
    figure.series("circle").add(data);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.push_back({cos(i * .2513f + 1), sin(i * .0628f + 4)});
    }
    figure.series("lissajous").add(data);
    figure.show(false);
  }

  {
    auto name = "no-axis";
    _::setWindowTitle(name, "hidden axis");
    _::moveWindow(name, 600, 600);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    figure.origin(false, false);
    figure.series("histogram")
        .setValue({4.f, 5.f, 7.f, 6.f})
        .type(_::Vistogram)
        .color(_::Blue);
    figure.series("min")
        .setValue(4.f)
        .type(_::Vertical)
        .color(_::Pink);
    figure.series("max")
        .setValue(7.f)
        .type(_::Vertical)
        .color(_::Purple);
    figure.show(false);
  }

  {
    auto name = "colors";
    _::setWindowTitle(name, "auto color");
    _::moveWindow(name, 900, 0);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    figure.series("color")
        .dynamicColor(true)
        .type(_::Vistogram)
        .legend(false);
    for (auto i = 0; i < 16; i++) {
      figure.series("color").addValue(6, _::Color::index(i).hue());
    }
    figure.show(false);
  }

  {
    auto name = "fill";
    _::setWindowTitle(name, "filled line");
    _::moveWindow(name, 900, 0);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    figure.gridSize(20);
    figure.series("fossil").type(_::FillLine).color(_::Orange);
    figure.series("electric")
        .type(_::FillLine)
        .color(_::Green.gamma(.5f));
    for (auto i = 0; i < 16; i++) {
      figure.series("fossil").addValue(10 - i + 10.f * rand() / RAND_MAX);
      figure.series("electric").addValue(i - 10 + 10.f * rand() / RAND_MAX);
    }
    figure.show(false);
  }

  {
    auto name = "range";
    _::setWindowTitle(name, "range plot");
    _::moveWindow(name, 900, 300);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    values.clear();
    figure.series("apples").type(_::RangeLine).color(_::Orange);
    figure.series("pears").type(_::RangeLine).color(_::Sky);
    for (auto i = 0; i <= 10; i++) {
      float v = (i - 4) * (i - 4) - 6;
      figure.series("apples").addValue(v + 10.f + 5.f * rand() / RAND_MAX,
                                       v + 5.f * rand() / RAND_MAX,
                                       v + 20.f + 5.f * rand() / RAND_MAX);
      v = -(i - 6) * (i - 6) + 30;
      figure.series("pears").addValue(v + 10.f + 5.f * rand() / RAND_MAX,
                                      v + 5.f * rand() / RAND_MAX,
                                      v + 20.f + 5.f * rand() / RAND_MAX);
    }
    figure.show(false);
  }

  {
    auto name = "balls";
    _::setWindowTitle(name, "transparent circles");
    _::moveWindow(name, 300, 600);
    _::resizeWindow(name, 300, 300);
    auto &figure = _::figure(name);
    figure.series("purple")
        .type(_::Circle)
        .color(_::Purple.alpha(192));
    figure.series("aqua").type(_::Circle).color(_::Aqua.alpha(193));
    for (auto i = 0; i <= 20; i++) {
      figure.series("purple").add(
          (rand() % 100) / 10.f, {(rand() % 100) / 10.f, (rand() % 100) / 5.f});
      figure.series("aqua").add((rand() % 100) / 10.f,
                                {(rand() % 100) / 10.f, (rand() % 100) / 5.f});
    }
    figure.show(false);
  }

  {
    auto name = "image";
    _::setWindowTitle(name, "image and text");
    _::moveWindow(name, 900, 600);
    _::resizeWindow(name, 300, 300);
    auto &view = _::Window::current().view(name);
    auto image = cv::imread("res/demo.jpg");
    cv::copyMakeBorder(image, image, 100, 100, 0, 0, cv::BORDER_REPLICATE);
    view.drawImage(&image);
    view.drawText("..and text", {13, 273}, _::Black.alpha(127));
    view.finish();
  }

  {
    auto name = "dynamic";
    _::setWindowTitle(name, "dynamic plotting");
    _::moveWindow(name, 300, 300);
    _::resizeWindow(name, 600, 300);
    auto &view = _::Window::current().view(name);
    auto &figure = _::figure(name);
    figure.square(true);
    figure.origin(false, false);
    srand(clock());
    auto x = 0.f, y = 0.f, dx = 1.f, dy = 0.f, f = 0.f, df = 0.f;
    figure.series("random").dynamicColor(true).legend(false);
    clock_t time = 0;
    for (int i = 0; i < 1000; i++) {
      auto fps = CLOCKS_PER_SEC / (float)(clock() - time);
      time = clock();
      auto l = sqrt((dx * dx + dy * dy) * (f * f + 1)) * 10;
      dx = (dx + f * dy) / l;
      dy = (dy - f * dx) / l;
      f = (f + df) * 0.8f;
      df = (df + rand() % 11 / 100.f - .05f) * 0.8f;
      figure.series("random").add(x += dx, {y += dy, i / 10.f});
      figure.show(false);
      auto string = std::to_string(fps).substr(0, 4) + " fps  " +
                    std::to_string(i / 10.f).substr(0, 4) + "%";
      view.drawText(string, {480, 277}, _::Gray);
      view.finish();
      view.flush();
    }
  }
}

void mouse_callback(int event, int x, int y, int flags, void *param) {
  auto &view = *(_::View *)param;
  std::ostringstream stream;
  if (event == cv::EVENT_MOUSEMOVE) stream << "mousemove";
  if (event == cv::EVENT_LBUTTONDOWN) stream << "lbuttondown";
  if (event == cv::EVENT_RBUTTONDOWN) stream << "rbuttondown";
  if (event == cv::EVENT_MBUTTONDOWN) stream << "mbuttondown";
  if (event == cv::EVENT_LBUTTONUP) stream << "lbuttonup";
  if (event == cv::EVENT_RBUTTONUP) stream << "rbuttonup";
  if (event == cv::EVENT_MBUTTONUP) stream << "mbuttonup";
  if (event == cv::EVENT_LBUTTONDBLCLK) stream << "lbuttondblclk";
  if (event == cv::EVENT_RBUTTONDBLCLK) stream << "rbuttondblclk";
  if (event == cv::EVENT_MBUTTONDBLCLK) stream << "mbuttondblclk";
#if CV_MAJOR_VERSION > 2
  if (event == cv::EVENT_MOUSEWHEEL) stream << "mousewheel";
  if (event == cv::EVENT_MOUSEHWHEEL) stream << "mousehwheel";
#endif
  if (flags & cv::EVENT_FLAG_LBUTTON) stream << " lbutton";
  if (flags & cv::EVENT_FLAG_RBUTTON) stream << " rbutton";
  if (flags & cv::EVENT_FLAG_MBUTTON) stream << " mbutton";
  if (flags & cv::EVENT_FLAG_CTRLKEY) stream << " ctrlkey";
  if (flags & cv::EVENT_FLAG_SHIFTKEY) stream << " shiftkey";
  if (flags & cv::EVENT_FLAG_ALTKEY) stream << " altkey";
  stream << "  " << x << "," << y;
  view.drawRect({10, 24, 280, 12}, _::Light);
  view.drawText(stream.str(), {10, 25}, _::Black);
}

void transparency() {
  std::vector<std::pair<float, float>> data;
  std::vector<float> values;

  _::Window::current("cvplot transparency").offset({30, 70}).cursor(true);

  {
    auto name = "opaque";
    _::setWindowTitle(name, "opaque");
    _::moveWindow(name, 0, 0);
    _::resizeWindow(name, 300, 300);
    _::setMouseCallback(name, mouse_callback);
    _::Window::current().view(name).frameColor(_::Sky);
    auto &figure = _::figure(name);
    figure.series("histogram")
        .setValue({1.f, 2.f, 3.f, 4.f, 5.f})
        .type(_::Histogram)
        .color(_::Red)
        .legend(false);
    figure.show(false);
  }

  {
    auto name = "transparent";
    auto alpha = 200;
    _::setWindowTitle(name, "transparent");
    _::moveWindow(name, 100, 100);
    _::resizeWindow(name, 300, 300);
    _::setMouseCallback(name, mouse_callback);
    _::Window::current().view(name).frameColor(_::Sky).alpha(alpha);
    auto &figure = _::figure(name);
    figure.series("histogram")
        .setValue({5.f, 4.f, 3.f, 2.f, 1.f})
        .type(_::Histogram)
        .color(_::Blue.alpha(alpha))
        .legend(false);
    figure.alpha(alpha).show(true);
  }
}

}  // namespace demo

int main(int argc, char **argv) {
  demo::example();
  demo::transparency();
  demo::demo();
  return 0;
}
