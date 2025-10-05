
#pragma once

#include <cstddef>
#include "rxskin.h"
#include <CommCtrl.h>
#include <map>
#include "fundation/tinyxml2.h"
#include "common/rximage.h"
#include "common/rxskinutil.h"
#include "controls/rxcontrol.h"

class RXSKIN_API  EasyGifReader {
    struct Internal;
    struct FrameBounds;

public:
    typedef unsigned char PixelComponent;

    enum class Error {
        UNKNOWN,
        INVALID_OPERATION,
        OPEN_FAILED,
        READ_FAILED,
        INVALID_FILENAME,
        NOT_A_GIF_FILE,
        INVALID_GIF_FILE,
        OUT_OF_MEMORY,
        CLOSE_FAILED,
        NOT_READABLE,
        IMAGE_DEFECT,
        UNEXPECTED_EOF
    };

    struct RXSKIN_API FrameDuration {
        int centiseconds;
        int milliseconds() const;
        double seconds() const;
        FrameDuration &operator+=(FrameDuration other);
        FrameDuration &operator-=(FrameDuration other);
        FrameDuration operator+(FrameDuration other) const;
        FrameDuration operator-(FrameDuration other) const;
        bool operator==(FrameDuration other) const;
        bool operator!=(FrameDuration other) const;
        bool operator<(FrameDuration other) const;
        bool operator>(FrameDuration other) const;
        bool operator<=(FrameDuration other) const;
        bool operator>=(FrameDuration other) const;
    };

    class RXSKIN_API Frame {
    public:
        Frame();
        Frame(const Frame &orig);
        Frame(Frame &&orig);
        ~Frame();
        Frame &operator=(const Frame &orig);
        Frame &operator=(Frame &&orig);
        const PixelComponent *pixels() const;
        int width() const;
        int height() const;
        FrameDuration duration() const;
        FrameDuration rawDuration() const;
    protected:
        const Internal *parentData;
        int index;
        int w, h;
        void nextFrame();
    private:
        PixelComponent *pixelBuffer;
        int disposal;
        int delay;
        PixelComponent *row(int y);
        PixelComponent *corner(const FrameBounds &bounds);
    };

    class RXSKIN_API FrameIterator : public Frame {
    public:
        enum Position {
            BEGIN,
            END,
            LOOP_END
        };
        explicit FrameIterator(const EasyGifReader *decoder = nullptr, Position position = BEGIN);
        FrameIterator &operator++();
        void operator++(int);
        bool operator==(const FrameIterator &other) const;
        bool operator!=(const FrameIterator &other) const;
        const Frame &operator*() const;
        const Frame *operator->() const;
        void rewind();
    };

    static EasyGifReader* openFile(const char *filename);
    static EasyGifReader* openMemory(const void *buffer, std::size_t size);
    static EasyGifReader* openCustom(std::size_t (*readFunction)(void *outData, std::size_t size, void *userPtr), void *userPtr);

    EasyGifReader();
    EasyGifReader(const EasyGifReader &) = delete;
    EasyGifReader(EasyGifReader &&orig);
    ~EasyGifReader();
    EasyGifReader &operator=(const EasyGifReader &) = delete;
    EasyGifReader &operator=(EasyGifReader &&orig);

    int width() const;
    int height() const;
    int frameCount() const;
    int repeatCount() const;
    bool repeatsInfinitely() const;
    FrameIterator begin() const;
    FrameIterator end() const;
    FrameIterator loopEnd() const;

private:
    Internal *data;

    explicit EasyGifReader(Internal *data);

};
