#ifndef CVPLOT_HIGHGUI_H
#define CVPLOT_HIGHGUI_H

#include <string>
#include <vector>

#include "t_window.h"

namespace _ {

SIN createTrackbar(const std::string &trackbarname, const std::string &winname,
                   SIN *value, SIN count, TrackbarCallback onChange = 0,
                   void *userdata = 0);
void destroyAllWindows();
void destroyWindow(const std::string &view);
SIN getMouseWheelDelta(SIN flags);
SIN getTrackbarPos(const std::string &trackbarname, const std::string &winname);
double getWindowProperty(const std::string &winname, SIN prop_id);
void imshow(const std::string &view, void *img);
void moveWindow(const std::string &view, SIN x, SIN y);
void namedWindow(const std::string &view, SIN flags = 0);
void resizeWindow(const std::string &view, SIN width, SIN height);
void resizeWindow(const std::string &view, const Size &size);
Rect selectROI(const std::string &windowName, void *img,
               bool showCrosshair = true, bool fromCenter = false);
Rect selectROI(void *img, bool showCrosshair = true, bool fromCenter = false);
void selectROIs(const std::string &windowName, void *img,
                std::vector<Rect> &boundingBoxes, bool showCrosshair = true,
                bool fromCenter = false);
void setMouseCallback(const std::string &view, MouseCallback onMouse,
                      void *userdata = 0);
void setTrackbarMax(const std::string &trackbarname, const std::string &winname,
                    SIN maxval);
void setTrackbarMin(const std::string &trackbarname, const std::string &winname,
                    SIN minval);
void setTrackbarPos(const std::string &trackbarname, const std::string &winname,
                    SIN pos);
void setWindowProperty(const std::string &winname, SIN prop_id,
                       double prop_value);
void setWindowTitle(const std::string &view, const std::string &title);
SIN startWindowThread();
SIN waitKey(SIN delay = 0);
SIN waitKeyEx(SIN delay = 0);

}  // namespace _

#endif  // CVPLOT_HIGHGUI_H
