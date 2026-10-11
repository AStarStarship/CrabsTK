/*
    /kabuki/toolkit/gui/imageview.h -- Widget used to display images.

    The image view widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once
#include <pch.h>

#include "t_glutil.h"
#include "t_screen.h"
#include "t_theme.h"
#include "t_widget.h"
#include "t_window.h"

#include <functional>

namespace _ {

/* Widget used to display images. */
class SDK ImageView : public Widget {
 public:
  ImageView(Widget* parent, GLuint imageID)
      : Widget(parent),
        mImageID(imageID),
        mScale(1.0f),
        mOffset(Vector2f::Zero()),
        mFixedScale(false),
        mFixedOffset(false),
        mPixelInfoCallback(nullptr) {
    updateImageParameters();
    mShader.init("ImageViewShader", defaultImageViewVertexShader,
                 defaultImageViewFragmentShader);

    MatrixXu indices(3, 2);
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 3, 1;

    MatrixXf vertices(2, 4);
    vertices.col(0) << 0, 0;
    vertices.col(1) << 1, 0;
    vertices.col(2) << 0, 1;
    vertices.col(3) << 1, 1;

    mShader.bind();
    mShader.uploadIndices(indices);
    mShader.uploadAttrib("vertex", vertices);
  }
  ~ImageView() override { mShader.free(); }

  void bindImage(GLuint imageId) {
    mImageID = imageId;
    updateImageParameters();
    fit();
  }

  GLShader& imageShader() { return mShader; }

  Vector2f positionF() const { return mPos.cast<float>(); }
  Vector2f sizeF() const { return mSize.cast<float>(); }

  const Vector2i& imageSize() const { return mImageSize; }
  Vector2i scaledImageSize() const {
    return (mScale * mImageSize.cast<float>()).cast<int>();
  }
  Vector2f imageSizeF() const { return mImageSize.cast<float>(); }
  Vector2f scaledImageSizeF() const {
    return (mScale * mImageSize.cast<float>());
  }

  const Vector2f& offset() const { return mOffset; }

  void setOffset(const Vector2f& offset) { mOffset = offset; }

  float scale() const { return mScale; }

  void setScale(float scale) { mScale = scale > 0.01f ? scale : 0.01f; }

  bool fixedOffset() const { return mFixedOffset; }

  void setFixedOffset(bool fixedOffset) { mFixedOffset = fixedOffset; }

  bool fixedScale() const { return mFixedScale; }

  void setFixedScale(bool fixedScale) { mFixedScale = fixedScale; }

  float zoomSensitivity() const { return mZoomSensitivity; }

  void setZoomSensitivity(float zoomSensitivity) {
    mZoomSensitivity = zoomSensitivity;
  }

  float gridThreshold() const { return mGridThreshold; }

  void setGridThreshold(float gridThreshold) { mGridThreshold = gridThreshold; }

  float pixelInfoThreshold() const { return mPixelInfoThreshold; }

