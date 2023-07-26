
// ========================================================================
//		Handle macros definitions across the whole library
// ========================================================================

#pragma once

#if		defined(_DEVELOPMENT)
#elif	defined(_DISTRIBUTED)
#else
static_assert(false, "Please set the appropriate environment");
#endif
