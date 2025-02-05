#include "Helper.h"
#include "Constants.h"

void DebugTrace(const std::string& InString)
{
#if DEBUG_MODE
	std::cout << InString << std::endl;
#endif
}
