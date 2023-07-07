/**
 * @file ft_test.cpp
 * @author Owen Cochell (owencochell@gmail.com)
 * @brief Tests for FT operations
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <random>
#include <ctime>
#include <vector>
#include <gtest/gtest.h>

#include "dsp/ft.hpp"
#include "dsp/util.hpp"

/// Known data to compute
std::vector<long double> ft_data = {-81.0611399999996160709, -32.4312400000002611124, -38.8382199999997827569, -111.7422700000002612, 52.9393500000000637312, 61.0861000000000130931, -16.2175099999997593923, -10.9511700000003418193, -61.7699399999997511741, -69.2286100000000882773, 0.421569999999996698238, 37.6517800000001357057, 80.4401700000000488971, -2.04557000000011644271, -85.9569899999997715442, 2.72508999999991183543, -66.3554300000000642709, -2.34488999999995559407, -40.3914900000000819981, -3.64987999999990044789, -17.7549800000004358823, 71.0510400000002124293, -48.0264400000000275734, 110.045180000000132392, -105.146259999999952539, -56.3275699999998687544, 95.8438100000005266874, 64.2721399999995513172, -43.4969999999996350254, -16.9237900000002831694, -109.863809999999695023, 84.7972800000000368273, 77.4762800000004462145, 57.2437800000003134647, -110.995029999999789382, -55.1876299999999012057, -95.6075800000002865298, 75.4721999999997809339, -12.7008200000005678967, -36.9976999999999474796, -49.1758900000000834345, -71.2256300000002166503, 108.823060000000235506, -110.420349999999915749, 90.9002899999999186831, -105.065209999999517897, 45.9153499999996984778, -1.05934999999953613926, -92.979900000000370891, -44.7161299999997329906};

/// Real and non-real parts of above data
std::vector<long double> reals = {-689.550949999998468776, -360.761730393757710567, -9.09627805513791995104, -327.622828943792766043, -237.608057949997369562, -56.4823335254953177095, -420.058431365067667523, -405.18109821272589835, 410.473673843006695933, 384.975560036773781697, -493.178524073103451975, 396.298291421185540917, 58.616429016356431804, 317.639586912586099038, -125.923587794969226675, -26.3771664745074108874, -186.769206518632613412, 192.096986502756412657, -396.525639886057485434, 145.43231520399339618, -187.554675926895537533, -345.553158653373423709, -606.114272127072479468, -44.830973873640273572, 821.155170837586112498, -357.606149999998992051};
std::vector<long double> nonreals = {0, -56.7987997464239218748, -88.4598663671056244934, 114.599317038853708184, 59.8147392880928162121, -318.732530313386465293, 25.4838243897190054234, 298.856932852788386101, -52.7828546196376580737, 639.085076012777274668, -725.060920075901687409, -217.793354878620727413, -50.8793220693166556787, -81.2345735657510905209, -312.27135476011737128, 13.6098176315475818607, -225.700304988403331885, 267.968070726491263162, -138.317628623756462919, 724.441385521063360764, 358.256926195155114334, 367.869822138617369595, -379.416274255358194706, -378.675193748761023865, -204.906665100901602239, 4.487825376552348945e-13};

/// Known complex data to compute
const std::vector<std::complex<long double>> cft_data = {std::complex<long double>(-15.189251, 0.000000), std::complex<long double>(11.704489, -7.808442), std::complex<long double>(-0.090070, 10.845936), std::complex<long double>(-11.697811, -5.706792), std::complex<long double>(-2.788215, 7.507124), std::complex<long double>(8.873675, -10.790867), std::complex<long double>(-5.838451, 2.531785), std::complex<long double>(4.630811, -6.716717), std::complex<long double>(0.936014, -15.374274), std::complex<long double>(15.496844, -3.780477), std::complex<long double>(6.904307, -2.551331), std::complex<long double>(-4.702844, 12.109802), std::complex<long double>(-4.071020, 15.037189), std::complex<long double>(7.972179, -6.781337), std::complex<long double>(0.624014, -5.571347), std::complex<long double>(4.138358, -0.394291), std::complex<long double>(-1.130739, 0.000000), std::complex<long double>(4.138358, 0.394291), std::complex<long double>(0.624014, 5.571347), std::complex<long double>(7.972179, 6.781337), std::complex<long double>(-4.071020, -15.037189), std::complex<long double>(-4.702844, -12.109802), std::complex<long double>(6.904307, 2.551331), std::complex<long double>(15.496844, 3.780477), std::complex<long double>(0.936014, 15.374274), std::complex<long double>(4.630811, 6.716717), std::complex<long double>(-5.838451, -2.531785), std::complex<long double>(8.873675, 10.790867), std::complex<long double>(-2.788215, -7.507124), std::complex<long double>(-11.697811, 5.706792), std::complex<long double>(-0.090070, -10.845936), std::complex<long double>(11.704489, 7.808442)};

// Known complex result:
const std::vector<std::complex<long double>> cft_output = {std::complex<long double>(47.8646, 0), std::complex<long double>(-60.679, 0), std::complex<long double>(-65.8105, 0), std::complex<long double>(69.5778, 0), std::complex<long double>(99.7526, 0), std::complex<long double>(-90.287, 0), std::complex<long double>(36.9178, 0), std::complex<long double>(80.6327, 0), std::complex<long double>(-88.2723, 0), std::complex<long double>(-62.5006, 0), std::complex<long double>(-76.6374, 0), std::complex<long double>(-52.985, 0), std::complex<long double>(-46.0648, 0), std::complex<long double>(-86.605, 0), std::complex<long double>(-68.5806, 0), std::complex<long double>(-56.903, 0), std::complex<long double>(-97.7982, 0), std::complex<long double>(24.3152, 0), std::complex<long double>(58.614, 0), std::complex<long double>(89.4592, 0), std::complex<long double>(-55.8749, 0), std::complex<long double>(-93.1961, 0), std::complex<long double>(19.1352, 0), std::complex<long double>(-77.2428, 0), std::complex<long double>(25.5402, 0), std::complex<long double>(54.4226, 4.33681e-19), std::complex<long double>(-49.1747, 0), std::complex<long double>(80.9814, 0), std::complex<long double>(-0.730811, 0), std::complex<long double>(-43.9266, 0), std::complex<long double>(-1.95563e-07, -2.1684e-19), std::complex<long double>(6.6234926488113743e-07, 0)};

/**
 * @brief Compares two complex numbers.
 * 
 * We compare the real and imaginary
 * parts of the two given complex numbers.
 * 
 * @param first First complex number
 * @param second Second complex number
 */
