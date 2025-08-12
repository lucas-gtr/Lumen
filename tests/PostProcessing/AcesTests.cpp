#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/ACES.hpp"

using namespace linalg;

TEST(AcesToneMappingTest, RttAndOdtFit_IdentityForZero) {
    Vec3d input(0.0, 0.0, 0.0);
    Vec3d result = AcesToneMapping::RttAndOdtFit(input);
    EXPECT_TRUE(result.isApprox(Vec3d(0.0, 0.0, 0.0), 1e-3));
}

TEST(AcesToneMappingTest, AcesFitted_KnownValue) {
    AcesToneMapping aces;
    Vec3d input(0.18, 0.18, 0.18); // 18% gray
    Vec3d result = aces.acesFitted(input);

    // Valeur attendue pré-calculée
    Vec3d expected(0.10559125, 0.10559125, 0.10559019); // ← valeurs à ajuster après calcul manuel/outil

    EXPECT_TRUE(result.isApprox(expected, 1e-6));
}

TEST(AcesToneMappingTest, ConvertToLDR_KnownValue) {
    AcesToneMapping aces;
    ColorRGB input{1.0, 0.5, 0.25};
    ColorRGB result = aces.convertToLDR(input);

    ColorRGB expected{0.63499038, 0.38460007, 0.20316036}; // ← valeurs à ajuster après pré-calcul
    EXPECT_NEAR(result.r, expected.r, 1e-6);
    EXPECT_NEAR(result.g, expected.g, 1e-6);
    EXPECT_NEAR(result.b, expected.b, 1e-6);
}

TEST(AcesToneMappingTest, ConvertToLDR_BlackIsBlack) {
    AcesToneMapping aces;
    ColorRGB input{0.0, 0.0, 0.0};
    ColorRGB result = aces.convertToLDR(input);
    EXPECT_NEAR(result.r, 0.0, 1e-3);
    EXPECT_NEAR(result.g, 0.0, 1e-3);
    EXPECT_NEAR(result.b, 0.0, 1e-3);
}
