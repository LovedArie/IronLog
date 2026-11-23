// SampleData.h — a realistic demo history, so a fresh install has
// something to look at (charts, PRs, weekly volume) in one click.
#pragma once

#include "core/Date.h"
#include "core/Workout.h"

namespace ironlog::sample {

// Eight weeks of a 5-day push/pull/legs split ending on `today`, with
// progressive overload (+2.5 kg per week on compounds) and one off-day so
// the progress chart isn't a perfectly straight line. Deterministic: the
// same `today` always gives the same log, which makes it testable.
TrainingLog demoLog(Date today);

} // namespace ironlog::sample
