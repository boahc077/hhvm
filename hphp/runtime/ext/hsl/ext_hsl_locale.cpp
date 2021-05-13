/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/array-init.h"
#include "hphp/runtime/base/locale.h"
#include "hphp/runtime/base/thread-safe-setlocale.h"
#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/vm/native.h"
#include "hphp/runtime/vm/native-data.h"
#include "hphp/system/systemlib.h"

namespace HPHP {
namespace {

const StaticString
  s_HSLLocale("HSLLocale"),
  s_FQHSLLocale("HH\\Lib\\_Private\\_Locale\\Locale");

Class* s_HSLLocaleClass = nullptr;

} // namespace

struct HSLLocale {
  HSLLocale() = default;
  explicit HSLLocale(std::shared_ptr<Locale> loc): m_locale(loc) {}

  ~HSLLocale() {
    sweep();
  }

  void sweep() {
    m_locale.reset();
  }

  std::shared_ptr<Locale> get() {
    return m_locale;
  }

  static Object newInstance(std::shared_ptr<Locale> loc) {
    assertx(s_HSLLocaleClass);
    Object obj { s_HSLLocaleClass };
    auto* data = Native::data<HSLLocale>(obj);
    new (data) HSLLocale(loc);
    return obj;
  }

  Array __debugInfo() const {
    if(!m_locale) {
      raise_fatal_error("Locale is null");
    }
    Array ret(Array::CreateDict());
    for (const auto& [category, locale] : m_locale->getAllCategoryLocaleNames()) {
      ret.set(
        String(category.data(), category.size(), CopyString),
        String(locale.data(), locale.size(), CopyString)
      );
    }
    return ret;
  }

  static std::shared_ptr<Locale> get(const Object& obj) {
    if (obj.isNull()) {
      raise_typehint_error("Expected an HSL Locale, got null");
    }
    if (!obj->instanceof(s_FQHSLLocale)) {
      raise_typehint_error(
        folly::sformat(
          "Expected an HSL Locale, got instance of class '{}'",
          obj->getClassName().c_str()
        )
      );
    }
    return Native::data<HSLLocale>(obj)->get();
  }
 private:
  std::shared_ptr<Locale> m_locale;
};

namespace {

Array HHVM_METHOD(HSLLocale, __debugInfo) {
  return Native::data<HSLLocale>(this_)->__debugInfo();
}

Object HHVM_FUNCTION(get_c_locale) {
  return HSLLocale::newInstance(Locale::getCLocale());
}

Object HHVM_FUNCTION(get_environment_locale) {
  return HSLLocale::newInstance(Locale::getEnvLocale());
}

Object HHVM_FUNCTION(get_request_locale) {
  return HSLLocale::newInstance(ThreadSafeLocaleHandler::getRequestLocale());
}

void HHVM_FUNCTION(set_request_locale, const Object& locale) {
  ThreadSafeLocaleHandler::setRequestLocale(HSLLocale::get(locale));
}

Object HHVM_FUNCTION(newlocale_mask,
                     int64_t mask,
                     const String& locale,
                     const Object& base) {
  auto loc = HSLLocale::get(base)->newlocale(LocaleCategoryMask, mask, locale.c_str());
  return HSLLocale::newInstance(loc);
}

Object HHVM_FUNCTION(newlocale_category,
                     int64_t category,
                     const String& locale,
                     const Object& base) {
  auto loc = HSLLocale::get(base)->newlocale(LocaleCategory, category, locale.c_str());
  return HSLLocale::newInstance(loc);
}

struct LocaleExtension final : Extension {

  LocaleExtension() : Extension("hsl_locale", "0.1") {}

  void moduleInit() override {
    // Remember to update the HHI :)

    Native::registerNativeDataInfo<HSLLocale>(s_HSLLocale.get());
    HHVM_NAMED_ME(HH\\Lib\\_Private\\_Locale\\Locale, __debugInfo, HHVM_MN(HSLLocale, __debugInfo));

    HHVM_FALIAS(HH\\Lib\\_Private\\_Locale\\get_c_locale, get_c_locale);
    HHVM_FALIAS(HH\\Lib\\_Private\\_Locale\\get_environment_locale, get_environment_locale);
    HHVM_FALIAS(HH\\Lib\\_Private\\_Locale\\get_request_locale, get_request_locale);
    HHVM_FALIAS(HH\\Lib\\_Private\\_Locale\\set_request_locale, set_request_locale);
    HHVM_FALIAS(HH\\Lib\\_Private\\_Locale\\newlocale_mask, newlocale_mask);
    HHVM_FALIAS(HH\\Lib\\_Private\\_Locale\\newlocale_category, newlocale_category);

#define LC_(x) \
    HHVM_RC_INT(HH\\Lib\\_Private\\_Locale\\LC_##x, LC_##x); \
    HHVM_RC_INT(HH\\Lib\\_Private\\_Locale\\LC_##x##_MASK, LC_##x##_MASK);
    LC_(ALL);
    LC_(COLLATE);
    LC_(CTYPE);
    LC_(MONETARY);
    LC_(NUMERIC);
    LC_(TIME);
#if defined(__APPLE__) || defined(__GLIBC__)
    LC_(MESSAGES);
#endif
#ifdef __GLIBC__
    LC_(PAPER);
    LC_(NAME);
    LC_(ADDRESS);
    LC_(TELEPHONE);
    LC_(MEASUREMENT);
    LC_(IDENTIFICATION);
#endif
#undef LC_

    loadSystemlib();
    s_HSLLocaleClass = Class::lookup(s_FQHSLLocale.get());
    assertx(s_HSLLocaleClass);
  }
} s_locale_extension;

} // namespace
} // namespace HPHP