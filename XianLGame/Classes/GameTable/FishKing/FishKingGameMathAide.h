#ifndef _FishKing_MATH_AIDE_H_
#define _FishKing_MATH_AIDE_H_

#include <vector>
#include "FishKingMessageHead.h"

namespace FishKing
{
	typedef std::vector<FPointAngle> TraceVector;

	template <typename T, size_t N>
	char (&ArraySizeHelper(T (&array)[N]))[N];

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

class MathAide {
private:
  MathAide();

public:
  static int Factorial(int number);
  static int Combination(int count, int r);
  static float CalcDistance(float x1, float y1, float x2, float y2);
  static float CalcAngle(float x1, float y1, float x2, float y2);
  static float CalcAngle1(float x1, float y1, float x2, float y2);
  static void BuildLinear(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance);
  static void BuildLinear(float init_x[], float init_y[], int init_count, std::vector<FPointAngle>& trace_vector, float distance);
  static void BuildBezier(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance);
  static void BuildBezierFast(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance);
  static void BuildBezier(float init_x[], float init_y[], int init_count, std::vector<FPointAngle>& trace_vector, float distance);
  static void BuildCircle(float center_x, float center_y, float radius, FPoint* fish_pos, int fish_count);
  static void BuildCircle(float center_x, float center_y, float radius, FPointAngle* fish_pos, int fish_count, float rotate = 0.f, float rotate_speed = 0.f);
};

}
#endif // MATH_AIDE_H_
