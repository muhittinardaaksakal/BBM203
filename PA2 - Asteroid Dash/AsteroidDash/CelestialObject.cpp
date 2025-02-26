#include "CelestialObject.h"

// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row, int time_of_appearance)
    : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {



}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    // Additional code for copying other members if necessary
}

// Function to rotate the shape 90 degrees clockwise
vector<vector<bool>> CelestialObject::rotate_90_clockwise(const vector<vector<bool>> &shape) {
    int n = shape.size();
    int m = shape[0].size();
    vector<vector<bool>> rotated(m, vector<bool>(n));


    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            rotated[j][n - 1 - i] = shape[i][j];
        }
    }

    return rotated;
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {

    CelestialObject *current = target;
    do {
        CelestialObject *next_rotation = current->right_rotation;
        delete current;
        current = next_rotation;
    } while (current != target);
}
