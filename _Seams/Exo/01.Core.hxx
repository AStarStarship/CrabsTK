// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
#if _Seam == _CrabsTKGUICore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
#include "../../Exo/Image.hxx"
#include "../../Exo/IDraw.hxx"
#include "../../Exo/VG.hxx"
#include "../../Exo/Canvas.hxx"
namespace CTKGUITest {
using namespace ::_;
using namespace ::CTK;
/* Exo seam core tests — the headless vector pipeline.

Exo owns the vector path: our VG core (the de-vendored VG.hxx) builds and
tessellates paths with ASCIICrabs math, and AVCanvas rasterises them in software
into an RGBA8 buffer. No GL context is involved, so this runs on a display-less
build box. The windowed demo lives in 02.Release.hxx.

Every assertion reads real pixels: a path is only correct if the raster agrees. */

/* One 8-bit channel of a pixel in an RGBA8 buffer. */
inline IUB ChannelAt(const Exo::CImage& image, ISC x, ISC y, ISC channel) {
  return image.data[((IUN)y * (IUN)image.width + (IUN)x) * 4 + (IUN)channel];
}

/* Bytes a normalised channel value represents, rounded. */
inline IUB ByteOf(FPC value) { return IUB(value * 255.0F + 0.5F); }

inline BOL SameByte(IUB actual, IUB expected) {
  return actual == expected || actual + 1 == expected || actual == expected + 1;
}

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKGUICore
  _ATestBegin;
  _DROut(Headingf("Testing Exo vector core (headless)..."));
  (void)args;

  // The colour codes carry bytes, not signed characters
  {
    Exo::ColorFPC color = Exo::ExoRGBA(10, 20, 30, 255);
    _DCheck(SameByte(ByteOf(color.r), 10), "ExoRGBA keeps red channel");
    _DCheck(SameByte(ByteOf(color.g), 20), "ExoRGBA keeps green channel");
    _DCheck(SameByte(ByteOf(color.b), 30), "ExoRGBA keeps blue channel");
    _DCheck(SameByte(ByteOf(color.a), 255), "ExoRGBA keeps alpha at 255");

    Exo::ColorFPC opaque = Exo::ExoRGB(255, 128, 0);
    _DCheck(SameByte(ByteOf(opaque.a), 255), "ExoRGB is opaque");
    _DCheck(SameByte(ByteOf(opaque.g), 128), "ExoRGB passes mid green");

    Exo::ColorFPC mid = Exo::ExoLerpRGBA(Exo::ExoRGBA(0, 0, 0, 255),
                                        Exo::ExoRGBA(255, 255, 255, 255), 0.5F);
    _DCheck(mid.r > 0.4F && mid.r < 0.6F, "ExoLerpRGBA halves black to white");
  }

  // HSL: alpha is IUA, not CHR
  /* The port typed ExoHSLA's alpha as CHR, so the 255 passed by ExoHSL arrived
     as -1 and every HSL colour came out with alpha -0.0039. */
  {
    Exo::ColorFPC c = Exo::ExoHSL(0.0F, 1.0F, 0.5F);
    _DCheck(c.a == 1.0F, "ExoHSL default alpha is opaque");
    _DCheck(c.r > 0.99F && c.g < 0.01F && c.b < 0.01F, "hue 0 is red");
  }

  // --- the canvas is a headless RGBA8 target ------------------------------
  {
    GUI::AVCanvas canvas(32, 32, Exo::IWhite);
    _DCheck(canvas.Image().width == 32 && canvas.Image().height == 32,
            "canvas is 32x32");
    _DCheck(canvas.Image().format == Exo::PixelFormat::RGBA8,
            "canvas is RGBA8");
    _DCheckPointer(canvas.Image().data, "canvas owns its pixels");
  }

  // A frame with nothing in it holds the background
  {
    GUI::AVCanvas canvas(8, 8, Exo::IBlue);
    canvas.BeginFrame();
    canvas.EndFrame();
    const Exo::CImage& image = canvas.Image();
    _DCheck(ChannelAt(image, 0, 0, 2) == 255 && ChannelAt(image, 0, 0, 0) == 0,
            "empty frame keeps the background");
    _DCheck(ChannelAt(image, 7, 7, 2) == 255, "background reaches the far corner");
  }

