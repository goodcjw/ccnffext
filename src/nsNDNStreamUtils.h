#ifndef nsNDNStreamUtil_h__
#define nsNDNStreamUtil_h__

#include "nsStringAPI.h"
#include "nsIInputStream.h"

class nsIOutputStream;
class nsIInputStreamCallback;
class nsIOutputStreamCallback;
class nsIEventTarget;
class nsIAsyncInputStream;
class nsIAsyncOutputStream;

/**
 * A "one-shot" proxy of the OnInputStreamReady callback.  The resulting
 * proxy object's OnInputStreamReady function may only be called once!  The
 * proxy object ensures that the real notify object will be free'd on the
 * thread corresponding to the given event target regardless of what thread
 * the proxy object is destroyed on.
 *
 * This function is designed to be used to implement AsyncWait when the
 * aTarget parameter is non-null.
 */
extern nsresult
NDN_NewInputStreamReadyEvent(nsIInputStreamCallback **aEvent,
                             nsIInputStreamCallback  *aNotify,
                             nsIEventTarget          *aTarget);

/**
 * A "one-shot" proxy of the OnOutputStreamReady callback.  The resulting
 * proxy object's OnOutputStreamReady function may only be called once!  The
 * proxy object ensures that the real notify object will be free'd on the
 * thread corresponding to the given event target regardless of what thread 
 * the proxy object is destroyed on.
 *
 * This function is designed to be used to implement AsyncWait when the
 * aTarget parameter is non-null.
 */
extern nsresult
NDN_NewOutputStreamReadyEvent(nsIOutputStreamCallback **aEvent,
                              nsIOutputStreamCallback  *aNotify,
                              nsIEventTarget           *aTarget);

/* ------------------------------------------------------------------------- */

enum nsNDNAsyncCopyMode {
  NDN_ASYNCCOPY_VIA_READSEGMENTS,
  NDN_ASYNCCOPY_VIA_WRITESEGMENTS
};

/**
 * This function is called when the async copy process completes.  The reported
 * status is NS_OK on success and some error code on failure.
 */
typedef void (* nsNDNAsyncCopyCallbackFun)(void *closure, nsresult status);

/**
 * This function asynchronously copies data from the source to the sink. All
 * data transfer occurs on the thread corresponding to the given event target.
 * A null event target is not permitted.
 *
 * The copier handles blocking or non-blocking streams transparently.  If a
 * stream operation returns NS_BASE_STREAM_WOULD_BLOCK, then the stream will
 * be QI'd to nsIAsync{In,Out}putStream and its AsyncWait method will be used
 * to determine when to resume copying.
 *
 * Source and sink are closed by default when copying finishes or when error
 * occurs. Caller can prevent closing source or sink by setting aCloseSource
 * or aCloseSink to false.
 *
 * Caller can obtain aCopierCtx to be able to cancel copying.
 */
extern nsresult
NDN_AsyncCopy(nsIInputStream             *aSource,
              nsIOutputStream            *aSink,
              nsIEventTarget             *aTarget,
              nsNDNAsyncCopyMode          aMode = NDN_ASYNCCOPY_VIA_READSEGMENTS,
              PRUint32                    aChunkSize = 4096,
              nsNDNAsyncCopyCallbackFun   aCallbackFun = nsnull,
              void                       *aCallbackClosure = nsnull,
              bool                        aCloseSource = true,
              bool                        aCloseSink = true,
              nsISupports               **aCopierCtx = nsnull);

/**
 * This    function cancels copying started by function NDN_AsyncCopy.
 *
 * @param aCopierCtx
 *        Copier context returned by NDN_AsyncCopy.
 * @param aReason
 *        A failure code indicating why the operation is being canceled.
 *        It is an error to pass a success code.
 */
extern nsresult
NDN_CancelAsyncCopy(nsISupports *aCopierCtx, nsresult aReason);

/**
 * This function copies all of the available data from the stream (up to at
 * most aMaxCount bytes) into the given buffer.  The buffer is truncated at
 * the start of the function.
 *
 * If an error occurs while reading from the stream or while attempting to
 * resize the buffer, then the corresponding error code is returned from this
 * function, and any data that has already been read will be returned in the
 * output buffer.  This allows one to use this function with a non-blocking
 * input stream that may return NS_BASE_STREAM_WOULD_BLOCK if it only has
 * partial data available.
 *
 * @param aSource
 *        The input stream to read.
 * @param aMaxCount
 *        The maximum number of bytes to consume from the stream.  Pass the
 *        value PR_UINT32_MAX to consume the entire stream.  The number of
 *        bytes actually read is given by the length of aBuffer upon return.
 * @param aBuffer
 *        The string object that will contain the stream data upon return.
 *        Note: The data copied to the string may contain null bytes and may
 *        contain non-ASCII values.
 */
extern nsresult
NDN_ConsumeStream(nsIInputStream *aSource, PRUint32 aMaxCount,
                  nsACString &aBuffer);

