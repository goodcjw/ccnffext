#include "nsNDNProtocolHandler.h"
#include "nsIURL.h"
//#include "nsIClassInfoImpl.h"
#include "nsNetUtil.h"
#include "nsNetCID.h"
//#include "nsStandardURL.h"

//NS_IMPL_CLASSINFO(nsNDNProtocolHandler, NULL, 0, NS_NDNHANDLER_CID)
NS_IMPL_ISUPPORTS2(nsNDNProtocolHandler,
                   nsIProtocolHandler,
                   nsINDNProtocolHandler);

nsNDNProtocolHandler::nsNDNProtocolHandler() {
}

nsNDNProtocolHandler::~nsNDNProtocolHandler() {
}

nsresult nsNDNProtocolHandler::Init() {
  nsresult rv;

  mIOService = do_GetIOService(&rv);
  if (NS_FAILED(rv))
    return rv;

  return NS_OK;
}

NS_IMETHODIMP nsNDNProtocolHandler::GetScheme(nsACString & result) {
  result.AssignLiteral("ndn");
  return NS_OK;
}

NS_IMETHODIMP
nsNDNProtocolHandler::GetDefaultPort(PRInt32 *result) {
  *result = 80;        // no port for res: URLs
  return NS_OK;
}

NS_IMETHODIMP nsNDNProtocolHandler::GetProtocolFlags(PRUint32 *result) {
  *result = URI_NORELATIVE | URI_NOAUTH | URI_LOADABLE_BY_ANYONE;
  return NS_OK;
}

NS_IMETHODIMP nsNDNProtocolHandler::AllowPort(PRInt32 port,
                                              const char * scheme, 
                                              bool *_retval) {
  // XXX: port in NDN
  return NS_OK;
}

NS_IMETHODIMP nsNDNProtocolHandler::NewURI(const nsACString & aSpec,
                                           const char * aCharset,
                                           nsIURI *aBaseURI,
                                           nsIURI * *result) {
  nsresult rv;
  NS_ASSERTION(!aBaseURI, "base url passed into ndn protocol handler");
  /*
  nsStandardURL* url = new nsStandardURL();
  if (!url)
    return NS_ERROR_OUT_OF_MEMORY;
  NS_ADDREF(url);
  rv = url->Init(nsIStandardURL::URLTYPE_STANDARD, -1, aSpec, aCharset, aBaseURI);
  if (NS_SUCCEEDED(rv))
    rv = CallQueryInterface(url, result);
  NS_RELEASE(url);

  */
  nsIURI* url;
  rv = CallCreateInstance(NS_SIMPLEURI_CONTRACTID, &url);
  if (NS_FAILED(rv))
    return rv;

  rv = url->SetSpec(aSpec);
  if (NS_FAILED(rv)) {
    NS_RELEASE(url);
    return rv;
  }
  *result = url;
  return rv;
}

NS_IMETHODIMP nsNDNProtocolHandler::NewChannel(nsIURI *aURI,
                                               nsIChannel * *result) {
  nsresult rv;
  nsCAutoString tag("http://twitter.com/goodcjw");
  nsCOMPtr<nsIURI> uri;
  rv = mIOService->NewURI(tag, nsnull, nsnull,
                          getter_AddRefs(uri));
  rv = mIOService->NewChannel(tag, nsnull, nsnull, result);
  return NS_OK;
}