  void setPixelInfoThreshold(float pixelInfoThreshold) {
    mPixelInfoThreshold = pixelInfoThreshold;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  void setPixelInfoCallback(const std::function<std::pair<std::string, Color>(
                                /*unused*/ const Vector2i&)>& callback) {
    mPixelInfoCallback = callback;
  }
  const std::function<std::pair<std::string, Color>(const Vector2i&)>&
  pixelInfoCallback() const {
    return mPixelInfoCallback;
  }
#endif  // DOXYGEN_SHOULD_SKIP_THIS

  void setFontScaleFactor(float fontScaleFactor) {
    mFontScaleFactor = fontScaleFactor;
  }
  float fontScaleFactor() const { return mFontScaleFactor; }

  // Image transformation functions.

  // Calculates the image coordinates of the given pixel position on the widget.
  Vector2f imageCoordinateAt(const Vector2f& position) const const {
    auto imagePosition = position - mOffset;
    return imagePosition / mScale;
  }

  /*
   * Calculates the image coordinates of the given pixel position on the widget.
   * If the position provided corresponds to a coordinate outside the range of
   * the image, the coordinates are clamped to edges of the image.
   */
  Vector2f clampedImageCoordinateAt(const Vector2f& position) const {
    auto imageCoordinate = imageCoordinateAt(position);
    return imageCoordinate.cwiseMax(Vector2f::Zero()).cwiseMin(imageSizeF());
  }

  // Calculates the position inside the widget for the given image coordinate.
  Vector2f positionForCoordinate(const Vector2f& imageCoordinate) const {
    return mScale * imageCoordinate + mOffset;
  }

  /*
   * Modifies the internal state of the image viewer widget so that the pixel at
   * the provided position on the widget has the specified image coordinate.
   * Also clamps the values of offset to the sides of the widget.
   */
  void setImageCoordinateAt(const Vector2f& position,
                            const Vector2f& imageCoordinate) {
    // Calculate where the new offset must be in order to satisfy the image
    // position equation. Round the floating point values to balance out the
    // floating point to integer conversions.
    mOffset = position - (imageCoordinate * mScale);

    // Clamp offset so that the image remains near the screen.
    mOffset = mOffset.cwiseMin(sizeF()).cwiseMax(-scaledImageSizeF());
  }

  // Centers the image without affecting the scaling factor.
  void center() { mOffset = (sizeF() - scaledImageSizeF()) / 2; }

  // Centers and scales the image so that it fits inside the widgets.
  void fit() {
    // Calculate the appropriate scaling factor.
    mScale = (sizeF().cwiseQuotient(imageSizeF())).minCoeff();
    center();
  }

  // Set the scale while keeping the image centered
  void setScaleCentered(float scale) {
    auto centerPosition = sizeF() / 2;
    auto p = imageCoordinateAt(centerPosition);
    mScale = scale;
    setImageCoordinateAt(centerPosition, p);
  }

  // Moves the offset by the specified amount. Does bound checking.
  void moveOffset(const Vector2f& delta) {
    // Apply the delta to the offset.
    mOffset += delta;

    // Prevent the image from going out of bounds.
    auto scaledSize = scaledImageSizeF();
    if (mOffset.x() + scaledSize.x() < 0) mOffset.x() = -scaledSize.x();
    if (mOffset.x() > sizeF().x()) mOffset.x() = sizeF().x();
    if (mOffset.y() + scaledSize.y() < 0) mOffset.y() = -scaledSize.y();
    if (mOffset.y() > sizeF().y()) mOffset.y() = sizeF().y();
  }

  /*
   * Changes the scale factor by the provided amount modified by the zoom
   * sensitivity member variable. The scaling occurs such that the image
   * coordinate under the focused position remains in the same position before
   * and after the scaling.
   */
  void zoom(int amount, const Vector2f& focusPosition) {
    auto focusedCoordinate = imageCoordinateAt(focusPosition);
    float scaleFactor = std::pow(mZoomSensitivity, amount);
    mScale = std::max(0.01f, scaleFactor * mScale);
    setImageCoordinateAt(focusPosition, focusedCoordinate);
  }

  bool keyboardEvent(int key, int scancode, int action,
                     int modifiers) override {
    if (action) {
      switch (key) {
        case GLFW_KEY_LEFT:
          if (!mFixedOffset) {
            if (GLFW_MOD_CONTROL & modifiers)
              moveOffset(Vector2f(30, 0));
            else
              moveOffset(Vector2f(10, 0));
            return true;
          }
          break;
        case GLFW_KEY_RIGHT:
          if (!mFixedOffset) {
            if (GLFW_MOD_CONTROL & modifiers)
              moveOffset(Vector2f(-30, 0));
            else
              moveOffset(Vector2f(-10, 0));
            return true;
          }
          break;
        case GLFW_KEY_DOWN:
          if (!mFixedOffset) {
            if (GLFW_MOD_CONTROL & modifiers)
              moveOffset(Vector2f(0, -30));
            else
              moveOffset(Vector2f(0, -10));
            return true;
          }
          break;
        case GLFW_KEY_UP:
          if (!mFixedOffset) {
            if (GLFW_MOD_CONTROL & modifiers)
              moveOffset(Vector2f(0, 30));
            else
              moveOffset(Vector2f(0, 10));
            return true;
          }
          break;
      }
    }
    return false;
  }

  bool keyboardCharacterEvent(unsigned int codepoint) override {
    switch (codepoint) {
      case '-':
        if (!mFixedScale) {
          zoom(-1, sizeF() / 2);
          return true;
        }
        break;
      case '+':
        if (!mFixedScale) {
          zoom(1, sizeF() / 2);
          return true;
        }
        break;
      case 'c':
        if (!mFixedOffset) {
          center();
          return true;
        }
        break;
      case 'f':
        if (!mFixedOffset && !mFixedScale) {
          fit();
          return true;
        }
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if (!mFixedScale) {
          setScaleCentered(1 << (codepoint - '1'));
          return true;
        }
        break;
      default:
        return false;
    }
    return false;
  }

  bool mouseDragEvent(const Vector2i& p, const Vector2i& rel, int button,
                      int modifiers) override {
    if ((button & (1 << GLFW_MOUSE_BUTTON_LEFT)) != 0 && !mFixedOffset) {
      setImageCoordinateAt((p + rel).cast<float>(),
                           imageCoordinateAt(p.cast<float>()));
      return true;
    }
    return false;
  }

  bool scrollEvent(const Vector2i& p, const Vector2f& rel) override {
    if (mFixedScale) return false;
    float v = rel.y();
    if (std::abs(v) < 1) v = std::copysign(1.f, v);
    zoom(v, (p - position()).cast<float>());
    return true;
  }

  // Function indicating whether the grid is currently visible.
  bool gridVisible() const {
    return (mGridThreshold != -1) && (mScale > mGridThreshold);
  }

  // Function indicating whether the pixel information is currently visible.
  bool pixelInfoVisible() const {
    return mPixelInfoCallback && (mPixelInfoThreshold != -1) &&
           (mScale > mPixelInfoThreshold);
  }

  // Function indicating whether any of the overlays are visible.
  bool helpersVisible() const { return gridVisible() || pixelInfoVisible(); }

  Vector2i preferredSize(NVGcontext* ctx) const override { return mImageSize; }

  void performLayout(NVGcontext* ctx) override {
    Widget::performLayout(ctx);
    center();
  }

  void draw(NVGcontext* ctx) override {
    Widget::draw(ctx);
    nvgEndFrame(ctx);  // Flush the NanoVG draw stack, not necessary to call
                       // nvgBeginFrame afterwards.

    drawImageBorder(ctx);

    // Calculate several variables that need to be send to OpenGL in order for
    // the image to be properly displayed inside the widget.
    const Screen* screen =
        dynamic_cast<const Screen*>(this->window()->parent());
    DASSERT(screen);
    Vector2f screenSize = screen->size().cast<float>();
    Vector2f scaleFactor = mScale * imageSizeF().cwiseQuotient(screenSize);
    Vector2f positionInScreen = absolutePosition().cast<float>();
    Vector2f positionAfterOffset = positionInScreen + mOffset;
    Vector2f imagePosition = positionAfterOffset.cwiseQuotient(screenSize);
    glEnable(GL_SCISSOR_TEST);
    float r = screen->pixelRatio();
    glScissor(positionInScreen.x() * r,
              (screenSize.y() - positionInScreen.y() - size().y()) * r,
              size().x() * r, size().y() * r);
    mShader.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageID);
    mShader.setUniform("image", 0);
    mShader.setUniform("scaleFactor", scaleFactor);
    mShader.setUniform("position", imagePosition);
    mShader.drawIndexed(GL_TRIANGLES, 0, 2);
    glDisable(GL_SCISSOR_TEST);

    if (helpersVisible()) drawHelpers(ctx);

    drawWidgetBorder(ctx);
  }

