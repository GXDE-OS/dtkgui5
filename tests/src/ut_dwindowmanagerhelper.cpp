#include "test.h"
#include <QTest>

#include "dwindowmanagerhelper.h"
#include "dforeignwindow.h"

DGUI_USE_NAMESPACE

class TDWindowMangerHelper : public DTest
{
protected:
    void SetUp();

    DWindowManagerHelper *wm_helper;
};

void TDWindowMangerHelper::SetUp()
{
    wm_helper = DWindowManagerHelper::instance();
}

TEST_F(TDWindowMangerHelper, testStaticFunction)
{
    if (qgetenv("QT_QPA_PLATFORM").contains("offscreen"))
        return;

    QWindow *w = new QWindow;
    w->create();

    enum { TestMotifFunction = DWindowManagerHelper::FUNC_RESIZE | DWindowManagerHelper::FUNC_MOVE, TestAllMotifFunction = DWindowManagerHelper::FUNC_ALL};
    enum { TestDecorations = DWindowManagerHelper::DECOR_BORDER | DWindowManagerHelper::DECOR_RESIZEH, TestAllDecorations = DWindowManagerHelper::DECOR_ALL };

    // 测试静态函数测试是否正常
    DWindowManagerHelper::setMotifFunctions(w, DWindowManagerHelper::MotifFunctions(TestMotifFunction));
    DWindowManagerHelper::MotifFunctions mFuncs = DWindowManagerHelper::getMotifFunctions(w);
    ASSERT_EQ(mFuncs, TestMotifFunction);

    mFuncs = DWindowManagerHelper::setMotifFunctions(w, DWindowManagerHelper::MotifFunctions(TestAllMotifFunction), true);
    ASSERT_EQ(mFuncs, TestAllMotifFunction);

    DWindowManagerHelper::setMotifDecorations(w, DWindowManagerHelper::MotifDecorations(TestDecorations));
    DWindowManagerHelper::MotifDecorations mDecos = DWindowManagerHelper::getMotifDecorations(w);
    ASSERT_EQ(mDecos, TestDecorations);

    mDecos = DWindowManagerHelper::setMotifDecorations(w, DWindowManagerHelper::MotifDecorations(TestAllDecorations), true);
    ASSERT_EQ(mDecos, TestAllDecorations);

    // 没有崩溃则测试成功
//    enum { TestWindowType =  DWindowManagerHelper::DesktopType | DWindowManagerHelper::MenuType };
//    DWindowManagerHelper::setWmWindowTypes(w, DWindowManagerHelper::WmWindowTypes(TestWindowType));
//    DWindowManagerHelper::setWmClassName(QByteArrayLiteral("TestWmClass"));
//    DWindowManagerHelper::popupSystemWindowMenu(w);
    delete w;
}

TEST_F(TDWindowMangerHelper, testFunctions)
{
    if (qgetenv("QT_QPA_PLATFORM").contains("offscreen"))
        return;

    ASSERT_TRUE(wm_helper->hasBlurWindow());
    ASSERT_TRUE(wm_helper->hasComposite());
    ASSERT_TRUE(wm_helper->hasNoTitlebar());
//    ASSERT_TRUE(wm_helper->hasWallpaperEffect());
    ASSERT_FALSE(wm_helper->windowManagerNameString().isEmpty());
    if (wm_helper->windowManagerNameString().contains(QStringLiteral("DeepinGala"))) {
        ASSERT_EQ(wm_helper->windowManagerName(), DWindowManagerHelper::DeepinWM);
    } else if (wm_helper->windowManagerNameString().contains(QStringLiteral("KWin"))) {
        ASSERT_EQ(wm_helper->windowManagerName(), DWindowManagerHelper::KWinWM);
    } else {
        ASSERT_EQ(wm_helper->windowManagerName(), DWindowManagerHelper::OtherWM);
    }

    ASSERT_FALSE(wm_helper->allWindowIdList().isEmpty());
    ASSERT_FALSE(wm_helper->currentWorkspaceWindowIdList().isEmpty());
    ASSERT_FALSE(wm_helper->currentWorkspaceWindows().isEmpty());
    ASSERT_TRUE(wm_helper->windowFromPoint(wm_helper->currentWorkspaceWindows().first()->position()));
}