  // Clear paints every pixel
  {
    GUI::AVCanvas canvas(16, 16, Exo::IWhite);
    canvas.BeginFrame();
    canvas.Clear(Exo::IRed);
    canvas.EndFrame();
    const Exo::CImage& image = canvas.Image();
    _DCheck(ChannelAt(image, 0, 0, 0) == 255 && ChannelAt(image, 0, 0, 1) == 0,
            "Clear paints the origin");
    _DCheck(ChannelAt(image, 15, 15, 0) == 255 && ChannelAt(image, 15, 15, 1) == 0,
            "Clear paints the far corner");
  }

  // A filled quad paints its interior and nothing outside
  {
    GUI::AVCanvas canvas(32, 32, Exo::IWhite);
    canvas.BeginFrame();
    canvas.BeginPath();
    canvas.MoveTo(8, 8);
    canvas.LineTo(24, 8);
    canvas.LineTo(24, 24);
    canvas.LineTo(8, 24);
    canvas.ClosePath();
    canvas.Fill(Exo::IRed);
    canvas.EndFrame();
    const Exo::CImage& image = canvas.Image();

    const IUB red = ChannelAt(image, 16, 16, 0);
    const IUB green = ChannelAt(image, 16, 16, 1);
    const IUB blue = ChannelAt(image, 16, 16, 2);
    _DCheck(red > 200 && green < 100 && blue < 100,
            "the quad interior is red");

    _DCheck(ChannelAt(image, 0, 0, 0) == 255 && ChannelAt(image, 0, 0, 1) == 255,
            "the fill leaves the origin untouched");
    _DCheck(ChannelAt(image, 31, 31, 1) == 255, "the fill leaves the far corner untouched");

    // The boundary pixel is neither fully painted nor fully background.
    ISC partial = 0;
    for (ISC y = 7; y <= 9; ++y) {
      const IUB value = ChannelAt(image, 8, y, 1);
      if (value > 0 && value < 255) ++partial;
    }
    _DCheck(partial > 0, "the quad edge is antialiased");
  }

  // A circle paints its disc, not its bounding box
  {
    GUI::AVCanvas canvas(32, 32, Exo::IWhite);
    canvas.BeginFrame();
    canvas.BeginPath();
    canvas.Circle(16, 16, 10);
    canvas.Fill(Exo::IGreen);
    canvas.EndFrame();
    const Exo::CImage& image = canvas.Image();

    _DCheck(ChannelAt(image, 16, 16, 1) > 200, "the circle centre is green");
    _DCheck(ChannelAt(image, 0, 0, 1) == 255 && ChannelAt(image, 0, 0, 0) == 255,
            "the circle leaves the corner white");
    _DCheck(ChannelAt(image, 28, 16, 1) == 255,
            "the circle stops before the bounding box edge");
  }

  // A stroke follows the segment----------------
  {
    GUI::AVCanvas canvas(32, 32, Exo::IWhite);
    canvas.BeginFrame();
    canvas.BeginPath();
    canvas.MoveTo(4, 16);
    canvas.LineTo(28, 16);
    canvas.Stroke(Exo::IBlack, 3.0F);
    canvas.EndFrame();
    const Exo::CImage& image = canvas.Image();

    _DCheck(ChannelAt(image, 16, 16, 1) < 80, "the stroke paints its midpoint");
    _DCheck(ChannelAt(image, 16, 2, 1) == 255, "the stroke stays off the top");
    _DCheck(ChannelAt(image, 16, 30, 1) == 255, "the stroke stays off the bottom");
  }

  // Geometry outside the canvas is clipped, never fatal
  {
    GUI::AVCanvas canvas(16, 16, Exo::IWhite);
    canvas.BeginFrame();
    canvas.BeginPath();
    canvas.MoveTo(-8, -8);
    canvas.LineTo(8, -8);
    canvas.LineTo(8, 8);
    canvas.LineTo(-8, 8);
    canvas.ClosePath();
    canvas.Fill(Exo::IRed);
    canvas.BeginPath();
    canvas.MoveTo(10, 10);
    canvas.LineTo(40, 10);
    canvas.LineTo(40, 40);
    canvas.LineTo(10, 40);
    canvas.ClosePath();
    canvas.Fill(Exo::IBlue);
    canvas.EndFrame();
    const Exo::CImage& image = canvas.Image();

    _DCheck(ChannelAt(image, 4, 4, 0) > 200 && ChannelAt(image, 4, 4, 1) < 100,
            "the overhanging quad still paints its visible part");
    _DCheck(ChannelAt(image, 15, 15, 2) > 200,
            "the quad past the far edge paints the corner");
  }

  _DROut("\n");
#endif
  return _NilP;
}

}  //< namespace CTKGUITest