/**
 * This function tests whether or not the input stream is buffered.  A buffered
 * input stream is one that implements readSegments.  The test for this is to
 * simply call readSegments, without actually consuming any data from the
 * stream, to verify that it functions.
 *
 * NOTE: If the stream is non-blocking and has no data available yet, then this
 * test will fail.  In that case, we return false even though the test is not 
 * really conclusive.
 *
 * @param aInputStream
 *        The input stream to test.
 */
extern bool
NDN_InputStreamIsBuffered(nsIInputStream *aInputStream);

/**
 * This function tests whether or not the output stream is buffered.  A
 * buffered output stream is one that implements writeSegments.  The test for
 * this is to simply call writeSegments, without actually writing any data into
 * the stream, to verify that it functions.
 *
 * NOTE: If the stream is non-blocking and has no available space yet, then
 * this test will fail.  In that case, we return false even though the test is
 * not really conclusive.
 *
 * @param aOutputStream
 *        The output stream to test.
 */
extern bool
NDN_OutputStreamIsBuffered(nsIOutputStream *aOutputStream);

/**
 * This function is intended to be passed to nsIInputStream::ReadSegments to
 * copy data from the nsIInputStream into a nsIOutputStream passed as the
 * aClosure parameter to the ReadSegments function.
 *
 * @see nsIInputStream.idl for a description of this function's parameters.
 */
extern NS_METHOD
NDN_CopySegmentToStream(nsIInputStream *aInputStream, void *aClosure,
                        const char *aFromSegment, PRUint32 aToOffset,
                        PRUint32 aCount, PRUint32 *aWriteCount);

/**
 * This function is intended to be passed to nsIInputStream::ReadSegments to
 * copy data from the nsIInputStream into a character buffer passed as the
 * aClosure parameter to the ReadSegments function.  The character buffer
 * must be at least as large as the aCount parameter passed to ReadSegments.
 *
 * @see nsIInputStream.idl for a description of this function's parameters.
 */
extern NS_METHOD
NDN_CopySegmentToBuffer(nsIInputStream *aInputStream, void *aClosure,
                        const char *aFromSegment, PRUint32 aToOffset,
                        PRUint32 aCount, PRUint32 *aWriteCount);

/**
 * This function is intended to be passed to nsIInputStream::ReadSegments to
 * discard data from the nsIInputStream.  This can be used to efficiently read
 * data from the stream without actually copying any bytes.
 *
 * @see nsIInputStream.idl for a description of this function's parameters.
 */
extern NS_METHOD
NDN_DiscardSegment(nsIInputStream *aInputStream, void *aClosure,
                   const char *aFromSegment, PRUint32 aToOffset,
                   PRUint32 aCount, PRUint32 *aWriteCount);

/**
 * This function is intended to be passed to nsIInputStream::ReadSegments to
 * adjust the aInputStream parameter passed to a consumer's WriteSegmentFun.
 * The aClosure parameter must be a pointer to a nsNDNWriteSegmentThunk object.
 * The mStream and mClosure members of that object will be passed to the mFun
 * function, with the remainder of the parameters being what are passed to
 * NDN_WriteSegmentThunk.
 *
 * This function comes in handy when implementing ReadSegments in terms of an
 * inner stream's ReadSegments.
 */
extern NS_METHOD
NDN_WriteSegmentThunk(nsIInputStream *aInputStream, void *aClosure,
                      const char *aFromSegment, PRUint32 aToOffset,
                      PRUint32 aCount, PRUint32 *aWriteCount);

struct nsNDNWriteSegmentThunk {
  nsIInputStream    *mStream;
  nsWriteSegmentFun  mFun;
  void              *mClosure;
};

/**
 * NS_NewPipe2 reimplemented, because it's not exported by XPCOM
 */
/**
 * NS_NewPipe2
 *
 * This function supersedes NS_NewPipe.  It differs from NS_NewPipe in two
 * major ways:
 *  (1) returns nsIAsyncInputStream and nsIAsyncOutputStream, so it is
 *      not necessary to QI in order to access these interfaces.
 *  (2) the size of the pipe is determined by the number of segments
 *      times the size of each segment.
 *
 * @param pipeIn
 *        resulting input end of the pipe
 * @param pipeOut
 *        resulting output end of the pipe
 * @param nonBlockingInput
 *        true specifies non-blocking input stream behavior
 * @param nonBlockingOutput
 *        true specifies non-blocking output stream behavior
 * @param segmentSize
 *        specifies the segment size in bytes (pass 0 to use default value)
 * @param segmentCount
 *        specifies the max number of segments (pass 0 to use default value)
 *        passing PR_UINT32_MAX here causes the pipe to have "infinite" space.
 *        this mode can be useful in some cases, but should always be used with
 *        caution.  the default value for this parameter is a finite value.
 * @param segmentAlloc
 *        pass reference to nsIMemory to have all pipe allocations use this
 *        allocator (pass null to use the default allocator)
 */
nsresult NDN_NewPipe2(nsIAsyncInputStream** input,
                      nsIAsyncOutputStream** output,
                      bool nonBlockingInput,
                      bool nonBlockingOutput,
                      PRUint32 segmentSize,
                      PRUint32 segmentCount,
                      nsIMemory* segmentAlloc);

#endif // nsNDNStreamUtil_h__
