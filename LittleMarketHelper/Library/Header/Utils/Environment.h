
// ========================================================================
//		Handle macros definitions across the whole library
// ========================================================================

#pragma once

#if		defined(DEVELOPMENT)
#elif	defined(DEPLOYMENT)
#else
static_assert(false, "Please set the appropriate environment");
#endif
