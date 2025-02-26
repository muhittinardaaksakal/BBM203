#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
public:
    int row, col;    // Position of the projectile
    bool active;     // Whether the projectile is active or not

    // Constructor to initialize the projectile's position and set it to active
    Projectile(int start_row, int start_col);

    // Function to check if the projectile is active
    bool is_active() const;

    // Function to move the projectile (typically moves to the right)
    void move();

    // Function to deactivate the projectile when it goes out of bounds
    void deactivate();
};

#endif // PROJECTILE_H
