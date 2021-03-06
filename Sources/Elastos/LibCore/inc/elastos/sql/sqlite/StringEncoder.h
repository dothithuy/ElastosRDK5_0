
#ifndef __STRINGENCODER_H__
#define __STRINGENCODER_H__

#include <elastos.h>

namespace Elastos {
namespace Sql {
namespace SQLite {

class StringEncoder
{
public:
    static String Encode(
        /* [in] */ AutoPtr<ArrayOf<Byte> > a);

    static String EncodeX(
        /* [in] */ AutoPtr<ArrayOf<Byte> > a);

    static AutoPtr<ArrayOf<Byte> > Decode(
        /* [in] */ const String& s);

private:
    static AutoPtr<ArrayOf<Byte> > ByteCopy(
        /* [in] */ AutoPtr<ArrayOf<Byte> > source,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ AutoPtr<ArrayOf<Byte> > target);

    static Boolean Init();

private:
    static Boolean initflag;

protected:
    static AutoPtr<ArrayOf<Char32> > xdigits;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __STRINGENCODER_H__
