#ifndef nsNDNHandler_h___
#define nsNDNHandler_h___

#include "nsINDNProtocolHandler.h"
#include "nsIIOService.h"
#include "nsCOMPtr.h"

#define NS_NDNPROTOCOLHANDLER_CLASSNAME \
    "nsNDNProtocolHandler"
/* 412fbbf0-f5bc-4446-9ca7-52b1959c9036 */
#define NS_NDNHANDLER_CID                               \
  { 0x412fbbf0, 0xf5bc, 0x4446,                         \
      {0x9c, 0xa7, 0x52, 0xb1, 0x95, 0x9c, 0x90, 0x36} }

class nsNDNProtocolHandler : public nsINDNProtocolHandler {
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPROTOCOLHANDLER
  NS_DECL_NSINDNPROTOCOLHANDLER

  nsNDNProtocolHandler();
  virtual ~nsNDNProtocolHandler();
  
  nsresult Init();

private:
  nsCOMPtr<nsIIOService> mIOService;
};

#endif /* nsNDNHandler_h___ */
