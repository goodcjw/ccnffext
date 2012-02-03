#ifndef nsNDNUtil_h__
#define nsNDNUtil_h__

#define NS_DEFAULT_SEGMENT_SIZE 4096
#define NS_DEFAULT_SEGMENT_COUNT 24

static inline nsIMemry*
NDN_GetSegmentAlloc(PRUint32 segsize) {
  // currently, we only use system allocator
  return nsnull;
}

static inline void
NDN_ResolveSegmentParams(PRUint32 &segsize, PRUint32 &segcount) {
  if (!segsize)
    segsize = NS_DEFAULT_SEGMENT_SIZE;
  
  if (!segcount)
    segcount = NS_DEFAULT_SEGMENT_COUNT;
}


#endif
