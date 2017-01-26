#include "ivideosource.h"
extern "C" {
#include <libavformat/avformat.h>
}


namespace gg
{

//!
//! \brief Uses FFmpeg for streaming video from
//! compatible sources such as video files
//!
class VideoSourceFFmpeg : public IVideoSource
{
protected:
    //!
    //! \brief Path to the video source, e.g.
    //! video file
    //!
    std::string _source_path;

    //!
    //! \brief FFmpeg format context
    //!
    AVFormatContext * _avformat_context;
    int video_stream_idx;
    int refcount;
    AVStream * video_stream;
    AVCodecContext * video_dec_ctx;
    int width;
    int height;
    enum AVPixelFormat pix_fmt;
    AVFrame * _avframe;
    AVPacket pkt;
    uint8_t * video_dst_data[4] = {nullptr};
    int video_dst_linesize[4];
    int video_dst_bufsize;

protected:
    //!
    //! \brief Default constructor that should not
    //! be publicly called
    //!
    VideoSourceFFmpeg();

public:
    //!
    //! \brief Open video source pointed to by \c
    //! source_path for acquiring video frames in
    //! specified \c colour_space
    //! \param source_path
    //! \param colour_space
    //! \throw VideoSourceError with a detailed
    //! error message if \c source_path cannot be
    //! opened
    //!
    VideoSourceFFmpeg(std::string source_path,
                      enum ColourSpace colour_space);

    //!
    //! \brief
    //!
    virtual ~VideoSourceFFmpeg();

public:
    bool get_frame_dimensions(int & width, int & height);

    bool get_frame(VideoFrame & frame);

    double get_frame_rate();

    void set_sub_frame(int x, int y, int width, int height);

    void get_full_frame();

protected:
    //!
    //! \brief Open the FFmpeg format context
    //! \param stream_idx
    //! \param fmt_ctx
    //! \param type
    //! \param error_msg
    //! \return non-negative value on success,
    //! negative value with a detailed \c error_msg
    //! otherwise
    //!
    int open_codec_context(int * stream_idx,
                           AVFormatContext * _avformat_context,
                           enum AVMediaType type,
                           std::string & error_msg);

    //!
    //! \brief Decode current FFmpeg packet of
    //! current FFmpeg frame
    //! \param got_frame
    //! \param cached
    //! \return number of decoded bytes on success,
    //! a negative value otherwise
    //!
    int decode_packet(int * got_frame, int cached);
};

}
