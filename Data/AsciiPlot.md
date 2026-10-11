# ASCII scalar plot contract

TAsciiPlot<Width, Height> is a caller-owned, fixed-capacity POD for one scalar
series. It fulfills AGENT_PLAN.md Gap B without Image, Forms, OpenGL, Python,
C++ std containers, heap allocation, or a training algorithm.

Value-initialize the POD with {} (or call TAsciiPlotClear). Push finite FPC
samples in arrival order. A full ring keeps the latest Width samples; output
orders them oldest to newest, one column per sample, left-aligned while the
window is not full. One scalar series per plot lets a producer track reward,
policy_loss, value_loss, approx_kl, and entropy independently.

TAsciiPlotRange returns the visible-window min/max. Render uses that range,
places maxima at the top, minima at the bottom, and connects consecutive
points with deterministic Bresenham lines. Observations are '*', connecting
pixels '.', and blank pixels spaces. A constant series uses the center row;
width/height 1 are valid. Width is 1..160, height 1..60. The producer should
print the returned min/max and units alongside a chart when numeric scale
matters; the raster deliberately does not invent tick labels or dates.

Output is ASCII with '+'/'-' borders and '|' sides, one LF per row, and a
final NUL (not counted in written). Bytes = (Width+3)*(Height+2)+1 includes
the NUL. The API writes caller-owned storage, so a Crabs console or native
file adapter can emit the same chart without a GUI.

Nonfinite pushes (NaN/Inf), damaged ring metadata, or sample-counter overflow
fail without mutating the ring. Empty plots, nonfinite stored samples, null
outputs, or insufficient render capacity fail with written=0 and leave
output bytes unchanged. Scaling uses FPD intermediates to avoid overflow
when a window spans the finite FPC extremes. The chart is not a measurement
of learning or qualia: that interpretation belongs to actual recorded data.

Include AsciiPlot.hpp for POD/template declarations/definitions and import
AsciiPlot.hxx once in the translation unit for the finite-value helper.
Data/_Package.hxx provides the module implementation rollup. The independent
_Seams/Data/ entrypoint is opt-in and keeps shared root/IMUL files untouched.

Tests and demos use deterministic, explicitly synthetic seam inputs; they are
not training results. Redirect the demo's stdout to a file to verify console
and file output are identical.

Copyright AStarship <https://astarship.net>.
