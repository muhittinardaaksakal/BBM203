#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {

    height = image.get_height();
    width = image.get_width();
    int upperCounter = 0;
    int lowerCounter = 0;
    int pixelValue;
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    upper_triangular = new int[width*(width + 1) / 2];
    for (int y = 0; y < height; y++) {
        int diagnose = y;
        for (int x = diagnose; x < width; x++) {
            pixelValue = image.get_pixel(y, x);
            upper_triangular[upperCounter] = pixelValue;


            ++upperCounter;
        }
    }
    lower_triangular = new int[width*(width - 1) / 2];
    for (int y = 0; y < height; y++) {
        int diagnose = y;
        for (int x = 0; x < diagnose; x++) {
            pixelValue = image.get_pixel(y, x);
            lower_triangular[lowerCounter] = pixelValue;


            ++lowerCounter;
        }
    }



}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // Copy data from input arrays to instance variables
    this->width = w;
    this->height = h;
    upper_triangular = upper;
    lower_triangular = lower;
}



// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}


// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    int upperCounter = 0;
    int lowerCounter = 0;

    // Loop over all the pixels in the image
    for (int y = 0; y < height; ++y) {
        int diagnose = y;
        for (int x = diagnose; x < width; ++x) {
            image.set_pixel(y, x, upper_triangular[upperCounter]);
            ++upperCounter;
        }
    }
    for (int y = 0; y < height; y++) {
        int diagnose = y;
        for (int x = 0; x < diagnose; x++) {

            image.set_pixel(y, x, lower_triangular[lowerCounter]);


            ++lowerCounter;
        }
    }


    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    height = image.get_height();
    width = image.get_width();
    int upperCounter = 0;
    int lowerCounter = 0;
    int pixelValue;
    upper_triangular = new int[width*(width + 1) / 2];
    for (int y = 0; y < height; y++) {
        int diagnose = y;
        for (int x = diagnose; x < width; x++) {
            pixelValue = image.get_pixel(y, x);
            upper_triangular[upperCounter] = pixelValue;


            ++upperCounter;
        }
    }
    lower_triangular = new int[width*(width - 1) / 2];
    for (int y = 0; y < height; y++) {
        int diagnose = y;
        for (int x = 0; x < diagnose; x++) {
            pixelValue = image.get_pixel(y, x);
            lower_triangular[lowerCounter] = pixelValue;


            ++lowerCounter;
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    // 1. Write width and height
    file << width << " " << height << "\n";

    // 2. Write the upper triangular array
    int upperSize = width * (width + 1) / 2;
    for (int i = 0; i < upperSize; ++i) {
        file << upper_triangular[i];
        if (i != upperSize - 1) {
            file << " ";
        }
    }
    file << "\n";

    // 3. Write the lower triangular array
    int lowerSize = width * (width - 1) / 2;
    for (int i = 0; i < lowerSize; ++i) {
        file << lower_triangular[i];
        if (i != lowerSize - 1) {
            file << " ";
        }
    }
    file << "\n";

    file.close();
}


SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for reading." << std::endl;
        return SecretImage(0, 0, nullptr, nullptr);
    }

    int w, h;

    // 1. Read width and height from the first line
    file >> w >> h;

    // 2. Calculate the sizes of the upper and lower triangular arrays
    int upperSize = w * (w + 1) / 2;
    int lowerSize = w * (w - 1) / 2;

    // 3. Dynamically allocate memory for the arrays
    int* upper_triangular = new int[upperSize];
    int* lower_triangular = new int[lowerSize];

    // 4. Read the upper triangular array from the second line
    for (int i = 0; i < upperSize; ++i) {
        file >> upper_triangular[i];
    }

    // 5. Read the lower triangular array from the third line
    for (int i = 0; i < lowerSize; ++i) {
        file >> lower_triangular[i];
    }

    // 6. Close the file
    file.close();

    // 7. Return a SecretImage object initialized with the data
    return SecretImage(w, h, upper_triangular, lower_triangular);
}



// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
