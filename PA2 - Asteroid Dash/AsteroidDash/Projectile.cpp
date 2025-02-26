#include "Projectile.h"

// Constructor to initialize the projectile's position and set it to active
Projectile::Projectile(int start_row, int start_col)
    : row(start_row), col(start_col), active(true) {}

// Check if the projectile is still active
bool Projectile::is_active() const {
    return active;
}

// Move the projectile (for example, move it to the right)
void Projectile::move() {
    if (active) {
        col++;
    }
}

// Deactivate the projectile (set it to inactive)
void Projectile::deactivate() {
    active = false;
}
