#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // 1. Copy the original image for reference.
    GrayscaleImage copyImage(image);
    int height = image.get_height();
    int width = image.get_width();

    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    int kernelRadius = kernelSize / 2;
    int kernelDenominator = kernelSize * kernelSize;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum = 0;



            for (int neighbourColumn = y - kernelRadius; neighbourColumn <= y + kernelRadius; ++neighbourColumn) {
                for (int neighbourRow = x - kernelRadius; neighbourRow <= x + kernelRadius; ++neighbourRow) {

                    // Check if the neighbor pixel is within bounds
                    if (neighbourRow >= 0 && neighbourRow < width && neighbourColumn >= 0 && neighbourColumn < height) {
                        sum += copyImage.get_pixel(neighbourColumn, neighbourRow);  // Access the pixel value

                    }
                }
            }



            int meanValue = sum / kernelDenominator;

            // 3. Update each pixel with the computed mean.
            image.set_pixel(y, x, meanValue);
        }
    }
}


// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    int kernelRadius = kernelSize / 2;
    double sigma2 = sigma * sigma;
    int height = image.get_height();
    int width = image.get_width();

    GrayscaleImage copyImage(image);
    double kernel[kernelSize][kernelSize];
    double sum = 0.0;

    // 1. Create a Gaussian kernel based on the given sigma value.
    for (int i = -kernelRadius; i <= kernelRadius; ++i) {
        for (int j = -kernelRadius; j <= kernelRadius; ++j) {
            double value = exp(-(i * i + j * j) / (2 * sigma2)) / (2 * M_PI * sigma2);
            kernel[i + kernelRadius][j + kernelRadius] = value;
            sum += value;
        }
    }

    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }

    // 3. For each pixel, compute the weighted sum using the kernel.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double newPixel = 0.0;

            for (int ky = -kernelRadius; ky <= kernelRadius; ++ky) {
                for (int kx = -kernelRadius; kx <= kernelRadius; ++kx) {
                    int neighborX = x + kx;
                    int neighborY = y + ky;

                    if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                        newPixel += copyImage.get_pixel(neighborX, neighborY) * kernel[kx + kernelRadius][ky + kernelRadius];
                    }
                }
            }

            // 4. Update the pixel values with the smoothed results.
            image.set_pixel(x, y, static_cast<int>(newPixel));
        }
    }
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // 1. Blur the image using Gaussian smoothing
    GrayscaleImage blurredImage(image);
    double sigma = 1.0;
    apply_gaussian_smoothing(blurredImage, kernelSize, sigma);

    int height = image.get_height();
    int width = image.get_width();
    int originalPixel;
    int blurredPixel;
    int filteredPixel;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            originalPixel = image.get_pixel(y, x);
            blurredPixel = blurredImage.get_pixel(y, x);


            blurredPixel = originalPixel - blurredPixel;

            // 2. Apply the unsharp mask formula: original + amount * (original - blurred)
            filteredPixel = originalPixel + static_cast<int>(floor(amount * blurredPixel));

            // 3. Clip values to ensure they are within a valid range [0-255].
            if (filteredPixel > 255) {
                filteredPixel = 255;
            } else if (filteredPixel < 0) {
                filteredPixel = 0;
            }

            // Update the pixel in the image
            image.set_pixel(y, x, filteredPixel);
        }
    }
}


