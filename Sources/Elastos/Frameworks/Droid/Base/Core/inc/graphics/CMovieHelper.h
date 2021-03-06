
#ifndef __CMOVIEHELPER_H__
#define __CMOVIEHELPER_H__

#include "_CMovieHelper.h"

using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CMovieHelper)
{
public:
    CARAPI DecodeFile(
        /* [in] */ const String& pathName,
        /* [out] */ IMovie** movie);

    CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [out] */ IMovie** movie);

    CARAPI DecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ IMovie** movie);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CMOVIEHELPER_H__
