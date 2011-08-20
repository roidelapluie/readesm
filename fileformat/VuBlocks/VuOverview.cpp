#include "VuOverview.h"

VuOverview::VuOverview(const constDataPointer& filewalker) : RawVuOverview(filewalker)
{}

///In the overview block, the bytes in the two certificates do not count for the checksum calculation
RawData VuOverview::signedBytes() const {
	return RawData(start + 2 + 194*2, size() - 128 - 2 -  194*2);
}
