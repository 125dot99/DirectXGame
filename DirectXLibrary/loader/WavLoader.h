#include <string>
#include "../sound/Sound.h"

namespace gamelib
{
class WavLoader
{
private:
	WavLoader() = delete;
	~WavLoader() = delete;
public:
	static WavSound* ReadWaveFile(const std::string& _filename);
};
} // namespace gamelib