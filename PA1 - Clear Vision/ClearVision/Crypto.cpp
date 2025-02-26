#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();
    int width = image.get_width();
    int height = image.get_height();

    // 2. Calculate the image dimensions.
    int total_pixels = width * height;
    // 3. Determine the total bits required based on message length.
    int neededBits = message_length * 7;

    // 4. Ensure the image has enough pixels; if not, throw an error.

    if (total_pixels < neededBits) {
        throw std::runtime_error("Error: The image does not have enough pixels to store the message.");
    }
    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    int unused_bits = total_pixels - neededBits;
    int unused_rows = unused_bits / width;  // This gives the number of unused rows
    int remained_bits = unused_bits % width;  // This gives the unused bits in the row
    for (int y = 0; y < height && LSB_array.size() < neededBits; ++y) {
        if (y < unused_rows) {
            continue;
        }
        for (int x = 0; x < width && LSB_array.size() < neededBits; ++x) {
            if(y == unused_rows) {
                if(x < remained_bits) {
                    continue;

                }
            }
            int pixel_value = image.get_pixel(y, x);


            int LSB = pixel_value & 1;


            LSB_array.push_back(LSB);

        }
    }
    // 6. Extract LSBs from the image pixels and return the result.

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("Error: LSB array size is not a multiple of 7.");
    }

    // 2. Convert each group of 7 bits into an ASCII character.
    for (size_t i = 0; i < LSB_array.size(); i += 7) {
        int ascii_value = 0;


        for (int j = 0; j < 7; ++j) {

            ascii_value = (ascii_value << 1) | LSB_array[i + j];
        }

        message += static_cast<char>(ascii_value);
    }

    // 3. Return the resulting message.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    // 1. Convert each character of the message into a 7-bit binary representation.
    for (char c : message) {

        std::bitset<7> binary_representation(c);

        // 2. Collect the bits into the LSB array.
        for (int i = 6; i >= 0; --i) {  // Extract bits from MSB to LSB

            LSB_array.push_back(binary_representation[i]);
        }
    }

    // 3. Return the array of bits.

    return LSB_array;
}


// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {


    int width = image.get_width();
    int height = image.get_height();
    int total_pixels = width * height;

    // 1. Ensure the image has enough pixels to store the LSB array.
    if (total_pixels < LSB_array.size()) {
        throw std::runtime_error("Error: The image does not have enough pixels to store the LSB array.");
    }
    int unused_bits = total_pixels - LSB_array.size();
    int unused_rows = unused_bits / width;  // This gives the number of unused rows
    int remained_bits = unused_bits % width;  // This gives the unused bits in the row
    int LSB_index = 0;
    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    for (int y = 0; y < height && LSB_index < LSB_array.size(); ++y) {
        if (y < unused_rows) {
            continue;
        }
        for (int x = 0; x < width && LSB_index < LSB_array.size(); ++x) {
            if(y == unused_rows) {
                if(x < remained_bits) {
                    continue;

                }
            }
            // 3. Iterate over the image pixels, embedding LSBs from the array.
            int pixel_value = image.get_pixel(y, x);
            pixel_value = (pixel_value & ~1) | LSB_array[LSB_index];



            image.set_pixel(y, x, pixel_value);

            ++LSB_index;
        }
    }


    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    //    with the embedded message.

    return SecretImage(image);
}
