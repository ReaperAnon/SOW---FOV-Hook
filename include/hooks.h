#ifndef HOOKS
    #define HOOKS

    namespace GameHook
    {
        void CreateFOVHook();

        // Sets up all the code injections and hooks for the game and reads the config file.
        void CreateGameHooks();

        void IncreaseFOV();
        void DecreaseFOV();
    }
#endif