void compare_complex(std::complex<long double> first, std::complex<long double> second) {

    // Compare real part:

    ASSERT_NEAR(first.real(), second.real(), 0.0001);

    // Compare imaginary parts:

    ASSERT_NEAR(first.imag(), second.imag(), 0.0001);
}

/**
 * @brief Generates N complex random numbers
 * 
 * The real and imaginary parts of the complex numbers
 * will be -1 < p < 1.
 * 
 * @param size Number of complex numbers to generate
 * @param out Iterator to output array
 */
void rand_complex(int size, std::vector<std::complex<long double>>::iterator out) {

    // Create a random number generator:

    std::default_random_engine rand;

    // Seed random number generator:

    rand.seed(time(nullptr));

    // Iterate over size of data to create:

    for (int i = 0; i < size; ++i) {

        // Create real value:

        auto real = static_cast<long double>((static_cast<long double>(rand()) - (std::default_random_engine::max() / 4.0)) / std::default_random_engine::max());

        // Create imaginary value:

        auto nonreal = static_cast<long double>((static_cast<long double>(rand()) - (std::default_random_engine::max() / 4.0)) / std::default_random_engine::max());
    
        // Set complex value to current iterator:

        *(out + i) = std::complex<long double>(real, nonreal);
    }
}

/**
 * @brief Generates N random numbers
 * 
 * Each number will be -1 < p < 1
 * 
 * @param size Number of complex numbers to generate
 * @param out Iterator to output array
 */
void rand_real(int size, std::vector<long double>::iterator out) {

    // Create random number generator:

    std::default_random_engine rand;

    // Seed random number generator:

    rand.seed(time(nullptr));

    // Iterate over size of data to create:

    for (int i = 0; i < size; ++i) {

        // Create value:

        *(out + i) = (static_cast<long double>(rand()) - (std::default_random_engine::max() / 4.0)) / std::default_random_engine::max();
    }
}

/**
 * @brief Ensures the length functions are accurate
 * 
 */
TEST(DFT, Length) {

    // Ensure length functions are accurate:

    ASSERT_EQ(length_ft(10), (10 / 2) + 1);

    ASSERT_EQ(length_ift(10), (10 - 1) * 2);
}

/**
 * @brief Tests that random data can transformed and reversed correctly
 * 
 */
