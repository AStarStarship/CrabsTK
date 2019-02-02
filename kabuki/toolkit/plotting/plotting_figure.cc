#include "internal.h"
#include "t_figure.h"
#include "t_window.h"

#include <opencv2/imgproc/imgproc.hpp>

namespace _ {

namespace {
std::map<std::string, Figure> shared_figures_;
}

void Series::verifyParams() const {
  auto dims = 1;
  auto depth = 0;
  switch (type_) {
    case Line:
    case DotLine:
    case Dots:
    case FillLine:
    case Vistogram:
    case Histogram:
    case Horizontal:
    case Vertical: {
      depth = 1;
      break;
    }
    case RangeLine: {
      depth = 3;
      break;
    }
    case Range:
    case Circle: {
      depth = 2;
      break;
    }
  }
  if (dynamic_color_) {
    depth += 1;
  }
  if (entries_.size() > 0) {
    EXPECT_EQ(dims_, dims);
    EXPECT_EQ(depth_, depth);
  }
}

void Series::ensureDimsDepth(SIN dims, SIN depth) {
  if (dims_ != dims) {
    if (dims_ != 0) {
      std::cerr << "incorrect dims (input dimensions), was " << dims_ << " now "
                << dims << std::endl;
    }
    dims_ = dims;
  }
  if (depth_ != depth) {
    if (depth_ != 0) {
      std::cerr << "incorrect depth (output dimensions), was " << depth_
                << " now " << depth << std::endl;
    }
    depth_ = depth;
  }
}

Series &Series::clear() {
  entries_.clear();
  data_.clear();
  dims_ = 0;
  depth_ = 0;
  return *this;
}

Series &Series::type(enum Type type) {
  type_ = type;
  return *this;
}

Series &Series::color(TRGBA color) {
  color_ = color;
  return *this;
}

Series &Series::dynamicTRGBA(bool dynamic_color) {
  dynamic_color_ = dynamic_color;
  return *this;
}

Series &Series::legend(bool legend) {
  legend_ = legend;
  return *this;
}

Series &Series::add(const std::vector<std::pair<FP4, FP4>> &data) {
  ensureDimsDepth(1, 1);
  for (const auto &d : data) {
    entries_.push_back(data_.size());
    data_.push_back(d.first);
    data_.push_back(d.second);
  }
  return *this;
}

Series &Series::add(const std::vector<std::pair<FP4, Point2>> &data) {
  ensureDimsDepth(1, 2);
  for (const auto &d : data) {
    entries_.push_back(data_.size());
    data_.push_back(d.first);
    data_.push_back(d.second.x);
    data_.push_back(d.second.y);
  }
  return *this;
}

Series &Series::add(const std::vector<std::pair<FP4, Point3>> &data) {
  ensureDimsDepth(1, 3);
  for (const auto &d : data) {
    entries_.push_back(data_.size());
    data_.push_back(d.first);
    data_.push_back(d.second.x);
    data_.push_back(d.second.y);
    data_.push_back(d.second.z);
  }
  return *this;
}

Series &Series::addValue(const std::vector<FP4> &values) {
  std::vector<std::pair<FP4, FP4>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i + entries_.size();
    d.second = values[i++];
  }
  return add(data);
}

Series &Series::addValue(const std::vector<Point2> &values) {
  std::vector<std::pair<FP4, Point2>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i + entries_.size();
    d.second = values[i++];
  }
  return add(data);
}

Series &Series::addValue(const std::vector<Point3> &values) {
  std::vector<std::pair<FP4, Point3>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i + entries_.size();
    d.second = values[i++];
  }
  return add(data);
}

Series &Series::add(FP4 key, FP4 value) {
  return add(std::vector<std::pair<FP4, FP4>>({{key, value}}));
}

Series &Series::add(FP4 key, Point2 value) {
  return add(std::vector<std::pair<FP4, Point2>>({{key, value}}));
}

Series &Series::add(FP4 key, Point3 value) {
  return add(std::vector<std::pair<FP4, Point3>>({{key, value}}));
}

Series &Series::addValue(FP4 value) {
  return addValue(std::vector<FP4>({value}));
}

Series &Series::addValue(FP4 value_a, FP4 value_b) {
  return addValue(std::vector<Point2>({{value_a, value_b}}));
}

