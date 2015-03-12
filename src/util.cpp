#include "util.hpp"

namespace otto {

float regularPolyRadius(float sideLen, uint32_t numSides) {
  return sideLen / (2.0f * std::sin(M_PI / numSides));
}

// Interpolate between two angles, assuming both angles are in the range 0-2pi.
float lerpAngular(float angle, float targetAngle, float t) {
  auto angleDiff = std::abs(targetAngle - angle);
  if (std::abs(angle - (targetAngle + TWO_PI)) < angleDiff) {
    targetAngle += TWO_PI;
  }
  return angle + (targetAngle - angle) * t;
}

void AngularParticle::step() {
  auto vel = (angle - anglePrev) * (1.0f - friction);

  anglePrev = angle;
  angle = angle + vel;

  // Wrap angle to the range 0-2pi. Assumes angle is not less than -2pi.
  auto wrappedAngle = std::fmod(angle + TWO_PI, TWO_PI);
  if (wrappedAngle != angle) {
    anglePrev += wrappedAngle - angle;
    angle = wrappedAngle;
  }
}

void AngularParticle::lerp(float targetAngle, float t) {
  angle = lerpAngular(angle, targetAngle, t);
}

std::pair<std::string, std::string> formatMebibytes(uint64_t bytes) {
  static const std::string suffixes[] = { "B", "KiB", "MiB", "GiB" };
  uint64_t place = std::log(static_cast<double>(bytes)) / std::log(1024.0);
  uint64_t units = std::round(bytes / std::pow(1024.0, place));
  return { std::to_string(units), suffixes[std::min(place, 3ull)] };
}

} // otto
