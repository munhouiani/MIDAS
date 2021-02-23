// -----------------------------------------------------------------------------
// Copyright 2020 Rui Liu (liurui39660) and Siddharth Bhatia (bhatiasiddharth)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// 	http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// -----------------------------------------------------------------------------

#pragma once

#include <cmath>

#include "CountMinSketch.hpp"

namespace MIDAS {
struct NormalCore {
	unsigned long timestamp = 1;
	unsigned long* const index; // Pre-compute the index to-be-modified, thanks to the same structure of CMSs
	CountMinSketch numCurrent, numTotal;

	NormalCore(unsigned long numRow, unsigned long numColumn):
		index(new unsigned long[numRow]),
		numCurrent(numRow, numColumn),
		numTotal(numCurrent) { }

	virtual ~NormalCore() {
		delete[] index;
	}

	static float ComputeScore(double a, double s, double t) {
		return s == 0 || t - 1 == 0 ? 0 : pow((a - s / t) * t, 2) / (s * (t - 1));
	}

	float operator()(unsigned long source, unsigned long destination, unsigned long timestamp) {
		if (this->timestamp < timestamp) {
			numCurrent.ClearAll();
			this->timestamp = timestamp;
		}
		numCurrent.Hash(index, source, destination);
		numCurrent.Add(index);
		numTotal.Add(index);
		return ComputeScore(numCurrent(index), numTotal(index), timestamp);
	}
};
}