Series &Series::addValue(FP4 value_a, FP4 value_b, FP4 value_c) {
  return addValue(std::vector<Point3>({{value_a, value_b, value_c}}));
}

Series &Series::set(const std::vector<std::pair<FP4, FP4>> &data) {
  clear();
  return add(data);
}

Series &Series::set(const std::vector<std::pair<FP4, Point2>> &data) {
  clear();
  return add(data);
}

Series &Series::set(const std::vector<std::pair<FP4, Point3>> &data) {
  clear();
  return add(data);
}

Series &Series::setValue(const std::vector<FP4> &values) {
  std::vector<std::pair<FP4, FP4>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i;
    d.second = values[i++];
  }
  return set(data);
}

Series &Series::setValue(const std::vector<Point2> &values) {
  std::vector<std::pair<FP4, Point2>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i;
    d.second = values[i++];
  }
  return set(data);
}

Series &Series::setValue(const std::vector<Point3> &values) {
  std::vector<std::pair<FP4, Point3>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i;
    d.second = values[i++];
  }
  return set(data);
}

Series &Series::set(FP4 key, FP4 value) {
  return set(std::vector<std::pair<FP4, FP4>>({{key, value}}));
}

Series &Series::set(FP4 key, FP4 value_a, FP4 value_b) {
  return set(
      std::vector<std::pair<FP4, Point2>>({{key, {value_a, value_b}}}));
}

Series &Series::set(FP4 key, FP4 value_a, FP4 value_b, FP4 value_c) {
  return set(std::vector<std::pair<FP4, Point3>>(
      {{key, {value_a, value_b, value_c}}}));
}

Series &Series::setValue(FP4 value) {
  return setValue(std::vector<FP4>({value}));
}

Series &Series::setValue(FP4 value_a, FP4 value_b) {
  return setValue(std::vector<Point2>({{value_a, value_b}}));
}

Series &Series::setValue(FP4 value_a, FP4 value_b, FP4 value_c) {
  return setValue(std::vector<Point3>({{value_a, value_b, value_c}}));
}

const std::string &Series::label() const { return label_; }

bool Series::legend() const { return legend_; }

TRGBA Series::color() const { return color_; }

bool Series::collides() const {
  return type_ == Histogram || type_ == Vistogram;
}

bool Series::flipAxis() const {
  return type_ == Vertical || type_ == Vistogram;
}

void Series::bounds(FP4 &x_min, FP4 &x_max, FP4 &y_min, FP4 &y_max,
                    SIN &n_max, SIN &p_max) const {
  for (const auto &e : entries_) {
    auto xe = e, xd = dims_, ye = e + dims_,
         yd = depth_ - (dynamic_color_ ? 1 : 0);
    if (type_ == Circle) {
      yd = 1;
    }
    if (flipAxis()) {
      auto s = xe;
      xe = ye;
      ye = s;
      s = xd;
      xd = yd;
      yd = s;
    }
    if (type_ != Horizontal) {  // TODO: check Horizontal/Vertical logic
      EXPECT_EQ(xd, 1);
      const auto &x = data_[xe];
      if (x_min > x) {
        x_min = x;
      }
      if (x_max < x) {
        x_max = x;
      }
    }
    if (type_ != Vertical) {
      for (auto yi = ye, _y = yi + yd; yi != _y; yi++) {
        const auto &y = data_[yi];
        if (y_min > y) {
          y_min = y;
        }
        if (y_max < y) {
          y_max = y;
        }
      }
    }
  }
  if (n_max < entries_.size()) {
    n_max = entries_.size();
  }
  if (type_ == Histogram || type_ == Vistogram) {
    p_max = std::max(30, p_max);
  }
}

void Series::dot(void *b, SIN x, SIN y, SIN r) const {
  Trans trans(b);
  cv::circle(trans.with(color_), {x, y}, r, color2scalar(color_), -1,
             cv::LINE_AA);
}

