# KRunner Translator Plugin

## Project Overview
A KRunner plugin for KDE Plasma 6 that translates text using multiple translation engines. Users type a language code (or source-target pair) followed by text in KRunner to get translations.

**Query syntax:** `<lang> text` or `<source>-<target> text` (e.g., `de hello` or `en-fr hello`)

## Build System
- **Framework:** KDE Frameworks 6 (KF6), Qt 6, ECM 6.0+
- **Build:** `mkdir build && cd build && cmake .. && make`
- **Test:** `cd build && ctest` or run `./bin/translator_test`
- **Install:** `cd build && sudo make install`
- Plugins install to `kf6/krunner/` and `kf6/krunner/kcms/` namespaces via `kcoreaddons_add_plugin`

## Architecture

### Three build targets
1. **krunner_translator** - the main runner plugin (.so)
2. **kcm_krunner_translator** - KCM config module for runner settings
3. **translator_test** - test executable

### Translation engines
- **GoogleTranslate** and **Bing** - command-line engines using `trans` (translate-shell) via `TranslateShellProcess`
- **Baidu** and **Youdao** - network API engines using `QNetworkAccessManager` with async HTTP requests
- All engines implement `CommandLineEngine` interface (despite the name, Baidu/Youdao are API-based)

### Key classes
- `Translator` (`src/Translator.cpp`) - main AbstractRunner subclass, orchestrates matching
- `Parser` (`src/Parser.cpp`) - parses query string into `TranslationQuery` (language pair + text)
- `TranslationQuery` (`src/TranslationQuery.cpp`) - holds source/target language and text
- `TranslateShellProcess` (`src/TranslateShellProcess.cpp`) - wraps `trans` CLI tool
- `LanguageRepository` (`src/language/LanguageRepository.cpp`) - maps abbreviations to `SupportedLanguage` enum
- `TranslatorConfig` (`src/config/translator_config.cpp`) - KCModule for settings UI

### Key files
- `src/plasma-runner-translator.json` - runner plugin metadata
- `src/config/kcm_krunner_translator.json` - KCM plugin metadata
- `src/config/config_keys.h` - shared config key constants
- `src/config/translator_config.ui` - Qt Designer UI for config dialog
- `src/language/SupportedLanguages.h` - enum of all supported languages

## KF6/Qt6 Conventions
- Uses `KRunner::` namespace (not `Plasma::`)
- Plugin registered via `K_PLUGIN_CLASS_WITH_JSON` in .cpp files
- Actions use `KRunner::Action` (not `QAction`)
- Match relevance via `setCategoryRelevance()` (not `setType()`)
- Must use `QStringLiteral()` for all string literals (`QT_NO_CAST_FROM_ASCII`)
- Must use `Q_EMIT`/`Q_SLOTS` (not `emit`/`slots` - `QT_NO_KEYWORDS`)
- `QByteArray` <-> `QString` conversions must be explicit (`.toUtf8()`, `QString::fromUtf8()`)
- KCModule is QObject-based; UI goes on `widget()`, use `setNeedsSave(true)` instead of `changed()`
- Runner JSON must have `"X-KDE-ConfigModule": "kf6/krunner/kcms/<kcm_target_name>"` for the config button to appear
- KCM JSON `"X-KDE-ParentComponents"` must match the runner's CMake target name (e.g., `["krunner_translator"]`)
