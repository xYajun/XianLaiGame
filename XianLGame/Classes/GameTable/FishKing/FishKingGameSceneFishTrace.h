#ifndef __HN_FishKing_SCENE_FISH_TRACE_H_
#define __HN_FishKing_SCENE_FISH_TRACE_H_

#include <vector>
#include "FishKingGameMathAide.h"

namespace FishKing
{

typedef std::vector<FPointAngle> TraceVector;

class SceneFishTrace 
{
public:
	SceneFishTrace();
	~SceneFishTrace();

public:
	TraceVector scene_kind_1_trace_[210];
	TraceVector scene_kind_2_trace_[200 + 7 * 2];
	TraceVector::size_type scene_kind_2_small_fish_stop_index_[200];
	TraceVector::size_type scene_kind_2_small_fish_stop_count_;
	TraceVector::size_type scene_kind_2_big_fish_stop_index_;
	TraceVector::size_type scene_kind_2_big_fish_stop_count_;
	TraceVector scene_kind_3_trace_[(50 + 40 + 30 + 1) * 2];
	TraceVector scene_kind_4_trace_[8 * 8];
	TraceVector scene_kind_5_trace_[(40 + 40 + 24 + 13 + 1) * 2];

	void BuildSceneKind1Trace(float screen_width, float screen_height);

	void BuildSceneKind2Trace(float screen_width, float screen_height);

	void BuildSceneKind3Trace(float screen_width, float screen_height);

	void BuildSceneKind4Trace(float screen_width, float screen_height);

	void BuildSceneKind5Trace(float screen_width, float screen_height);

	void GetTargetPoint(float screen_width, float screen_height, float src_x_pos, float src_y_pos, float angle, float& target_x_pos, float& target_y_pos);
};

}

#endif  // _FishKing_SCENE_FISH_TRACE_H_