 private:
  // Helper image methods.
  void updateImageParameters() {
    // Query the width of the OpenGL texture.
    glBindTexture(GL_TEXTURE_2D, mImageID);
    GLint w, h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    mImageSize = Vector2i(w, h);
  }

  // Helper drawing methods.
  void drawWidgetBorder(NVGcontext* ctx) const {
    nvgBeginPath(ctx);
    nvgStrokeWidth(ctx, 1);
    nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 0.5f, mSize.x() - 1,
                   mSize.y() - 1, 0);
    nvgStrokeColor(ctx, mTheme->mWindowPopup);
    nvgStroke(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 0.5f, mSize.x() - 1,
                   mSize.y() - 1, mTheme->mButtonCornerRadius);
    nvgStrokeColor(ctx, mTheme->mBorderDark);
    nvgStroke(ctx);
  }

  void drawImageBorder(NVGcontext* ctx) const {
    nvgSave(ctx);
    nvgBeginPath(ctx);
    nvgScissor(ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y());
    nvgStrokeWidth(ctx, 1.0f);
    Vector2i borderPosition = mPos + mOffset.cast<int>();
    Vector2i borderSize = scaledImageSizeF().cast<int>();
    nvgRect(ctx, borderPosition.x() - 0.5f, borderPosition.y() - 0.5f,
            borderSize.x() + 1, borderSize.y() + 1);
    nvgStrokeColor(ctx, Color(1.0f, 1.0f, 1.0f, 1.0f));
    nvgStroke(ctx);
    nvgResetScissor(ctx);
    nvgRestore(ctx);
  }

  void drawHelpers(NVGcontext* ctx) const {
    // We need to apply mPos after the transformation to account for the
    // position of the widget relative to the parent.
    Vector2f upperLeftCorner =
        positionForCoordinate(Vector2f::Zero()) + positionF();
    Vector2f lowerRightCorner =
        positionForCoordinate(imageSizeF()) + positionF();
    if (gridVisible())
      drawPixelGrid(ctx, upperLeftCorner, lowerRightCorner, mScale);
    if (pixelInfoVisible()) drawPixelInfo(ctx, mScale);
  }

  static void drawPixelGrid(NVGcontext* ctx, const Vector2f& upperLeftCorner,
                            const Vector2f& lowerRightCorner, float stride) {
    nvgBeginPath(ctx);

    // Draw the vertical grid lines
    float currentX = upperLeftCorner.x();
    while (currentX <= lowerRightCorner.x()) {
      nvgMoveTo(ctx, std::round(currentX), std::round(upperLeftCorner.y()));
      nvgLineTo(ctx, std::round(currentX), std::round(lowerRightCorner.y()));
      currentX += stride;
    }

    // Draw the horizontal grid lines
    float currentY = upperLeftCorner.y();
    while (currentY <= lowerRightCorner.y()) {
      nvgMoveTo(ctx, std::round(upperLeftCorner.x()), std::round(currentY));
      nvgLineTo(ctx, std::round(lowerRightCorner.x()), std::round(currentY));
      currentY += stride;
    }

    nvgStrokeWidth(ctx, 1.0f);
    nvgStrokeColor(ctx, Color(1.0f, 1.0f, 1.0f, 0.2f));
    nvgStroke(ctx);
  }

  void drawPixelInfo(NVGcontext* ctx, float stride) const {
    // Extract the image coordinates at the two corners of the widget.
    Vector2i topLeft = clampedImageCoordinateAt(Vector2f::Zero())
                           .unaryExpr([](float x) { return std::floor(x); })
                           .cast<int>();

    Vector2i bottomRight = clampedImageCoordinateAt(sizeF())
                               .unaryExpr([](float x) { return std::ceil(x); })
                               .cast<int>();

    // Extract the positions for where to draw the text.
    Vector2f currentCellPosition =
        (positionF() + positionForCoordinate(topLeft.cast<float>()));

    float xInitialPosition = currentCellPosition.x();
    int xInitialIndex = topLeft.x();

    // Properly scale the pixel information for the given stride.
    auto fontSize = stride * mFontScaleFactor;
    static constexpr float maxFontSize = 30.0f;
    fontSize = fontSize > maxFontSize ? maxFontSize : fontSize;
    nvgBeginPath(ctx);
    nvgFontSize(ctx, fontSize);
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgFontFace(ctx, "sans");
    while (topLeft.y() != bottomRight.y()) {
      while (topLeft.x() != bottomRight.x()) {
        writePixelInfo(ctx, currentCellPosition, topLeft, stride, fontSize);
        currentCellPosition.x() += stride;
        ++topLeft.x();
      }
      currentCellPosition.x() = xInitialPosition;
      currentCellPosition.y() += stride;
      ++topLeft.y();
      topLeft.x() = xInitialIndex;
    }
  }

  void writePixelInfo(NVGcontext* ctx, const Vector2f& cellPosition,
                      const Vector2i& pixel, float stride,
                      float fontSize) const {
    auto pixelData = mPixelInfoCallback(pixel);
    auto pixelDataRows = tokenize(pixelData.first);

    // If no data is provided for this pixel then simply return.
    if (pixelDataRows.empty()) return;

    nvgFillColor(ctx, pixelData.second);
    float yOffset = (stride - fontSize * pixelDataRows.size()) / 2;
    for (size_t i = 0; i != pixelDataRows.size(); ++i) {
      nvgText(ctx, cellPosition.x() + stride / 2, cellPosition.y() + yOffset,
              pixelDataRows[i].data(), nullptr);
      yOffset += fontSize;
    }
  }

  // Image parameters.
  GLShader mShader;
  GLuint mImageID;
  Vector2i mImageSize;

  // Image display parameters.
  float mScale;
  Vector2f mOffset;
  bool mFixedScale;
  bool mFixedOffset;

  // Fine-tuning parameters.
  float mZoomSensitivity = 1.1f;

  // Image info parameters.
  float mGridThreshold = -1;
  float mPixelInfoThreshold = -1;

  // Image pixel data display members.
  std::function<std::pair<std::string, Color>(const Vector2i&)>
      mPixelInfoCallback;
  float mFontScaleFactor = 0.2f;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}  // namespace _