void Series::draw(void *b, FP4 x_min, FP4 x_max, FP4 y_min, FP4 y_max,
                  FP4 xs, FP4 xd, FP4 ys, FP4 yd, FP4 x_axis,
                  FP4 y_axis, SIN unit, FP4 offset) const {
  if (dims_ == 0 || depth_ == 0) {
    return;
  }
  Trans trans(*(cv::Mat *)b);
  auto color = color2scalar(color_);
  switch (type_) {
    case Line:
    case DotLine:
    case Dots:
    case FillLine:
    case RangeLine: {
      if (type_ == FillLine) {
        bool has_last = false;
        FP4 last_x, last_y;
        for (const auto &e : entries_) {
          auto x = data_[e], y = data_[e + dims_];
          if (dynamic_color_) {
            color = color2scalar(TRGBA::cos(data_[e + dims_ + 1]));
          }
          cv::Point point((SIN)(x * xs + xd), (SIN)(y * ys + yd));
          if (has_last) {
            cv::Point points[4] = {
                point,
                {point.x, (SIN)(y_axis * ys + yd)},
                {(SIN)(last_x * xs + xd), (SIN)(y_axis * ys + yd)},
                {(SIN)(last_x * xs + xd), (SIN)(last_y * ys + yd)},
            };
            cv::fillConvexPoly(trans.with(color_.a / 2), points, 4, color,
                               cv::LINE_AA);
          } else {
            has_last = true;
          }
          last_x = x, last_y = y;
        }
      } else if (type_ == RangeLine) {
        bool has_last = false;
        FP4 last_x, last_y1, last_y2;
        for (const auto &e : entries_) {
          auto x = data_[e], y1 = data_[e + dims_ + 1],
               y2 = data_[e + dims_ + 2];
          if (dynamic_color_) {
            color = color2scalar(TRGBA::cos(data_[e + dims_ + 1]));
          }
          if (has_last) {
            cv::Point points[4] = {
                {(SIN)(x * xs + xd), (SIN)(y1 * ys + yd)},
                {(SIN)(x * xs + xd), (SIN)(y2 * ys + yd)},
                {(SIN)(last_x * xs + xd), (SIN)(last_y2 * ys + yd)},
                {(SIN)(last_x * xs + xd), (SIN)(last_y1 * ys + yd)},
            };
            cv::fillConvexPoly(trans.with(color_.a / 2), points, 4, color,
                               cv::LINE_AA);
          } else {
            has_last = true;
          }
          last_x = x, last_y1 = y1, last_y2 = y2;
        }
      }
      bool has_last = false;
      FP4 last_x, last_y;
      for (const auto &e : entries_) {
        auto x = data_[e], y = data_[e + dims_];
        if (dynamic_color_) {
          color = color2scalar(TRGBA::cos(data_[e + dims_ + 1]));
        }
        cv::Point point((SIN)(x * xs + xd), (SIN)(y * ys + yd));
        if (has_last) {
          if (type_ == DotLine || type_ == Line || type_ == FillLine ||
              type_ == RangeLine) {
            cv::line(trans.with(color_),
                     {(SIN)(last_x * xs + xd), (SIN)(last_y * ys + yd)}, point,
                     color, 1, cv::LINE_AA);
          }
        } else {
          has_last = true;
        }
        if (type_ == DotLine || type_ == Dots) {
          cv::circle(trans.with(color_), point, 2, color, 1, cv::LINE_AA);
        }
        last_x = x, last_y = y;
      }
    } break;
    case Vistogram:
    case Histogram: {
      auto u = 2 * unit;
      auto o = (SIN)(2 * u * offset);
      for (const auto &e : entries_) {
        auto x = data_[e], y = data_[e + dims_];
        if (dynamic_color_) {
          color = color2scalar(TRGBA::cos(data_[e + dims_ + 1]));
        }
        if (type_ == Histogram) {
          cv::rectangle(trans.with(color_),
                        {(SIN)(x * xs + xd) - u + o, (SIN)(y_axis * ys + yd)},
                        {(SIN)(x * xs + xd) + u + o, (SIN)(y * ys + yd)}, color,
                        -1, cv::LINE_AA);
        } else if (type_ == Vistogram) {
          cv::rectangle(trans.with(color_),
                        {(SIN)(x_axis * xs + xd), (SIN)(x * ys + yd) - u + o},
                        {(SIN)(y * xs + xd), (SIN)(x * ys + yd) + u + o}, color,
                        -1, cv::LINE_AA);
        }
      }

    } break;
    case Horizontal:
    case Vertical: {
      for (const auto &e : entries_) {
        auto y = data_[e + dims_];
        if (dynamic_color_) {
          color = color2scalar(TRGBA::cos(data_[e + dims_ + 1]));
        }
        if (type_ == Horizontal) {
          cv::line(trans.with(color_),
                   {(SIN)(x_min * xs + xd), (SIN)(y * ys + yd)},
                   {(SIN)(x_max * xs + xd), (SIN)(y * ys + yd)}, color, 1,
                   cv::LINE_AA);
        } else if (type_ == Vertical) {
          cv::line(trans.with(color_),
                   {(SIN)(y * xs + xd), (SIN)(y_min * ys + yd)},
                   {(SIN)(y * xs + xd), (SIN)(y_max * ys + yd)}, color, 1,
                   cv::LINE_AA);
        }
      }
    } break;
    case Range: {
      bool has_last = false;
      cv::Point last_a, last_b;
      for (const auto &e : entries_) {
        auto x = data_[e], y_a = data_[e + dims_], y_b = data_[e + dims_ + 1];
        if (dynamic_color_) {
          color = color2scalar(TRGBA::cos(data_[e + dims_ + 2]));
        }
        cv::Point point_a((SIN)(x * xs + xd), (SIN)(y_a * ys + yd));
        cv::Point point_b((SIN)(x * xs + xd), (SIN)(y_b * ys + yd));
        if (has_last) {
          cv::Point points[4] = {point_a, point_b, last_b, last_a};
          cv::fillConvexPoly(trans.with(color_), points, 4, color, cv::LINE_AA);
        } else {
          has_last = true;
        }
        last_a = point_a, last_b = point_b;
      }
    } break;
    case Circle: {
      for (const auto &e : entries_) {
        auto x = data_[e], y = data_[e + dims_], r = data_[e + dims_ + 1];
        if (dynamic_color_) {
          color = color2scalar(TRGBA::cos(data_[e + dims_ + 2]));
        }
        cv::Point point((SIN)(x * xs + xd), (SIN)(y * ys + yd));
        cv::circle(trans.with(color_), point, r, color, -1, cv::LINE_AA);
      }
    } break;
  }
}

