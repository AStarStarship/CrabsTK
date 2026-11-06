// Copyright AStarship <https://astarship.net>.
namespace CTKTest {

inline const CHA* Release(const CHA* args) {
  return TTestTree<CTKAudioTest::Main, CTKCodeTest::Main,
    CTKDataTest::Main, CTKEnvTest::Main, CTKFormsTest::Main,
    CTKGUITest::Main, CTKImageTest::Main, CTKIMULTest::Main,
    CTKIPCTest::Main, CTKMathTest::Main, CTKPlotTest::Main,
    CTKTouchTest::Main, CTKWhoTest::Main, CTKWebTest::Main>(args);
}

}  //< namespace CTKTest
