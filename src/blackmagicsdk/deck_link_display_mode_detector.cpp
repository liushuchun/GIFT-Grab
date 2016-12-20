#include "deck_link_display_mode_detector.h"

namespace gg
{

DeckLinkDisplayModeDetector::DeckLinkDisplayModeDetector()
    : _deck_link_input(nullptr)
    , _display_mode(bmdModeUnknown)
    , _error_msg("This constructor should never be used")
{
    // nop
}


DeckLinkDisplayModeDetector::DeckLinkDisplayModeDetector(IDeckLinkInput * deck_link_input,
                                                         std::vector<BMDDisplayMode> display_modes_to_check,
                                                         BMDPixelFormat pixel_format,
                                                         BMDVideoInputFlags video_input_flags)
    : _deck_link_input(deck_link_input)
    , _display_modes_to_check(display_modes_to_check)
    , _pixel_format(pixel_format)
    , _video_input_flags(video_input_flags)
    , _display_mode(bmdModeUnknown)
    , _frame_rate(0.0)
    , _error_msg("")
{
    // These are output and result variables
    BMDDisplayModeSupport display_mode_support;
    IDeckLinkDisplayMode * deck_link_display_mode = nullptr;
    HRESULT res;
    // TODO
    bool video_mode_supported = true;

    // TODO
    _display_mode = bmdModeHD1080i6000;

    // Check whether the mode is supported
    res = _deck_link_input->DoesSupportVideoMode(
        _display_mode, _pixel_format, _video_input_flags,
        &display_mode_support, &deck_link_display_mode
    );
    // No glory (could not even check mode support)
    if (res != S_OK or deck_link_display_mode == nullptr)
    {
        video_mode_supported = false;
        _error_msg = "Could not check video mode support of Blackmagic DeckLink device";
    }
    else
    {
        // If mode supported, then get frame rate
        if (display_mode_support == bmdDisplayModeSupported)
        {
            // Get frame rate of DeckLink device
            BMDTimeValue frame_rate_duration, frame_rate_scale;
            res = deck_link_display_mode->GetFrameRate(&frame_rate_duration, &frame_rate_scale);
            // No glory
            if (res != S_OK)
            {
                video_mode_supported = false;
                _error_msg = "Could not infer frame rate of Blackmagic DeckLink device";
            }
            else
                _frame_rate = (double) frame_rate_scale / (double) frame_rate_duration;
        }
        else
        {
            video_mode_supported = false;
            // TODO
            _error_msg = "Your DeckLink device does not support 1080i @ 60 Hz";
        }
    }

    // Release the DeckLink display mode object
    if (deck_link_display_mode != nullptr)
    {
        deck_link_display_mode->Release();
        deck_link_display_mode = nullptr;
    }

    if (not video_mode_supported)
        _display_mode = bmdModeUnknown;
}


DeckLinkDisplayModeDetector::~DeckLinkDisplayModeDetector()
{
    // nop
}


BMDDisplayMode DeckLinkDisplayModeDetector::get_display_mode() noexcept
{
    return _display_mode;
}


double DeckLinkDisplayModeDetector::get_frame_rate() noexcept
{
    return _frame_rate;
}


std::string DeckLinkDisplayModeDetector::get_error_msg() noexcept
{
    return _error_msg;
}


HRESULT STDMETHODCALLTYPE DeckLinkDisplayModeDetector::QueryInterface(REFIID iid, LPVOID * ppv)
{
    return E_NOINTERFACE;
}


ULONG STDMETHODCALLTYPE DeckLinkDisplayModeDetector::AddRef(void)
{
    __sync_add_and_fetch(&_n_ref, 1);
    return _n_ref;
}


ULONG STDMETHODCALLTYPE DeckLinkDisplayModeDetector::Release(void)
{
    __sync_sub_and_fetch(&_n_ref, 1);
    return _n_ref;
}


HRESULT STDMETHODCALLTYPE DeckLinkDisplayModeDetector::VideoInputFormatChanged(
    BMDVideoInputFormatChangedEvents events,
    IDeckLinkDisplayMode * display_mode,
    BMDDetectedVideoInputFormatFlags format_flags
)
{
    // nop
    return S_OK;
}


HRESULT STDMETHODCALLTYPE DeckLinkDisplayModeDetector::VideoInputFrameArrived(
    IDeckLinkVideoInputFrame * video_frame,
    IDeckLinkAudioInputPacket * audio_packet
)
{
    if (video_frame != nullptr)
    {
        if (not (video_frame->GetFlags() & bmdFrameHasNoInputSource))
            // TODO
            ;
    }
    return S_OK;
}

}