TEST(DFT, Random) {

    // Configure the signal size, should be even!

    int size = 20;

    // Create a collection of input data:

    std::vector<long double> nums;

    // Preallocate (just for fun)

    nums.reserve(size);

    // Create a random number generator:

    std::default_random_engine rand;

    // Seed random number generator:

    rand.seed(time(nullptr));

    // Create containers for real and non-real parts

    std::vector<long double> nonreal;
    std::vector<long double> real;

    // Reserve parts:

    nonreal.reserve(length_ft(size));
    real.reserve(length_ft(size));

    // Create container for output signal:

    std::vector<long double> output;

    output.reserve(size);

    // Iterate a number of times

    for (int i = 0; i < size; ++i) {

        // Add a random value to the collection:

        nums.push_back(static_cast<long double>((static_cast<long double>(rand()) - (std::default_random_engine::max() / 4.0)) / std::default_random_engine::max()));

        // Add a zero to the output signal:

        output.push_back(0);
    }

    for (int i = 0; i < length_ft(size); ++i) {

        // Set each value to zero:

        nonreal.push_back(0);
        real.push_back(0);
    }

    // Now, send the data through the FT algorithm:

    dft(nums.begin(), size, real.begin(), nonreal.begin());

    // Send parts through inverse algorithm:

    inv_dft(real.begin(), nonreal.begin(), length_ft(size), output.begin());

    // Ensure data matches:

    for(int i = 0; i < size; ++i) {

        // Check this value:

        ASSERT_NEAR(nums.at(i), output.at(i), 0.0000001);
    }
}

/**
 * @brief Tests that the DFT function can work correctly with known data
 * 
 */
TEST(DFT, Known) {

    // Create vectors containing responses:

    std::vector<long double> real;
    std::vector<long double> nonreal;

    // Get length of output data:

    int output_length = length_ft(ft_data.size());

    // Reserve the vectors:

    real.reserve(output_length);
    nonreal.reserve(output_length);

    // Fill the vectors with zeros:

    for (int i = 0; i < output_length; ++i) {

        real.push_back(0);
        nonreal.push_back(0);
    }

    // Now, send the data through the DFT function:

    dft(ft_data.begin(), static_cast<int>(ft_data.size()), real.begin(), nonreal.begin());

    // Finally, ensure returned data is correct:

    for (int i = 0; i < output_length; ++i) {

        // Check real part:

        ASSERT_DOUBLE_EQ(reals.at(i), real.at(i));

        // Check non-real part:

        ASSERT_DOUBLE_EQ(nonreals.at(i), nonreal.at(i));
    }
}

/**
 * @brief Tests that the DFT works with AudioBuffers
 * 
 */
TEST(DFT, KnownBuffer) {

    // Create AudioBuffers for known data:

    auto dbuff = std::make_unique<AudioBuffer>(ft_data);

    // Sed buffer through algorithm:

    auto fbuff = dft(std::move(dbuff));

    // Check parts to ensure accuracy:

    auto real_iter = fbuff->chbegin()->begin();
    auto nonreal_iter = (fbuff->chbegin()+1)->begin();

    for (int i = 0; i < fbuff->size(); ++i) {

        // Check real part

        ASSERT_DOUBLE_EQ(*real_iter, reals.at(i));

        // Check non-real part

        ASSERT_DOUBLE_EQ(*nonreal_iter, nonreals.at(i));

        // Increment iterators

        ++real_iter;
        ++nonreal_iter;
    }
}

/**
 * @brief Tests that the inverse DFT works with known data
 * 
 */
TEST(DFT, InvKnown) {

    // Create vector for output data:

    std::vector<long double> output;

    // Determine size of output data:

    int output_size = length_ift(nonreals.size());

    // Reserve the output data:

    output.reserve(output_size);

    // Fill output buffer with 0:

    for (int i = 0; i < output_size; ++i) {

        output.push_back(0);
    }

    // Send data through function:

    inv_dft(reals.begin(), nonreals.begin(), static_cast<int>(reals.size()), output.begin());

    // Ensure outgoing data is correct:

    for (int i = 0; i < output_size; ++i) {

        // Ensure returned data is accurate:

        ASSERT_NEAR(ft_data.at(i), output.at(i), 0.00001);
    }
}

/**
 * @brief Tests that the inverse DFT works with AudioBuffers
 * 
 */
TEST(DFT, InvKnownBuffer) {

    // Create AudioBuffers for real and non-real parts:

    auto real = std::make_unique<AudioBuffer>(reals);
    auto nonreal = std::make_unique<AudioBuffer>(nonreals);

    // Do operation

    auto bdata = inv_dft(std::move(real), std::move(nonreal));

    // Ensure data is accurate:

    auto data_iter = bdata->ibegin();

    for(int i = 0; i < bdata->size(); ++i) {

        // Ensure current data is accurate:

        ASSERT_NEAR(*data_iter, ft_data.at(i), 0.00001);

        // Increment iterator:

        ++data_iter;
    }
}

/**
 * @brief Tests that the radix 2 FFT algorithm works with known data
 * 
 */
