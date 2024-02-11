#pragma once

class FuzzySystem {
public:
	virtual int infer(
		const int left,
		const int right,
		const int left_angled,
		const int right_angled,
		const int speed,
		const int direction
	) const = 0;

	virtual ~FuzzySystem() {};
};
