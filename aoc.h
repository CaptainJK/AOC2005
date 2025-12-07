#pragma once

#include <cassert>
#include <format>
#include <memory>
#include <string>

#include <curl/curl.h>

template <typename T>
concept Day = requires(T t, const std::string_view &i) {
  { t.id() } -> std::same_as<size_t>;
  { t.problem1(i) } -> std::same_as<size_t>;
  { t.problem2(i) } -> std::same_as<size_t>;
};

struct AoC {
  static constexpr const char *inputBaseUrl =
      "https://adventofcode.com/2025/day/{}/input";

  explicit AoC(size_t day, const std::string_view &session)
      : _url{std::format(inputBaseUrl, day)},
        _cookie{std::format("session={}; ", session)} {

    static std::once_flag f;
    std::call_once(f, [] { curl_global_init(CURL_GLOBAL_ALL); });
  }

  std::string fetchInput() const {
    auto handle = std::unique_ptr<CURL, decltype(&curl_easy_cleanup)>(
        curl_easy_init(), curl_easy_cleanup);
    assert(handle && "curl initialization failed");

    CURL *curl = handle.get();

    std::string ret;
    curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(curl, CURLOPT_COOKIE, _cookie.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);

    CURLcode resp = curl_easy_perform(curl);
    assert(resp == CURLE_OK && "unexpected response");

    return ret;
  }

  static size_t _writeFunc(char *ptr, size_t size, size_t nmemb, void *dst) {
    std::copy_n(ptr, size * nmemb,
                std::back_inserter(*reinterpret_cast<std::string *>(dst)));
    return size * nmemb;
  }

  const std::string _url;
  const std::string _cookie;
};
