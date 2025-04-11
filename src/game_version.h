#ifndef GAME_VERSION_H
#define GAME_VERSION_H
#include <magic_enum/magic_enum.hpp>

namespace leprac {
#define CONCAT(a, b) a##b
#define EXPAND(...) __VA_ARGS__

#define LE(...) EXPAND(LE_HELPER(1, __VA_ARGS__))
#define LE_HELPER(cnt, name, ...) name = cnt __VA_OPT__(,) LE_HELPER(cnt+1, __VA_ARGS__))

#define PATCH(...) EXPAND(PATCH_HELPER(1, __VA_ARGS__))
#define PATCH_HELPER(cnt, name, ...) name = (cnt << 3) __VA_OPT__(,) PATCH_HELPER(cnt+1, __VA_ARGS__))

#define COMBINE(base, ...) EXPAND(COMBINE_HELPER(base, __VA_ARGS__))
#define COMBINE_HELPER(base, patch, ...)                                       \
CONCAT(base, _## patch) = base | patch \
__VA_OPT__(,) COMBINE_HELPER(base, __VA_ARGS__))

  enum class GameVersion {
    UNKNOWN = 0,

    LE(Le01, Le02, Le03, Le04)

    PATCH(v1_00a, v1_03a, v1_10a, v1_11a, v1_20a, v1_21a, v1_20f)

    COMBINE(Le01, v1_00a, v1_10a, v1_11a, v1_20a),
    COMBINE(Le02, v1_00a, v1_10a, v1_11a, v1_20a),
    COMBINE(Le03, v1_00a, v1_10a, v1_11a, v1_20a, v1_21a)
  };

  // 验证数值自动生成
  static_assert(static_cast<int>(GameVersion::Le03) == 3);
  static_assert(static_cast<int>(GameVersion::v1_20a) == 5 << 3);
  static_assert(static_cast<int>(GameVersion::Le01_1_00a) == (1 | 8));


} // namespace leprac

#endif // GAME_VERSION_H
