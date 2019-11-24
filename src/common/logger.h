#pragma once

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_log.h>
#include <stdarg.h>

#define ASSERT(cond) \
    SDL_assert((cond))

#define ASSERT_MSG(cond, fmt, args...)                         \
    if (!(cond)) {                                             \
        SDL_LogCritical(SDL_LOG_CATEGORY_ASSERT, fmt, ##args); \
        SDL_assert((cond));                                    \
    }

#define ASSERT_FAIL(fmt, args...)                          \
    SDL_LogCritical(SDL_LOG_CATEGORY_ASSERT, fmt, ##args); \
    SDL_assert(false);

template <typename... Args>
void LogMsg(const char* fmt, const Args&... args)
{
    SDL_Log(fmt, args...);
}

template <typename... Args>
void LogError(const char* fmt, const Args&... args)
{
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, fmt, args...);
}
