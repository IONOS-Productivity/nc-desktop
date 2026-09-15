#include "sesFileIconProvider.h"
#include "theme.h"
#include "whitelabeltheme.h"

#include <QFileIconProvider>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>

#ifdef Q_OS_WIN
#include <QDir>
#include <QScopeGuard>
#include <shlobj.h>
#include <shobjidl.h>
#endif

namespace {

#ifdef Q_OS_WIN
// QFileIconProvider's Windows backend fetches the icon via SHGetFileInfo(), which (a) is capped
// at the shell's small cached icon sizes - so requesting a larger size than what's cached makes
// Qt upscale a small bitmap, producing a blurry result - and (b) composites any active Explorer
// icon overlay handler into the result, including our own sync/share-status overlay, which has
// no business appearing on a plain file icon in the Share dialog header (SES-611).
//
// IShellItemImageFactory::GetImage() with SIIGBF_ICONONLY is the documented way around both:
// SIIGBF_ICONONLY explicitly excludes overlays, and SIIGBF_BIGGERSIZEOK allows requesting a size
// larger than the shell's cache without an upscale. GetImage() returns a top-down, 32bpp
// premultiplied-alpha DIB section, so its bits can be wrapped directly into a QImage.
//
// NOTE: implemented against the documented Win32/COM API contract, but not yet verified on an
// actual Windows machine (this session has no Windows toolchain) - re-check on first Windows test,
// particularly the DIB row order/alpha format and the COM init/uninit balance.
QIcon windowsShellFileIcon(const QString &filePath, int pixelSize)
{
    const auto comInitResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(comInitResult) && comInitResult != RPC_E_CHANGED_MODE) {
        return {};
    }
    const auto weInitializedCom = comInitResult == S_OK;
    const auto comGuard = qScopeGuard([weInitializedCom]() {
        if (weInitializedCom) {
            CoUninitialize();
        }
    });

    IShellItem *shellItem = nullptr;
    const auto nativePath = QDir::toNativeSeparators(filePath);
    if (FAILED(SHCreateItemFromParsingName(reinterpret_cast<const wchar_t *>(nativePath.utf16()), nullptr, IID_PPV_ARGS(&shellItem)))
        || !shellItem) {
        return {};
    }
    const auto shellItemGuard = qScopeGuard([shellItem]() { shellItem->Release(); });

    IShellItemImageFactory *imageFactory = nullptr;
    if (FAILED(shellItem->QueryInterface(IID_PPV_ARGS(&imageFactory))) || !imageFactory) {
        return {};
    }
    const auto imageFactoryGuard = qScopeGuard([imageFactory]() { imageFactory->Release(); });

    HBITMAP hBitmap = nullptr;
    const SIZE size{pixelSize, pixelSize};
    if (FAILED(imageFactory->GetImage(size, SIIGBF_ICONONLY | SIIGBF_BIGGERSIZEOK, &hBitmap)) || !hBitmap) {
        return {};
    }
    const auto bitmapGuard = qScopeGuard([hBitmap]() { DeleteObject(hBitmap); });

    BITMAP bitmap;
    if (GetObject(hBitmap, sizeof(BITMAP), &bitmap) == 0 || !bitmap.bmBits) {
        return {};
    }

    const QImage image(reinterpret_cast<const uchar *>(bitmap.bmBits), bitmap.bmWidth, bitmap.bmHeight, bitmap.bmWidthBytes,
        QImage::Format_ARGB32_Premultiplied);

    // deep copy: the backing bmBits memory goes away once hBitmap is deleted above
    return QIcon(QPixmap::fromImage(image.copy()));
}
#endif
// Theme::createColorAwareIcon() just inverts the source SVG's raw RGB values for dark
// mode; ses-folderIcon.svg is filled with #2F2F70, which inverts to an undesigned
// washed-out khaki/beige instead of an actual dark-mode color (same issue fixed for the
// wizard's folder/avatar/sync icons in owncloudadvancedsetuppage.cpp). Render once and
// re-tint with a real themed color via SourceIn-compositing instead.
//
// Rendered via QSvgRenderer straight into an image of the target size - same approach
// Theme::createColorAwareIcon() uses - rather than QIcon(path).pixmap(size), which for
// this non-square source SVG (58x52) picks/scales an already-rasterized pixmap and comes
// out the wrong size.
QIcon tintedThemeIcon(const QString &path, const QColor &color, const QSize &size = QSize(64, 64))
{
    QSvgRenderer renderer(path);
    QImage img(size, QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter svgPainter(&img);
    renderer.render(&svgPainter);
    svgPainter.end();

    QPixmap tinted(size);
    tinted.fill(Qt::transparent);
    QPainter painter(&tinted);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(0, 0, img);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(tinted.rect(), color);
    painter.end();

    return QIcon(tinted);
}
}

QIcon SesFileIconProvider::icon(const QFileInfo &info) const
{
    QFileIconProvider provider;

    if (info.isDir())
    {
        return tintedThemeIcon(OCC::WLTheme.folderIcon("qtwidget"), QColor(OCC::WLTheme.iconDarkColor()));
    }

    if (info.suffix().isEmpty())
    {
        return QIcon(":/client/theme/ses/ses-file.svg");
    }

#ifdef Q_OS_WIN
    if (const auto shellIcon = windowsShellFileIcon(info.absoluteFilePath(), 256); !shellIcon.isNull())
    {
        return shellIcon;
    }
#endif

    return provider.icon(info);
};