#include "nsNDNProtocolHandler.h"
#include "mozilla/ModuleUtils.h"

NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(nsNDNProtocolHandler, Init);
NS_DEFINE_NAMED_CID(NS_NDNHANDLER_CID);

static const mozilla::Module::CIDEntry kNDNCIDs[] = {
    { &kNS_NDNHANDLER_CID, false, NULL, nsNDNProtocolHandlerConstructor },
    { NULL }
};

static const mozilla::Module::ContractIDEntry kNDNContracts[] = {
    { NS_NETWORK_PROTOCOL_CONTRACTID_PREFIX "ndn", &kNS_NDNHANDLER_CID },
    { NULL }
};

static const mozilla::Module kNDNModule = {
    mozilla::Module::kVersion,
    kNDNCIDs,
    kNDNContracts,
};

NSMODULE_DEFN(ndn) = &kNDNModule;