Figure &Figure::clear() {
  series_.clear();
  return *this;
}

Figure &Figure::origin(bool x, bool y) {
  include_zero_x_ = x, include_zero_y_ = y;
  return *this;
}

Figure &Figure::square(bool square) {
  aspect_square_ = square;
  return *this;
}

Figure &Figure::border(SIN size) {
  border_size_ = size;
  return *this;
}

Figure &Figure::alpha(SIN alpha) {
  background_color_ = background_color_.alpha(alpha);
  axis_color_ = axis_color_.alpha(alpha);
  sub_axis_color_ = sub_axis_color_.alpha(alpha);
  text_color_ = text_color_.alpha(alpha);
  return *this;
}

Figure &Figure::gridSize(SIN size) {
  grid_size_ = size;
  return *this;
}

Figure &Figure::backgroundTRGBA(TRGBA color) {
  background_color_ = color;
  return *this;
}

Figure &Figure::axisTRGBA(TRGBA color) {
  axis_color_ = color;
  return *this;
}

Figure &Figure::subaxisTRGBA(TRGBA color) {
  sub_axis_color_ = color;
  return *this;
}

Figure &Figure::textTRGBA(TRGBA color) {
  text_color_ = color;
  return *this;
}

TRGBA Figure::backgroundTRGBA() { return background_color_; }

TRGBA Figure::axisTRGBA() { return axis_color_; }

TRGBA Figure::subaxisTRGBA() { return sub_axis_color_; }

TRGBA Figure::textTRGBA() { return text_color_; }

Series &Figure::series(const std::string &label) {
  for (auto &s : series_) {
    if (s.label() == label) {
      return s;
    }
  }
  Series s(label, Line, TRGBA::hash(label));
  series_.push_back(s);
  return series_.back();
}

