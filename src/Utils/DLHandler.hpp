#ifndef GP__DL_HANDLER_HPP_
#define GP__DL_HANDLER_HPP_

namespace GP
{
    namespace DLHandler
    {
	void* GetSymbol(void *handle, const char * const name);
	void* GetSymbol(const char * const name);
    }
}

#endif