TEST(FFT2, OutKnown) {

    // Get length of output data:

    int output_length = static_cast<int>(cft_data.size());

    // Create vector of output data:

    std::vector<std::complex<long double>> out(output_length);

    // Fill output buffer with zeroes:

    std::fill_n(out.begin(), output_length, 0);

    // Send data through FFT function:

    fft_c_radix2(cft_data.begin(), output_length, out.begin());

    // Iterate over output data:

    for (int i = 0; i < output_length; ++i) {

        // Compare with known data:

        compare_complex(out.at(i), cft_output.at(i));
    }
}

/**
 * @brief Tests that the radix 2 inverse FFT algorithm works with known data
 * 
 */
TEST(FFT2, InvOutKnown) {

    // Get length of output data:

    int output_length = static_cast<int>(cft_output.size());

    // Create vector of output data:

    std::vector<std::complex<long double>> out(output_length);

    // Reserve output buffer:

    out.reserve(output_length);

    // Fill output buffer with zeros:

    std::fill_n(out.begin(), output_length, 0);

    // Send data through IFFT function:

    ifft_c_radix2(cft_output.begin(), output_length, out.begin());

    // Iterate over output data:

    for (int i = 0; i < output_length; ++i) {

        // Compare with known data:

        compare_complex(out.at(i), cft_data.at(i));
    }
}

/**
 * @brief Tests that the Radix2 (i)FFT out of place algorithm works with random data
 * 
 */
TEST(FFT2, RandOut) {

    // Define size of random data:

    int size = 32;

    // Create vector to hold random data:

    std::vector<std::complex<long double>> rdata(size);
    std::vector<std::complex<long double>> output(size);
    std::vector<std::complex<long double>> final_data(size);

    // Create random complex data:

    rand_complex(size, rdata.begin());

    // Run through Radix2:

    fft_c_radix2(rdata.begin(), size, output.begin());

    // Run through inverse:

    ifft_c_radix2(output.begin(), size, final_data.begin());

    // Check final data matches input:

    for (int i = 0; i < size; ++i) {

        // Compare the complex number:

        compare_complex(rdata.at(i), final_data.at(i));
    }
}

/**
 * @brief Tests that the radix 2 FFT in place algorithm works with known data
 * 
 * This also tests that the bit reversal sorting function
 * works correctly.
 */
TEST(FFT2, KnownInplace) {

    // Get length of output data:

    int output_length = static_cast<int>(cft_data.size());

    // Create vector of output data:

    std::vector<std::complex<long double>> out(output_length);

    // Fill output buffer with data from input:

    std::copy(cft_data.begin(), cft_data.end(), out.begin());

    // Send data through FFT function:

    fft_c_radix2(out.begin(), output_length);

    // Iterate over output data:

    bit_reverse(output_length, out.begin());

    for (int i = 0; i < output_length; ++i) {

        // Compare with known data:

        compare_complex(out.at(i), cft_output.at(i));
    }
}

/**
 * @brief Tests that the Radix2 Inverse FFT in place algorithm works with known data
 * 
 */
TEST(FFT2, InvKnownInplace) {

    // Get length of output data:

    int output_length = static_cast<int>(cft_output.size());

    // Create vector of output data:

    std::vector<std::complex<long double>> out(output_length);

    // Fill output buffer with data from input:

    std::copy(cft_output.begin(), cft_output.end(), out.begin());

    // Send data through FFT function:

    ifft_c_radix2(out.begin(), output_length);

    // Iterate over output data:

    bit_reverse(output_length, out.begin());

    // Iterate over output data:

    for (int i = 0; i < output_length; ++i) {

        // Compare with known data:

        compare_complex(out.at(i), cft_data.at(i));
    }
}

/**
 * @brief Tests that the radix 2 (i)FFT in place algorithm works with random data
 * 
 */
TEST(FFT2, RandInPlace) {

    // Define size of random data:

    int size = 32;

    // Create vector to hold random data:

    std::vector<std::complex<long double>> rdata(size);
    std::vector<std::complex<long double>> output(size);

    // Create random complex data:

    rand_complex(size, rdata.begin());

    // Copy to output:

    std::copy_n(rdata.begin(), size, output.begin());

    // Run through Radix 2:

    fft_c_radix2(output.begin(), size);

    // Do bit reversal:

    bit_reverse(size, output.begin());

    // Run through inverse Radix 2:

    ifft_c_radix2(output.begin(), size);

    // Do bit reversal:

    bit_reverse(size, output.begin());

    // Check final data matches input:

    for (int i = 0; i < size; ++i) {

        // Compare the complex number:

        compare_complex(rdata.at(i), output.at(i));
    }
}

/**
 * @brief tests that FFT operations on naive real data works 
 * 
 */
TEST(FFT2, Real) {

    // Create vector to hold real to complex data:

    std::vector<std::complex<long double>> out(ft_data.size());

    // Sed data through real FFT function:

    fft_r_radix2(ft_data.begin(), ft_data.size(), out.begin());
}