void Figure::draw(void *b, FP4 x_min, FP4 x_max, FP4 y_min, FP4 y_max,
                  SIN n_max, SIN p_max) const {
  auto &buffer = *(cv::Mat *)b;
  Trans trans(b);

  // draw background and sub axis square
  cv::rectangle(trans.with(background_color_), {0, 0},
                {buffer.cols, buffer.rows}, color2scalar(background_color_), -1,
                cv::LINE_AA);
  cv::rectangle(trans.with(sub_axis_color_), {border_size_, border_size_},
                {buffer.cols - border_size_, buffer.rows - border_size_},
                color2scalar(sub_axis_color_), 1, cv::LINE_AA);

  // size of the plotting area
  auto w_plot = buffer.cols - 2 * border_size_;
  auto h_plot = buffer.rows - 2 * border_size_;

  // add padding inside graph (histograms get extra)
  if (p_max) {
    auto dx = p_max * (x_max - x_min) / w_plot;
    auto dy = p_max * (y_max - y_min) / h_plot;
    x_min -= dx;
    x_max += dx;
    y_min -= dy;
    y_max += dy;
  }

  // adjust value range if aspect ratio square
  if (aspect_square_) {
    if (h_plot * (x_max - x_min) < w_plot * (y_max - y_min)) {
      auto dx = w_plot * (y_max - y_min) / h_plot - (x_max - x_min);
      x_min -= dx / 2;
      x_max += dx / 2;
    } else if (w_plot * (y_max - y_min) < h_plot * (x_max - x_min)) {
      auto dy = h_plot * (x_max - x_min) / w_plot - (y_max - y_min);
      y_min -= dy / 2;
      y_max += dy / 2;
    }
  }

  // calc where to draw axis
  auto x_axis = std::max(x_min, std::min(x_max, 0.f));
  auto y_axis = std::max(y_min, std::min(y_max, 0.f));

  // calc sub axis grid size
  auto x_grid =
      (x_max != x_min ? value2snap((x_max - x_min) / floor(w_plot / grid_size_))
                      : 1);
  auto y_grid =
      (y_max != x_min ? value2snap((y_max - y_min) / floor(h_plot / grid_size_))
                      : 1);

  // calc affine transform value space to plot space
  auto xs = (x_max != x_min ? (buffer.cols - 2 * border_size_) / (x_max - x_min)
                            : 1.f);
  auto xd = border_size_ - x_min * xs;
  auto ys = (y_max != y_min ? (buffer.rows - 2 * border_size_) / (y_min - y_max)
                            : 1.f);
  auto yd = buffer.rows - y_min * ys - border_size_;

  // safe unit for showing points
  auto unit =
      std::max(1, ((SIN)std::min(buffer.cols, buffer.rows) - 2 * border_size_) /
                      n_max / 10);

  // draw sub axis
  for (auto x = ceil(x_min / x_grid) * x_grid; x <= x_max; x += x_grid) {
    cv::line(trans.with(sub_axis_color_), {(SIN)(x * xs + xd), border_size_},
             {(SIN)(x * xs + xd), buffer.rows - border_size_},
             color2scalar(sub_axis_color_), 1, cv::LINE_AA);
  }
  for (auto y = ceil(y_min / y_grid) * y_grid; y <= y_max; y += y_grid) {
    cv::line(trans.with(sub_axis_color_), {border_size_, (SIN)(y * ys + yd)},
             {buffer.cols - border_size_, (SIN)(y * ys + yd)},
             color2scalar(sub_axis_color_), 1, cv::LINE_AA);
  }
  if (std::abs(x_grid * xs) < 30) {
    x_grid *= std::ceil(30.f / std::abs(x_grid * xs));
  }
  for (auto x = std::ceil(x_min / x_grid) * x_grid; x <= x_max; x += x_grid) {
    std::ostringstream out;
    out << std::setprecision(4) << (x == 0 ? 0 : x);
    SIN baseline;
    cv::Size size =
        getTextSize(out.str(), cv::FONT_HERSHEY_SIMPLEX, 0.3f, 1.f, &baseline);
    cv::Point org(x * xs + xd - size.width / 2,
                  buffer.rows - border_size_ + 5 + size.height);
    cv::putText(trans.with(text_color_), out.str().c_str(), org,
                cv::FONT_HERSHEY_SIMPLEX, 0.3f, color2scalar(text_color_), 1.f);
  }
  if (std::abs(y_grid * ys) < 20) {
    y_grid *= std::ceil(20.f / std::abs(y_grid * ys));
  }
  for (auto y = std::ceil(y_min / y_grid) * y_grid; y <= y_max; y += y_grid) {
    std::ostringstream out;
    out << std::setprecision(4) << (y == 0 ? 0 : y);
    SIN baseline;
    cv::Size size =
        getTextSize(out.str(), cv::FONT_HERSHEY_SIMPLEX, 0.3f, 1.f, &baseline);
    cv::Point org(border_size_ - 5 - size.width, y * ys + yd + size.height / 2);
    cv::putText(trans.with(text_color_), out.str().c_str(), org,
                cv::FONT_HERSHEY_SIMPLEX, 0.3f, color2scalar(text_color_), 1.f);
  }

  // draw axis
  cv::line(trans.with(text_color_), {border_size_, (SIN)(y_axis * ys + yd)},
           {buffer.cols - border_size_, (SIN)(y_axis * ys + yd)},
           color2scalar(text_color_), 1, cv::LINE_AA);
  cv::line(trans.with(axis_color_), {(SIN)(x_axis * xs + xd), border_size_},
           {(SIN)(x_axis * xs + xd), buffer.rows - border_size_},
           color2scalar(axis_color_), 1, cv::LINE_AA);

  // draw plot
  auto index = 0;
  for (const auto &s : series_) {
    if (s.collides()) {
      index++;
    }
  }
  std::max((SIN)series_.size() - 1, 1);
  for (auto s = series_.rbegin(); s != series_.rend(); ++s) {
    if (s->collides()) {
      index--;
    }
    s->draw(&trans.with(s->color()), x_min, x_max, y_min, y_max, xs, xd, ys, yd,
            x_axis, y_axis, unit, (FP4)index / series_.size());
  }

  // draw label names
  index = 0;
  for (const auto &s : series_) {
    if (!s.legend()) {
      continue;
    }
    auto name = s.label();
    SIN baseline;
    cv::Size size =
        getTextSize(name, cv::FONT_HERSHEY_SIMPLEX, 0.4f, 1.f, &baseline);
    cv::Point org(buffer.cols - border_size_ - size.width - 17,
                  border_size_ + 15 * index + 15);
    auto shadow = true;
    cv::putText(trans.with(background_color_), name.c_str(),
                {org.x + (shadow ? 1 : 0), org.y + (shadow ? 1 : 0)},
                cv::FONT_HERSHEY_SIMPLEX, 0.4f, color2scalar(background_color_),
                (shadow ? 1.f : 2.f));
    cv::circle(trans.with(background_color_),
               {buffer.cols - border_size_ - 10 + 1, org.y - 3 + 1}, 3,
               color2scalar(background_color_), -1, cv::LINE_AA);
    cv::putText(trans.with(text_color_), name.c_str(), org,
                cv::FONT_HERSHEY_SIMPLEX, 0.4f, color2scalar(text_color_), 1.f);
    s.dot(&trans.with(s.color()), buffer.cols - border_size_ - 10, org.y - 3,
          3);
    index++;
  }
}

