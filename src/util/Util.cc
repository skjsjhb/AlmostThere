#include "Util.hh"

#include <algorithm>
#include <functional>
#include <cctype>
#include <codecvt>
#include <locale>

glm::vec4 parseColorStr(const std::string &colorIn) {
  if (colorIn.empty()) {
    return {1, 1, 1, 1};
  }
  auto cs = splitStr(colorIn, ",");
  if (cs.size() == 1) {
    auto gray = std::stof(cs[0]);
    return {gray, gray, gray, 1};
  }
  glm::vec4 cl = {1, 1, 1, 1};
  if (cs.size() == 3) {
    cs.emplace_back("255"); // Alpha
  }
  if (cs.size() == 4) {
    for (int i = 0; i < 4; ++i) {
      cl[i] = std::stof(cs[i]) / 255.0f;
    }
  }
  return cl;
}

std::wstring unicode2wstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.from_bytes(str);
}

std::string wstring2unicode(const std::wstring &wstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.to_bytes(wstr);
}

bool isOverlapped(double p1, double r1, double p2, double r2) {
  return std::abs(p2 - p1) <= std::abs(r1) + std::abs(r2);
}

std::vector<std::string> splitStr(const std::string &src, const std::string &del, int maxn) {
  if (maxn <= 1) {
    return std::vector({src});
  }
  size_t pos_start = 0, pos_end, delim_len = del.length(), split_cnt = 0;
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = src.find(del, pos_start)) != std::string::npos) {
    token = src.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
    split_cnt++;
    if ((int) split_cnt >= maxn - 1) {
      break;
    }
  }
  res.push_back(src.substr(pos_start));
  return res;
}

void trimStr(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](unsigned char c) -> bool { return !std::isspace(c); }));
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char c) -> bool { return !std::isspace(c); })
              .base(),
          s.end());
}

