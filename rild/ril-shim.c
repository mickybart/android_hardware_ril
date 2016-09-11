#define LOG_TAG "RIL_SHIM"

#include "ril-shim.h"

#include <dlfcn.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <unwind.h>

#include <cutils/compiler.h>
#include <cutils/properties.h>
#include <sys/cdefs.h>
#include <telephony/ril.h>
#include <telephony/librilutils.h>
#include <utils/Log.h>

extern const char * requestToString(int request);

typedef struct {
    int requestNumber;
    void (*dispatchFunction) (void *p, void *pRI);
    int(*responseFunction) (void *p, void *response, size_t responselen);
} CommandInfo;

typedef struct RequestInfo {
    int32_t token;
    CommandInfo *pCI;
    struct RequestInfo *p_next;
    char cancelled;
    char local;
    int client_id;      // 0 or 1 corresponding to each of RIL.java clients
} RequestInfo;

/* A copy of the original RIL function table. */
static const RIL_RadioFunctions *origRilFunctions;

/* A copy of the ril environment passed to RIL_Init. */
static const struct RIL_Env *rilEnv;

static void onRequestCompleteShim(RIL_Token t, RIL_Errno e, void *response, size_t responselen) {
	int request;
	RequestInfo *pRI;

	pRI = (RequestInfo *)t;

	/* If pRI is null, this entire function is useless. */
	if (pRI == NULL) {
        RLOGD("onRequestCompleteShim - pRI = NULL");
        rilEnv->OnRequestComplete(t, e, response, responselen);
        RLOGD("onRequestCompleteShim - finished");
	    return;
	}

	request = pRI->pCI->requestNumber;
    RLOGD("onRequestCompleteShim: %s", requestToString(request));
    ril_hexDump(NULL, response, responselen);
	rilEnv->OnRequestComplete(t, e, response, responselen);
    RLOGD("onRequestCompleteShim %s - finished", requestToString(request));
}

static void onRequestShim (int request, void *data, size_t datalen, RIL_Token t) {
    RLOGD("onRequestShim: %s", requestToString(request));
    ril_hexDump(NULL, data, datalen);

    origRilFunctions->onRequest(request, data, datalen, t);
}

const RIL_RadioFunctions* RIL_Init_Shim(void *rilInit, const struct RIL_Env *env, int argc, char **argv)
{
	RIL_RadioFunctions const* (*origRilInit)(const struct RIL_Env *env, int argc, char **argv);
	static RIL_RadioFunctions shimmedFunctions;
	static struct RIL_Env shimmedEnv;

    RLOGI("RIL_Init_Shim");

	/* Shim the RIL_Env passed to the real RIL, saving a copy of the original */
	rilEnv = env;
	shimmedEnv = *env;
	shimmedEnv.OnRequestComplete = onRequestCompleteShim;

	origRilInit = (const RIL_RadioFunctions *(*)(const struct RIL_Env *, int, char **)) rilInit;

	origRilFunctions = origRilInit(&shimmedEnv, argc, argv);
	if (CC_UNLIKELY(!origRilFunctions)) {
		RLOGE("%s: the original RIL_Init derped.\n", __func__);
		return NULL;
	}

	/* Shim functions as needed. */
	shimmedFunctions = *origRilFunctions;
	shimmedFunctions.onRequest = onRequestShim;

	return &shimmedFunctions;
}


// ####### Signal Handling #########

#define ssprintf(pBuf, ...) pBuf += sprintf(pBuf, __VA_ARGS__)

typedef struct s_BacktraceState
{
    void** current;
    void** end;
} BacktraceState;

static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context* context, void* arg)
{
    BacktraceState* state = (BacktraceState *)arg;
    uintptr_t pc = _Unwind_GetIP(context);
    if (pc) {
        if (state->current == state->end) {
            return _URC_END_OF_STACK;
        } else {
            *state->current++ = (void*)pc;
        }
    }
    return _URC_NO_REASON;
}

static size_t captureBacktrace(void** buffer, size_t max)
{
    BacktraceState state = {buffer, buffer + max};
    _Unwind_Backtrace(unwindCallback, &state);

    return state.current - buffer;
}

static void logBacktrace(void** buffer, size_t count)
{
    char buff[16384];
    char *pBuff = buff;
    size_t idx;
    for (idx = 0; idx < count; ++idx) {
        const void* addr = buffer[idx];
        const char* symbol = "";

        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname) {
            symbol = info.dli_sname;
        }
        ssprintf(pBuff, "  #%2d: %p  %s\n" , idx, addr, symbol);
    }
    RLOGE("%s", buff);
}

static void sigHandler(int sig) {
    const size_t max = 50;
    void* buffer[max];

    RLOGE("Error: signal %d", sig);
    logBacktrace(buffer, captureBacktrace(buffer, max));
    exit(EXIT_FAILURE);
}

void RIL_Shim_AddSignalHandlers() {
    RLOGI("RIL_Shim_AddSignalHandlers - registering SIGABRT & SIGSEGV handler");
    signal(SIGABRT, sigHandler);
    signal(SIGSEGV, sigHandler);
}

