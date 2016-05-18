#include "factory.h"
#include "opencv_video_source.h"
#include "opencv_video_target.h"
#ifdef USE_FFMPEG
#include "ffmpeg_video_target.h"
#endif
#ifdef USE_COLOUR_SPACE_I420
#include "epiphansdk_video_source.h"
#endif

namespace gg {

IVideoSource * Factory::_sources[2] = { NULL, NULL };

IVideoSource * Factory::connect(enum Device type) {
#ifdef USE_COLOUR_SPACE_I420
    std::string device_id = "";
#else
    int device_id = -1; // default value makes no sense
#endif

    switch (type) {
    case DVI2PCIeDuo_DVI:
#ifdef USE_COLOUR_SPACE_I420
        device_id = EpiphanSDK_DVI;
#else
        device_id = 0; // always /dev/video0
#endif
        break;
    case DVI2PCIeDuo_SDI:
#ifdef USE_COLOUR_SPACE_I420
        device_id = EpiphanSDK_SDI;
#else
        device_id = 1; // always /dev/video1
#endif
        break;
    default:
        std::string msg;
        msg.append("Device ")
           .append(std::to_string(type))
           .append(" not recognised");
        throw DeviceNotFound(msg);
    }

    if (_sources[(int) type] == NULL)
    {
#ifdef USE_COLOUR_SPACE_I420
        IVideoSource * src = nullptr;
        try
        {
            src = new VideoSourceEpiphanSDK(device_id,
                                            V2U_GRABFRAME_FORMAT_I420);
        }
        catch (VideoSourceError & e)
        {
            throw DeviceNotFound(e.what());
        }
#else
        IVideoSource * src = new VideoSourceOpenCV(device_id);
#endif

        // check querying frame dimensions
        int width = -1, height = -1;
        if (not src->get_frame_dimensions(width, height))
        {
            std::string error;
            error.append("Device ")
#ifdef USE_COLOUR_SPACE_I420
                 .append(device_id)
#else
                 .append(std::to_string(device_id))
#endif
                 .append(" connected, but ")
                 .append(" does not return frame dimensions.");
            throw DeviceOffline(error);
        }

        // check meaningful frame dimensions
        if (width <= 0 or height <= 0)
        {
            std::string error;
            error.append("Device ")
#ifdef USE_COLOUR_SPACE_I420
                 .append(device_id)
#else
                 .append(std::to_string(device_id))
#endif
                 .append(" connected, but ")
                 .append(" returns meaningless frame dimensions.");
            throw DeviceOffline(error);
        }

        // check querying frames
#ifdef USE_COLOUR_SPACE_I420
        VideoFrame_I420 frame;
#else
        VideoFrame_BGRA frame;
#endif
        if (not src->get_frame(frame))
        {
            std::string error;
            error.append("Device ")
#ifdef USE_COLOUR_SPACE_I420
                 .append(device_id)
#else
                 .append(std::to_string(device_id))
#endif
                 .append(" connected, but ")
                 .append(" does not return frames.");
            throw DeviceOffline(error);
        }

        // if no exception raised up to here, glory be to GiftGrab
        _sources[(int) type] = src;
    }

    return _sources[(int) type];
}

void Factory::disconnect(enum Device type) {
    switch (type) {
    case DVI2PCIeDuo_DVI:
    case DVI2PCIeDuo_SDI:
        break; // everything up to here is recognised
    default:
        std::string msg;
        msg.append("Device ")
           .append(std::to_string(type))
           .append(" not recognised");
        throw DeviceNotFound(msg);
    }

    if (_sources[(int) type])
    {
        delete _sources[(int) type];
        _sources[(int) type] = NULL;
    }
}

IVideoTarget * Factory::writer(Storage type)
{
    switch (type)
    {
    case File_XviD:
        return new VideoTargetOpenCV("XVID");
    case File_H265:
#ifdef USE_FFMPEG
        return new VideoTargetFFmpeg("H265");
#else
        // nop, see default below
#endif
    default:
        std::string msg;
        msg.append("Video target type ")
           .append(std::to_string(type))
           .append(" not recognised");
        throw VideoTargetError(msg);
    }
}

}
