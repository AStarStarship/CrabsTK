// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKGUIImageCanvasH
#define _CrabsTKGUIImageCanvasH
#include <_Config.h>
#include "../Exo/Image.h"
#include "../Exo/IDraw.h"
#include "VG.h"

namespace CTK {
namespace Exo {

/* The one Room an application owns.
An app that IS an iGeek Environment passes Environment::Room(); an app that
is not (a tool, a shell, a game) creates exactly one of these and shares it
with every canvas it opens. The Room is the whole memory model: its Floor is
the inline state and each Wall is a block carved out of it. There is no
second path — a canvas only draws into a Room Wall — so pixels, renderer
state and raster scratch rise and fall with the Room instead of with a
per-canvas heap. Apps never name ROM_D; this alias is the contract. */
using ARoom = _::TRoom<ROM_D>;

/* Backend state handed to the Exo::RenderParams vtable as userPtr. It must
  outlive the per-frame render callbacks and delete, so it lives with the
  canvas as a member rather than a constructor local. */
struct SHeadless {
  Exo::CImage* img;
  IUA* pixels;
  ISC w, h;
  ISC* indices;          //< polygon scratch, reserved in the canvas Wall.
  ISC index_count;
};

/* A vector canvas drawn entirely out of one Chinese Room Wall. The path is
built with BeginPath/MoveTo/LineTo/Circle then committed with Fill/Stroke.
There is no constructor that allocates outside a Room: a canvas without a
Room would be a second memory model, so Init(room) is the only way up. */
class AVCanvas {
 public:
  AVCanvas() = default;
  AVCanvas(const AVCanvas&) = delete;
  AVCanvas& operator=(const AVCanvas&) = delete;

  /* Attach a graphics Wall to the application's Room. The Room must outlive
  the canvas. Pixels, renderer state and raster scratch share this Wall.
  Failure leaves the canvas empty; Release detaches only its own Wall. */
  BOL Init(ARoom& room, ISC width, ISC height, Exo::VColor bg = Exo::IWhite);
  void Release();
  BOL Ready() const { return ctx_ != _NilP; }
  ARoom* Room() const { return room_; }

  ~AVCanvas();

  /* The rasterized canvas (headless backend). */
  Exo::CImage& Image() { return img_; }
  const Exo::CImage& Image() const { return img_; }

  /* One drawing pass: BeginFrame -> paths -> EndFrame (flushes to the image
  or the GL context). */
  void BeginFrame();
  void EndFrame();

  // Path API
  void BeginPath();
  void MoveTo(FPD x, FPD y);
  void LineTo(FPD x, FPD y);
  void Circle(FPD cx, FPD cy, FPD r);
  void ClosePath();
  /* Fill or stroke the current path with the given colour. */
  void Fill(Exo::VColor c);
  void Stroke(Exo::VColor c, FPD width);
  /* Fill the whole canvas with a colour (background). */
  void Clear(Exo::VColor c);

 private:
  BOL Init_(ISC width, ISC height, Exo::VColor bg);
  ARoom* room_ = _NilP;
  _::TWall<> wall_ = {}; //< registered with room_ when application-owned.
  Exo::RenderContext* ctx_ = _NilP;
  Exo::CImage img_ = {}; //< pixels borrowed from wall_.
  SHeadless headless_ = {};
  ISC width_ = 0, height_ = 0;
};

}  //< namespace Exo
}  //< namespace CTK
#endif  //< _CrabsTKGUIImageCanvasH
