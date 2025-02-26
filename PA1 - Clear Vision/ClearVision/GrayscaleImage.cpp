#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {
    // Image loading code using stbi
    int channels;

    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    // Dynamically allocate memory for the 2D matrix (data)
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];

    }

    // Fill the matrix with pixel values from the image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            set_pixel(y, x,image[y * width + x]);  // 1D to 2D index conversion
        }
    }

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}


// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) : height(h), width(w) {
    // Dynamically allocate memory for the 2D matrix (data)
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];

        // Copy values from inputData to data
        for (int j = 0; j < width; ++j) {
            set_pixel(i, j, inputData[i][j]);
        }
    }
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // Dynamically allocate memory for the 2D matrix (data)
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
        for (int j = 0; j < width; ++j) {
            set_pixel(i, j, 0); // Set each pixel value to 0
        }
    }
}


// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) : width(other.width), height(other.height) {
    // Dynamically allocate memory for the 2D matrix (data)
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];

        // Copy pixel values from the other image
        for (int j = 0; j < width; ++j) {
            set_pixel(i, j, other.data[i][j]);
        }
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    for (int i = 0; i < get_height(); ++i) {
        delete[] data[i];  // Delete the ith row
    }

    // Then, delete the array of row pointers
    delete[] data;
}


// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // Check if dimensions are the same
    if (width != other.width || height != other.height) {
        return false; // If dimensions differ, images can't be the same
    }

    // Loop through each pixel and compare values
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (this->get_pixel(i, j) != other.get_pixel(i, j)) {
                return false; // If any pixel is different, images are not equal
            }
        }
    }

    return true; // If all pixels are the same, images are equal
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // Create a new image for the result
    int resultPixel;
    GrayscaleImage result(width, height);
    GrayscaleImage firstOperand(data,height,width);
    GrayscaleImage secondOperand(other);
    // TODO: Your code goes here.
    // Add two images' pixel values and return a new image, clamping the results.
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            resultPixel = firstOperand.get_pixel(i, j) + secondOperand.get_pixel(i, j);
            if (resultPixel > 255) {
                resultPixel = 255;
            }
            result.set_pixel(i, j, resultPixel);

        }
    }

    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    int resultPixel;
    // Create a new image for the result
    GrayscaleImage result(width, height);
    GrayscaleImage firstOperand(data,height,width);
    GrayscaleImage secondOperand(other);
    // TODO: Your code goes here.
    // Subtract pixel values of two images and return a new image, clamping the results.
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            resultPixel = firstOperand.get_pixel(i, j) - secondOperand.get_pixel(i, j);
            if (resultPixel < 0) {
                resultPixel = 0;
            }
            result.set_pixel(i, j, resultPixel);

        }
    }
    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
