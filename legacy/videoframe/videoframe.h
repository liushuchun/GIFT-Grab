//!
//! \brief A class to represent a video frame with BGRA pixels
//!
//! We set the format to BGRA, simply because this is native to
//! most GPUs in the wild and should be faster to load than traditional
//! RGB (due to 4 byte alignment) and RGBA due to less reordering
//! on the GPU but this is all probably hardware dependent.
class VideoFrame_BGRA : public gg::VideoFrame
{
public:
    //!
    //! \brief Constructor that by default will use externally
    //! managed frame data
    //! \param manage_data
    //!
    VideoFrame_BGRA(bool manage_data=false);

    //!
    //! \brief Allocates memory for specified dimensions, and
    //! sets all pixels to black
    //! \param rows
    //! \param cols
    //!
    VideoFrame_BGRA(const size_t rows, const size_t cols);

#ifdef USE_OPENCV
    //!
    //! \brief OpenCV based constructor
    //! \param mat
    //! \param manage_data
    //!
    VideoFrame_BGRA(const cv::Mat & mat, bool manage_data=false);
#endif // USE_OPENCV

    //!
    //! \brief Constructor with full data and parameter specs
    //! \param data if \c manage_data is set, all data in this
    //! will be copied; otherwise this \c data will be used (i.e.
    //! pointer will be copied)
    //! \param rows
    //! \param cols
    //! \param manage_data
    //!
    VideoFrame_BGRA(unsigned char * data, size_t rows, size_t cols, bool manage_data=false);

    //!
    //! \brief Copy data from \c rhs, also setting data management
    //! \param rhs
    //! \sa manages_own_data
    //!
    VideoFrame_BGRA(const VideoFrame_BGRA & rhs);

public:
    //!
    //! \brief operator =
    //! \param rhs
    //! \sa VideoFrame(const VideoFrame & rhs)
    //!
    void operator=(const VideoFrame_BGRA & rhs);

#ifdef USE_OPENCV
    //!
    //! \brief Initialise from passed OpenCV \c mat, based on the
    //! data management setting
    //! \param mat
    //! \sa manages_own_data
    //!
    void init_from_opencv_mat(const cv::Mat & mat);
#endif // USE_OPENCV

    //!
    //! \brief Initialise from passed \c data, based on the data
    //! management setting
    //! \param data
    //! \param rows
    //! \param cols
    //! \sa manages_own_data
    //!
    void init_from_pointer(unsigned char * data, size_t rows, size_t cols);

    //!
    //! \brief Get number of channels, i.e. 4 in BGRA
    //! \return
    //!
    size_t channels() const
    {
        return 4;
    }

private:
    //!
    //! \brief
    //! \param rhs
    //! \sa VideoFrame(const VideoFrame & rhs)
    //!
    void clone(const VideoFrame_BGRA &rhs);
};