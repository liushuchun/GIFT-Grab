#include "deck_link_bgra_video_frame.h"
#include <cstring>

namespace gg
{

DeckLinkBGRAVideoFrame::DeckLinkBGRAVideoFrame(
    size_t width, size_t height,
    LPVOID pixel_buffer, BMDFrameFlags frame_flags
)
    : _width(width)
    , _height(height)
    , _pixel_buffer(pixel_buffer)
    , _flags(frame_flags)
{
    // just to avoid calling a method for each
    // data copy:
    _pixel_buffer_length = 4 * width * height;
}

DeckLinkBGRAVideoFrame::~DeckLinkBGRAVideoFrame()
{
    _width = 0;
    _height = 0;
    _pixel_buffer = nullptr;
    _pixel_buffer_length = 0;
}

long STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::GetWidth(void)
{
    return _width;
}

long STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::GetHeight(void)
{
    return _height;
}

long STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::GetRowBytes(void)
{
    return 4 * _width;
}

HRESULT STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::GetBytes(void **buffer)
{
    std::memcpy(
        reinterpret_cast<void *>(buffer),
        _pixel_buffer,
        _pixel_buffer_length
    );
    return S_OK;
}

BMDFrameFlags STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::GetFlags(void)
{
    return _flags;
}

BMDPixelFormat STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::GetPixelFormat(void)
{
    return bmdFormat8BitBGRA;
}

HRESULT STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::QueryInterface(
    REFIID iid, LPVOID *ppv
)
{
    // TODO
}

ULONG STDMETHODCALLTYPE DeckLinkBGRAVideoFrame::AddRef()
{
    __sync_add_and_fetch(&_ref_count, 1);
    return _ref_count;
}

ULONG STDMETHODCALLTYPE DeckLinkBGRAVideoFrame::Release()
{
    __sync_sub_and_fetch(&_ref_count, 1);
    return _ref_count;
}

HRESULT STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::GetAncillaryData(
    IDeckLinkVideoFrameAncillary **ancillary
)
{
    // nop
}

HRESULT STDMETHODCALLTYPE
DeckLinkBGRAVideoFrame::GetTimecode(
    BMDTimecodeFormat format, IDeckLinkTimecode** timecode
)
{
    // nop
}

}
