// direction.h

#ifndef DIRECTION_H
#define DIRECTION_H

/**
 * Enum Direction
 *
 * Enumeration Type used to represent a Movement Direction.
 * Each Direction object includes the vector of Direction.
 */
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STAY
};

/**
 * Helper struct to get x,y components for each direction
 */
struct DirectionHelper {
    /**
     * Get the X component (column displacement) of a direction
     * @param dir The direction
     * @return X displacement (-1, 0, or 1)
     */
    static int getX(Direction dir) {
        switch(dir) {
            case Direction::UP:    return 0;
            case Direction::DOWN:  return 0;
            case Direction::LEFT:  return -1;
            case Direction::RIGHT: return 1;
            case Direction::STAY:  return 0;
        }
        return 0;
    }

    /**
     * Get the Y component (row displacement) of a direction
     * @param dir The direction
     * @return Y displacement (-1, 0, or 1)
     */
    static int getY(Direction dir) {
        switch(dir) {
            case Direction::UP:    return -1;
            case Direction::DOWN:  return 1;
            case Direction::LEFT:  return 0;
            case Direction::RIGHT: return 0;
            case Direction::STAY:  return 0;
        }
        return 0;
    }
};

#endif // DIRECTION_H