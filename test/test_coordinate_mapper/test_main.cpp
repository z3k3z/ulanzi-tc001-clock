#include <Arduino.h>
#include <unity.h>

#include "CoordinateMapper.h"

namespace {

void testIndexFromCoordinatesTopLeftReturnsZero()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 999u;

    bool fSuccess = mapper.indexFromCoordinates(0, 0, uiIndex);

    TEST_ASSERT_TRUE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(0u, uiIndex);
}

void testIndexFromCoordinatesTopRightReturnsLastIndexOfFirstRow()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 999u;

    bool fSuccess = mapper.indexFromCoordinates(31, 0, uiIndex);

    TEST_ASSERT_TRUE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(31u, uiIndex);
}

void testIndexFromCoordinatesSecondRowIsSerpentine()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 999u;

    bool fSuccess = mapper.indexFromCoordinates(0, 1, uiIndex);

    TEST_ASSERT_TRUE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(63u, uiIndex);
}

void testIndexFromCoordinatesSecondRowRightEdgeReturnsRowStart()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 999u;

    bool fSuccess = mapper.indexFromCoordinates(31, 1, uiIndex);

    TEST_ASSERT_TRUE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(32u, uiIndex);
}

void testIndexFromCoordinatesBottomLeftRespectsAlternatingRows()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 999u;

    bool fSuccess = mapper.indexFromCoordinates(0, 7, uiIndex);

    TEST_ASSERT_TRUE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(255u, uiIndex);
}

void testIndexFromCoordinatesRejectsNegativeX()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 123u;

    bool fSuccess = mapper.indexFromCoordinates(-1, 0, uiIndex);

    TEST_ASSERT_FALSE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(123u, uiIndex);
}

void testIndexFromCoordinatesRejectsNegativeY()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 123u;

    bool fSuccess = mapper.indexFromCoordinates(0, -1, uiIndex);

    TEST_ASSERT_FALSE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(123u, uiIndex);
}

void testIndexFromCoordinatesRejectsXEqualToWidth()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 123u;

    bool fSuccess = mapper.indexFromCoordinates(32, 0, uiIndex);

    TEST_ASSERT_FALSE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(123u, uiIndex);
}

void testIndexFromCoordinatesRejectsYEqualToHeight()
{
    CoordinateMapper mapper(32u, 8u);
    unsigned int uiIndex = 123u;

    bool fSuccess = mapper.indexFromCoordinates(0, 8, uiIndex);

    TEST_ASSERT_FALSE(fSuccess);
    TEST_ASSERT_EQUAL_UINT(123u, uiIndex);
}

} // namespace

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testIndexFromCoordinatesTopLeftReturnsZero);
    RUN_TEST(testIndexFromCoordinatesTopRightReturnsLastIndexOfFirstRow);
    RUN_TEST(testIndexFromCoordinatesSecondRowIsSerpentine);
    RUN_TEST(testIndexFromCoordinatesSecondRowRightEdgeReturnsRowStart);
    RUN_TEST(testIndexFromCoordinatesBottomLeftRespectsAlternatingRows);
    RUN_TEST(testIndexFromCoordinatesRejectsNegativeX);
    RUN_TEST(testIndexFromCoordinatesRejectsNegativeY);
    RUN_TEST(testIndexFromCoordinatesRejectsXEqualToWidth);
    RUN_TEST(testIndexFromCoordinatesRejectsYEqualToHeight);

    UNITY_END();
}

void loop()
{
}
