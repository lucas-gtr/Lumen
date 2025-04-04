#include "factorial.hpp"

/**
 * Calculates the factorial of a number recursively.
 * @param n The number to calculate the factorial of.
 * @return The factorial of n.
 * @note The function uses recursion to calculate the factorial.
 */
int factorial(int n) {
  if(n <= 1) {
    return 1;
  }
  return n * factorial(n - 1);
}