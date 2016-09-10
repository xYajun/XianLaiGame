#include "FishKingGameMathAide.h"
#include "math.h"


namespace FishKing
{

MathAide::MathAide() {
}

int MathAide::Factorial(int number) {
  int factorial = 1;
  int temp = number;
  for (int i = 0; i < number; ++i) {
    factorial *= temp;
    --temp;
  }

  return factorial;
}

int MathAide::Combination(int count, int r) {
  return Factorial(count) / (Factorial(r) * Factorial(count - r));
}

float MathAide::CalcDistance(float x1, float y1, float x2, float y2) {
  return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//以Y轴正方向为0方向
float MathAide::CalcAngle(float x1, float y1, float x2, float y2) 
{
	float distance = CalcDistance(x1, y1, x2, y2);
	if (distance == 0.f) return 0.f;
	float sin_value = (x1 - x2) / distance;
	float angle = acosf(sin_value);
	if (y1 > y2) angle= 2 * M_PI - angle;
	angle += M_PI_2;
	return angle;
}

//以X轴正方向为0方向
float MathAide::CalcAngle1(float x1, float y1, float x2, float y2) {
	float distance = CalcDistance(x1, y1, x2, y2);
	if (distance == 0.f) return 0.f;
	float sin_value = (x1 - x2) / distance;
	float angle = acosf(sin_value);
	if (y1 < y2)
		angle= - angle;
	return angle;
}

void MathAide::BuildLinear(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance) 
{
	trace_vector.clear();

	if (init_count < 2) 
		return;
	if (distance <= 0.0f) 
	{
		return;
	}

	float distance_total = CalcDistance(init_x[init_count - 1], init_y[init_count - 1], init_x[0], init_y[0]);
	if (distance_total <= 0.0f) 
	{
		return;
	}

	float cos_value = abs(init_y[init_count - 1] - init_y[0]) / distance_total;
	float angle = acosf(cos_value);

	FPoint point;
	point.x = init_x[0];
	point.y = init_y[0];
	trace_vector.push_back(point);
	float temp_distance = 0.f;

	std::vector<FPoint>::size_type size;
	while (temp_distance < distance_total) 
	{
		size = trace_vector.size();

		if (init_x[init_count - 1] < init_x[0]) 
		{
			point.x = init_x[0] - sinf(angle) * (distance * size);
		} 
		else 
		{
			point.x = init_x[0] + sinf(angle) * (distance * size);
		}

		if (init_y[init_count - 1] < init_y[0]) 
		{
			point.y = init_y[0] - cosf(angle) * (distance * size);
		} 
		else 
		{
			point.y = init_y[0] + cosf(angle) * (distance * size);
		}

		trace_vector.push_back(point);
		temp_distance = CalcDistance(point.x, point.y, init_x[0], init_y[0]);
	}

	FPoint& temp_point = trace_vector.back();
	temp_point.x = init_x[init_count - 1];
	temp_point.y = init_y[init_count - 1];
}

void MathAide::BuildLinear(float init_x[], float init_y[], int init_count, std::vector<FPointAngle>& trace_vector, float distance) {
  trace_vector.clear();

  if (init_count < 2) return;
  if (distance <= 0.0f) return;

  float distance_total = CalcDistance(init_x[init_count - 1], init_y[init_count - 1], init_x[0], init_y[0]);
  if (distance_total <= 0.0f) return;

  float cos_value = abs(init_y[init_count - 1] - init_y[0]) / distance_total;
  float temp_angle = acosf(cos_value);

  FPointAngle point;
  point.x = init_x[0];
  point.y = init_y[0];
  point.angle = 1.f;
  trace_vector.push_back(point);
  float temp_distance = 0.f;

  FPointAngle temp_pos = { 0.f, 0.f, 0.f };

  std::vector<FPointAngle>::size_type size;
  while (temp_distance < distance_total) {
    size = trace_vector.size();

    if (init_x[init_count - 1] < init_x[0]) {
      point.x = init_x[0] - sinf(temp_angle) * (distance * size);
    } else {
      point.x = init_x[0] + sinf(temp_angle) * (distance * size);
    }

    if (init_y[init_count - 1] < init_y[0]) {
      point.y = init_y[0] - cosf(temp_angle) * (distance * size);
    } else {
      point.y = init_y[0] + cosf(temp_angle) * (distance * size);
    }

    float temp_dis = CalcDistance(point.x, point.y, temp_pos.x, temp_pos.y);
    if (temp_dis != 0.f) {
      float temp_value = (point.x - temp_pos.x) / temp_dis;
      if ((point.y - temp_pos.y) >= 0.f) point.angle = acosf(temp_value);
      else point.angle = -acosf(temp_value);
    } else {
      point.angle = 1.f;
    }

    temp_pos.x = point.x;
    temp_pos.y = point.y;
    trace_vector.push_back(point);
    temp_distance = CalcDistance(point.x, point.y, init_x[0], init_y[0]);
  }

  FPointAngle& temp_point = trace_vector.back();
  temp_point.x = init_x[init_count - 1];
  temp_point.y = init_y[init_count - 1];
}

void MathAide::BuildBezier(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance) {
  trace_vector.clear();

  int index = 0;
  FPoint temp_pos = { 0.f, 0.f };
  float t = 0.f;
  int count = init_count - 1;
  float temp_distance = distance;
  float temp_value = 0.f;

  while (t <= 1.0f) {
    temp_pos.x = 0.f;
    temp_pos.y = 0.f;
    index = 0;
    while (index <= count) {
      temp_value = pow(t, index) * pow(1.f - t, count - index) * Combination(count, index);
      temp_pos.x += init_x[index] * temp_value;
      temp_pos.y += init_y[index] * temp_value;
      ++index;
    }
    
    float pos_space = 0.f;
    if (trace_vector.size() > 0) {
      FPoint& back_pos = trace_vector.back();
      pos_space = CalcDistance(back_pos.x, back_pos.y, temp_pos.x, temp_pos.y);
    }

    if (pos_space >= temp_distance || trace_vector.size() == 0) {
      trace_vector.push_back(temp_pos);
    }

    t += 0.00001f;
  }
}

void MathAide::BuildBezierFast(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance) {
  trace_vector.clear();

  int index = 0;
  FPoint temp_pos = { 0.f, 0.f };
  float t = 0.f;
  int count = init_count - 1;
  float temp_distance = distance;
  float temp_value = 0.f;

  while (t <= 1.0f) {
    temp_pos.x = 0.f;
    temp_pos.y = 0.f;
    index = 0;
    while (index <= count) {
      temp_value = pow(t, index) * pow(1.f - t, count - index) * Combination(count, index);
      temp_pos.x += init_x[index] * temp_value;
      temp_pos.y += init_y[index] * temp_value;
      ++index;
    }

    float pos_space = 0.f;
    if (trace_vector.size() > 0) {
      FPoint& back_pos = trace_vector.back();
      pos_space = CalcDistance(back_pos.x, back_pos.y, temp_pos.x, temp_pos.y);
    }

    if (pos_space >= temp_distance || trace_vector.size() == 0) {
      trace_vector.push_back(temp_pos);
    }

    t += 0.01f;
  }
}

void MathAide::BuildBezier(float init_x[], float init_y[], int init_count, std::vector<FPointAngle>& trace_vector, float distance) {
  trace_vector.clear();
  FPointAngle pos1 = { init_x[0], init_y[0], 1.f };
  trace_vector.push_back(pos1);

  int index = 0;
  FPointAngle temp_pos0 = { 0.f, 0.f };
  float t = 0.f;
  int count = init_count - 1;
  float temp_distance = distance;
  FPointAngle temp_pos = { 0.f, 0.f };
  float temp_value = 0.f;

  while (t < 1.0f) {
    temp_pos.x = 0.f;
    temp_pos.y = 0.f;
    index = 0;
    while (index <= count) {
      temp_value = pow(t, index) * pow(1.f - t, count - index) * Combination(count, index);
      temp_pos.x += init_x[index] * temp_value;
      temp_pos.y += init_y[index] * temp_value;
      ++index;
    }

    float pos_space = 0.f;
    if (trace_vector.size() > 0) {
      FPointAngle& back_pos = trace_vector.back();
      pos_space = CalcDistance(back_pos.x, back_pos.y, temp_pos.x, temp_pos.y);
    }

    if (pos_space >= temp_distance || trace_vector.size() == 0) {
      if (trace_vector.size() > 0) {
        float temp_dis = CalcDistance(temp_pos.x, temp_pos.y, temp_pos0.x, temp_pos0.y);
        if (temp_dis != 0.f) {
          float temp_value = (temp_pos.x - temp_pos0.x) / temp_dis;
          if ((temp_pos.y - temp_pos0.y) >= 0.f) temp_pos.angle = acosf(temp_value);
          else temp_pos.angle = -acosf(temp_value);
        } else {
          temp_pos.angle = 1.f;
        }
      } else {
        temp_pos.angle = 1.f;
      }
      trace_vector.push_back(temp_pos);
      temp_pos0.x = temp_pos.x;
      temp_pos0.y = temp_pos.y;
    }

    t += 0.00001f;
  }
}

void MathAide::BuildCircle(float center_x, float center_y, float radius, FPoint* fish_pos, int fish_count) {
  assert(fish_count > 0);
  if (fish_count <= 0) return;
  float cell_radian = 2 * M_PI / fish_count;

  // x = xo + r * cos(α)
  // y = yo + r * sin(α)
  for (int i = 0; i < fish_count; ++i) {
    fish_pos[i].x = center_x + radius * cosf(i * cell_radian);
    fish_pos[i].y = center_y + radius * sinf(i * cell_radian);
  }
}

void MathAide::BuildCircle(float center_x, float center_y, float radius, FPointAngle* fish_pos, int fish_count, float rotate, float rotate_speed) {
 assert(fish_count > 0);
  if (fish_count <= 0) return;
  float cell_radian = 2 * M_PI / fish_count;

  FPoint last_pos;
  // 这里计算好像有问题
  for (int i = 0; i < fish_count; ++i) {
    last_pos.x = center_x + radius * cosf(i * cell_radian + rotate - rotate_speed);
    last_pos.y = center_y + radius * sinf(i * cell_radian + rotate - rotate_speed);

    fish_pos[i].x = center_x + radius * cosf(i * cell_radian + rotate);
    fish_pos[i].y = center_y + radius * sinf(i * cell_radian + rotate);
    float temp_dis = CalcDistance(fish_pos[i].x, fish_pos[i].y, last_pos.x, last_pos.y);
    if (temp_dis != 0.f) {
      float temp_value = (fish_pos[i].x - last_pos.x) / temp_dis;
      if ((fish_pos[i].y - last_pos.y) >= 0.f) {
        fish_pos[i].angle = acosf(temp_value);
      } else {
        fish_pos[i].angle = -acosf(temp_value);
      }
    } else {
      fish_pos[i].angle = M_PI_2;
    }
  }

}

}
