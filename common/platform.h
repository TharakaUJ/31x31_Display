#pragma once

// Platform abstraction layer for threading and platform-specific functions

// Thread function signature
typedef void (*ThreadFunction)(void* params);

// Create a new thread/task
// - name: Thread name for debugging
// - function: Function to run in the thread
// - params: Parameters to pass to the function
// - stackSize: Stack size in bytes (may be ignored on some platforms)
// - priority: Thread priority (platform-dependent meaning)
void createThread(const char* name, ThreadFunction function, void* params, 
                  int stackSize, int priority);

// Platform-specific delay in milliseconds
void platformDelay(int milliseconds);

// Delete/exit the current thread
void platformDeleteCurrentThread();

// Platform-specific display initialization
void platformInitDisplay();

// Platform-specific display update (e.g., FastLED.show())
void platformShowDisplay();


#ifndef constrain
#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif
