
#ifndef __CHARSETENCODER_H__
#define __CHARSETENCODER_H__

#include <cmdef.h>
#include <elastos.h>

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace IO {
namespace Charset {

class CharsetEncoder
    : public ElRefBase
    , public ICharsetEncoder
{
public:
    CharsetEncoder();

    virtual ~CharsetEncoder();

    CAR_INTERFACE_DECL()

    CARAPI Init(
        /* [in] */ ICharset* cs,
        /* [in] */ Float averageBytesPerChar,
        /* [in] */ Float maxBytesPerChar);

    CARAPI Init(
        /* [in] */ ICharset* cs,
        /* [in] */ Float averageBytesPerChar,
        /* [in] */ Float maxBytesPerChar,
        /* [in] */ ArrayOf<Byte>* replacement);

    CARAPI Init(
        /* [in] */ ICharset* cs,
        /* [in] */ Float averageBytesPerChar,
        /* [in] */ Float maxBytesPerChar,
        /* [in] */ ArrayOf<Byte>* replacement,
        /* [in] */ Boolean trusted);

    CARAPI AverageBytesPerChar(
        /* [out] */ Float* averageNumber);

    CARAPI CanEncode(
        /* [in] */ Char8 c,
        /* [out] */ Boolean* result);

    CARAPI CanEncodeEx(
        /* [in] */ ICharSequence* pSequence,
        /* [out] */ Boolean* result);

    CARAPI Charset(
        /* [out] */ ICharset** charset);

    CARAPI Encode(
        /* [in] */ ICharBuffer* charBuffer,
        /* [out] */ IByteBuffer** byteBuffer);

    CARAPI EncodeEx(
        /* [in] */ ICharBuffer* charBuffer,
        /* [in, out] */ IByteBuffer* byteBuffer,
        /* [in] */ Boolean endOfInput,
        /* [out] */ ICoderResult** result);

    CARAPI Flush(
        /* [in, out] */ IByteBuffer* byteBuffer,
        /* [out] */ ICoderResult** result);

    CARAPI IsLegalReplacement(
        /* [in] */ ArrayOf<Byte>* replacement,
        /* [out] */ Boolean* result);

    CARAPI MalformedInputAction(
        /* [out] */ ICodingErrorAction** errorAction);

    CARAPI MaxBytesPerChar(
        /* [out] */ Float* maxNumber);

    CARAPI OnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction,
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI OnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction,
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI Replacement(
        /* [out, callee] */ ArrayOf<Byte>** replace);

    CARAPI ReplaceWith(
        /* [in] */ ArrayOf<Byte>* replacement,
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI Reset(
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI UnmappableCharacterAction(
        /* [out] */ ICodingErrorAction** errorAction);

    CARAPI GetCharset(
        /* [in] */ ICharset** charset);

    CARAPI GetMalformedInputAction(
        /* [out] */ ICodingErrorAction** errorAction);

    CARAPI GetMaxBytesPerChar(
        /* [out] */ Float* maxNumber);

    CARAPI GetReplacement(
        /* [out] */ ArrayOf<Byte>** replace);

    CARAPI GetUnmappableCharacterAction(
        /* [out] */ ICodingErrorAction** errorAction);

protected:
    virtual CARAPI EncodeLoop(
        /* [in] */ ICharBuffer* charBuffer,
        /* [in, out] */ IByteBuffer* byteBuffer,
        /* [out] */ ICoderResult** result) = 0;

    /**
     * Flushes this encoder. The default implementation does nothing and always
     * returns <code>CoderResult.UNDERFLOW</code>; this method can be
     * overridden if needed.
     *
     * @param out
     *            the output buffer.
     * @return <code>CoderResult.UNDERFLOW</code> or
     *         <code>CoderResult.OVERFLOW</code>.
     */
    virtual CARAPI ImplFlush(
        /* [in] */ IByteBuffer* outBuffer,
        /* [out] */  ICoderResult** result);

    /**
     * Notifies that this encoder's <code>CodingErrorAction</code> specified
     * for malformed input error has been changed. The default implementation
     * does nothing; this method can be overridden if needed.
     *
     * @param newAction
     *            the new action.
     */
    virtual CARAPI ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction);

        /**
     * Notifies that this encoder's <code>CodingErrorAction</code> specified
     * for unmappable character error has been changed. The default
     * implementation does nothing; this method can be overridden if needed.
     *
     * @param newAction
     *            the new action.
     */
    virtual CARAPI ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction);

    /**
     * Notifies that this encoder's replacement has been changed. The default
     * implementation does nothing; this method can be overridden if needed.
     *
     * @param newReplacement
     *            the new replacement string.
     */
    virtual CARAPI ImplReplaceWith(
        /* [in] */ ArrayOf<Byte>* newReplacement);

    /**
     * Resets this encoder's charset related state. The default implementation
     * does nothing; this method can be overridden if needed.
     */
    virtual CARAPI ImplReset();

private:
    // implementation of canEncode
    CARAPI ImplCanEncode(
        /* [in] */ ICharBuffer* cb,
        /* [out] */ Boolean* result);

    /*
     * checks the result whether it needs to throw CharacterCodingException.
     */
    CARAPI CheckCoderResult(
        /* [in] */ ICoderResult* result);

    // allocate more spaces to the given ByteBuffer
    CARAPI AllocateMore(
        /* [in, out] */ IByteBuffer* output,
        /* [out] */ IByteBuffer** byteBuffer);

private:
    /*
     * internal status consts
     */
    static const Int32 READY = 0;
    static const Int32 ONGOING = 1;
    static const Int32 END = 2;
    static const Int32 FLUSH = 3;
    static const Int32 INIT = 0;

    // the Charset which creates this encoder
    AutoPtr<ICharset> mCs;

    // average bytes per character created by this encoder
    Float mAverageBytesPerChar;

    // maximum bytes per character can be created by this encoder
    Float mMaxBytesPerChar;

    // replacement byte array
    AutoPtr< ArrayOf<Byte> > mReplacementBytes;

    // internal status
    Int32 mStatus;

    // internal status indicates encode(CharBuffer) operation is finished
    Boolean mFinished;

    // action for malformed input
    AutoPtr<ICodingErrorAction> mMalformedInputAction;

    // action for unmapped char input
    AutoPtr<ICodingErrorAction> mUnmappableCharacterAction;

    // decoder instance for this encoder's charset, used for replacement value
    // checking
    AutoPtr<ICharsetDecoder> mDecoder;
};

} // namespace CharSet
} // namespace IO
} // namespace Elastos

#endif // __CHARSETENCODER_H__