void Figure::show(bool flush) const {
  auto x_min = (include_zero_x_ ? 0.f : FLT_MAX);
  auto x_max = (include_zero_x_ ? 0.f : FLT_MIN);
  auto y_min = (include_zero_y_ ? 0.f : FLT_MAX);
  auto y_max = (include_zero_y_ ? 0.f : FLT_MIN);
  auto n_max = 0;
  auto p_max = grid_padding_;

  // find value bounds
  for (const auto &s : series_) {
    s.verifyParams();
    s.bounds(x_min, x_max, y_min, y_max, n_max, p_max);
  }

  if (n_max) {
    Rect rect(0, 0, 0, 0);
    auto &buffer = *(cv::Mat *)view_.buffer(rect);
    auto sub = buffer({rect.x, rect.y, rect.width, rect.height});
    draw(&sub, x_min, x_max, y_min, y_max, n_max, p_max);
    view_.finish();
    if (flush) {
      view_.flush();
    }
  }
}

Figure &figure(const std::string &name) {
  if (shared_figures_.count(name) == 0) {
    auto &view = Window::current().view(name.c_str());
    shared_figures_.insert(
        std::map<std::string, Figure>::value_type(name, Figure(view)));
  }
  return shared_figures_.at(name);
}

}  // namespace _
