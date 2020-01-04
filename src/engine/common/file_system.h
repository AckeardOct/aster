#pragma once

#include "common/string.h"

namespace fs {

bool exists(StringRef filePath);

String cat(StringRef filePath);

} // namespace fs
