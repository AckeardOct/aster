#pragma once

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_log.h>

template<typename... Args>
void
LogMsg(const char* fmt, const Args&... args)
{
  SDL_Log(fmt, args...);
}

template<typename... Args>
void
LogError(const char* fmt, const Args&... args)
{
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, fmt, args...);
}

template<typename... Args>
void
LogCritical(const char* fmt, const Args&... args)
{
  SDL_LogCritical(SDL_LOG_CATEGORY_ASSERT, fmt, args...);
  SDL_assert(false);
}
