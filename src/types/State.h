#ifndef STATE_H
#define STATE_H

namespace Types
{
    enum class State : uint8_t
    {
        SEARCHING_FOR_BALL,
        CENTERING_TARGET,
        MOVING_TO_BALL,
        GRAB_CLAW,
        RELEASE_CLAW,
        ROTATE_TO_BASE,
        ROTATE_TO_CENTER,
        MOVING_TO_BASE,
        CLAW_REPEATS,        // for testing, repeat the grab/release cycle a few times
        MOVE_TO_BARRIER,        // for testing, move to the barrier and stop there
        MOVE_BACK_TO_BASE        // for testing, move back to the base and stop there
    };

    inline const char *toString(State state)
    {
        switch (state)
        {
        case State::SEARCHING_FOR_BALL:
            return "SEARCHING_FOR_BALL";
        case State::CENTERING_TARGET:
            return "CENTERING_TARGET";
        case State::MOVING_TO_BALL:
            return "MOVING_TO_BALL";
        case State::GRAB_CLAW:
            return "GRAB_CLAW";
        case State::RELEASE_CLAW:
            return "RELEASE_CLAW";
        case State::ROTATE_TO_BASE:
            return "ROTATE_TO_BASE";
        case State::ROTATE_TO_CENTER:
            return "ROTATE_TO_CENTER";
        case State::MOVING_TO_BASE:
            return "MOVING_TO_BASE";
        case State::CLAW_REPEATS:
            return "CLAW_REPEATS";
        default:
            return "UNKNOWN_STATE";
        }
    };
};

#endif