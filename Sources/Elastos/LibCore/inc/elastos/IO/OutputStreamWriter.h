#ifndef __OUTPUTSTREAMWRITER_H__
#define __OUTPUTSTREAMWRITER_H__

#include "elastos/io/Writer.h"

namespace Elastos {
namespace IO {

class OutputStreamWriter
    : public Writer
{
protected:
    OutputStreamWriter();

    /**
     * Constructs a new OutputStreamWriter using {@code out} as the target
     * stream to write converted characters to. The default character encoding
     * is used.
     *
     * @param out
     *            the non-null target stream to write converted bytes to.
     */
    CARAPI Init(IOutputStream *os);

    /**
     * Constructs a new OutputStreamWriter using {@code out} as the target
     * stream to write converted characters to and {@code enc} as the character
     * encoding. If the encoding cannot be found, an
     * UnsupportedEncodingException error is thrown.
     *
     * @param out
     *            the target stream to write converted bytes to.
     * @param enc
     *            the string describing the desired character encoding.
     * @throws NullPointerException
     *             if {@code enc} is {@code null}.
     * @throws UnsupportedEncodingException
     *             if the encoding specified by {@code enc} cannot be found.
     */
    CARAPI Init(
        /* [in] */ IOutputStream *os,
        /* [in] */ const String &enc);

    /**
     * Constructs a new OutputStreamWriter using {@code out} as the target
     * stream to write converted characters to and {@code cs} as the character
     * encoding.
     *
     * @param out
     *            the target stream to write converted bytes to.
     * @param cs
     *            the {@code Charset} that specifies the character encoding.
     */
//    CARAPI Init(
//        /* [in] */ IOutputStream *os,
//        /* [in] */ ICharset *cs);

    /**
     * Constructs a new OutputStreamWriter using {@code out} as the target
     * stream to write converted characters to and {@code enc} as the character
     * encoder.
     *
     * @param out
     *            the target stream to write converted bytes to.
     * @param enc
     *            the character encoder used for character conversion.
     */
//    CARAPI Init(
//        /* [in] */ IOutputStream *os,
//        /* [in] */ ICharsetEncoder *cs);

public:
    /**
     * Closes this writer. This implementation flushes the buffer as well as the
     * target stream. The target stream is then closed and the resources for the
     * buffer and converter are released.
     * <p>
     * Only the first invocation of this method has any effect. Subsequent calls
     * do nothing.
     *
     * @throws IOException
     *             if an error occurs while closing this writer.
     */
    CARAPI Close();

    /**
     * Flushes this writer. This implementation ensures that all buffered bytes
     * are written to the target stream. After writing the bytes, the target
     * stream is flushed as well.
     *
     * @throws IOException
     *             if an error occurs while flushing this writer.
     */
    CARAPI Flush();

    /**
     * Gets the name of the encoding that is used to convert characters to
     * bytes.
     *
     * @return the string describing the converter or {@code null} if this
     *         writer is closed.
     */
    CARAPI GetEncoding(
        /* [out] */ String *encoding);

    /**
     * Writes {@code count} characters starting at {@code offset} in {@code buf}
     * to this writer. The characters are immediately converted to bytes by the
     * character converter and stored in a local buffer. If the buffer gets full
     * as a result of the conversion, this writer is flushed.
     *
     * @param buffer
     *            the array containing characters to write.
     * @param offset
     *            the index of the first character in {@code buf} to write.
     * @param count
     *            the maximum number of characters to write.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if
     *             {@code offset + count} is greater than the size of
     *             {@code buf}.
     * @throws IOException
     *             if this writer has already been closed or another I/O error
     *             occurs.
     */
    CARAPI WriteCharsEx(
        /* [in] */ const ArrayOf<Char32>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    /**
     * Writes the character {@code oneChar} to this writer. The lowest two bytes
     * of the integer {@code oneChar} are immediately converted to bytes by the
     * character converter and stored in a local buffer. If the buffer gets full
     * by converting this character, this writer is flushed.
     *
     * @param oneChar
     *            the character to write.
     * @throws IOException
     *             if this writer is closed or another I/O error occurs.
     */
    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    /**
     * Writes {@code count} characters starting at {@code offset} in {@code str}
     * to this writer. The characters are immediately converted to bytes by the
     * character converter and stored in a local buffer. If the buffer gets full
     * as a result of the conversion, this writer is flushed.
     *
     * @param str
     *            the string containing characters to write.
     * @param offset
     *            the start position in {@code str} for retrieving characters.
     * @param count
     *            the maximum number of characters to write.
     * @throws IOException
     *             if this writer has already been closed or another I/O error
     *             occurs.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if
     *             {@code offset + count} is bigger than the length of
     *             {@code str}.
     */
    CARAPI WriteStringEx(
        /* [in] */ const String& str,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

private:
    CARAPI FlushBytes(
        /* [in] */ Boolean flushUnderlyingStream);

    CARAPI Convert(
       /* [in] */ ICharBuffer* chars);

    CARAPI DrainEncoder();

    CARAPI CheckStatus();

private:
    AutoPtr<IByteBuffer> mBytes;

    AutoPtr<IOutputStream> mOut;

//    AutoPtr<ICharsetEncoder> mEncoder;
};

} // namespace IO
} // namespace Elastos

#endif  //__OUTPUTSTREAMWRITER_H__