#!/bin/bash
set -euo pipefail

# ── Configuration ──────────────────────────────────────────────────
APP_NAME="CellularAutomata"
BUNDLE="${APP_NAME}.app"
BINARY="cellular_automata"
SFML_LIB="/opt/homebrew/opt/sfml@2/lib"
SFML_INC="/opt/homebrew/opt/sfml@2/include"
FREETYPE_LIB="/opt/homebrew/opt/freetype/lib"
LIBPNG_LIB="/opt/homebrew/opt/libpng/lib"
SRC_FILES="Main.cpp Grid.cpp RuleSet.cpp Cell.cpp Menu.cpp Settings.cpp"

echo "==> Building release for ${APP_NAME}..."

# ── 1. Compile with @rpath ─────────────────────────────────────────
echo "  Compiling..."
g++ -std=c++17 -O2 \
    -I"${SFML_INC}" \
    -L"${SFML_LIB}" \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -Wl,-rpath,@executable_path/../Frameworks \
    -o "${BINARY}" ${SRC_FILES}

# ── 2. Create .app bundle structure ────────────────────────────────
echo "  Creating ${BUNDLE}..."
rm -rf "${BUNDLE}"
mkdir -p "${BUNDLE}/Contents/MacOS"
mkdir -p "${BUNDLE}/Contents/Frameworks"
mkdir -p "${BUNDLE}/Contents/Resources/resources"

# ── 3. Copy binary ────────────────────────────────────────────────
cp "${BINARY}" "${BUNDLE}/Contents/MacOS/"

# ── 4. Copy resources ─────────────────────────────────────────────
cp resources/arial.ttf "${BUNDLE}/Contents/Resources/resources/"
# Include default settings if present
[ -f settings.txt ] && cp settings.txt "${BUNDLE}/Contents/Resources/"

# ── 5. Copy dylibs (SFML + transitive deps) ───────────────────────
DYLIBS=(
    "${SFML_LIB}/libsfml-graphics.2.6.dylib"
    "${SFML_LIB}/libsfml-window.2.6.dylib"
    "${SFML_LIB}/libsfml-system.2.6.dylib"
    "${FREETYPE_LIB}/libfreetype.6.dylib"
    "${LIBPNG_LIB}/libpng16.16.dylib"
)

for lib in "${DYLIBS[@]}"; do
    cp "$lib" "${BUNDLE}/Contents/Frameworks/"
done

# ── 6. Fix dylib install names (set id to @rpath) ─────────────────
FW="${BUNDLE}/Contents/Frameworks"

for lib in "${FW}"/*.dylib; do
    base=$(basename "$lib")
    # Set the library's own id
    install_name_tool -id "@rpath/${base}" "$lib" 2>/dev/null || true
done

# Fix the main binary: repoint SFML libs from homebrew paths to @rpath
install_name_tool -change "${SFML_LIB}/libsfml-graphics.2.6.dylib" "@rpath/libsfml-graphics.2.6.dylib" "${BUNDLE}/Contents/MacOS/${BINARY}"
install_name_tool -change "${SFML_LIB}/libsfml-window.2.6.dylib"   "@rpath/libsfml-window.2.6.dylib"   "${BUNDLE}/Contents/MacOS/${BINARY}"
install_name_tool -change "${SFML_LIB}/libsfml-system.2.6.dylib"   "@rpath/libsfml-system.2.6.dylib"   "${BUNDLE}/Contents/MacOS/${BINARY}"

# Fix SFML graphics → freetype, sfml-window, sfml-system
install_name_tool -change "${FREETYPE_LIB}/libfreetype.6.dylib"    "@rpath/libfreetype.6.dylib"    "${FW}/libsfml-graphics.2.6.dylib"
install_name_tool -change "@rpath/libsfml-window.2.6.dylib"        "@rpath/libsfml-window.2.6.dylib" "${FW}/libsfml-graphics.2.6.dylib" 2>/dev/null || true
install_name_tool -change "@rpath/libsfml-system.2.6.dylib"        "@rpath/libsfml-system.2.6.dylib" "${FW}/libsfml-graphics.2.6.dylib" 2>/dev/null || true

# Fix SFML window → sfml-system
install_name_tool -change "@rpath/libsfml-system.2.6.dylib"        "@rpath/libsfml-system.2.6.dylib" "${FW}/libsfml-window.2.6.dylib" 2>/dev/null || true

# Fix freetype → libpng
install_name_tool -change "${LIBPNG_LIB}/libpng16.16.dylib"        "@rpath/libpng16.16.dylib"      "${FW}/libfreetype.6.dylib"

# Add rpath to each bundled dylib so they can find each other
for lib in "${FW}"/*.dylib; do
    install_name_tool -add_rpath "@loader_path" "$lib" 2>/dev/null || true
done

# ── 7. Create launcher script (sets cwd to Resources) ─────────────
cat > "${BUNDLE}/Contents/MacOS/launch.sh" << 'LAUNCHER'
#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${SCRIPT_DIR}/../Resources"
exec "${SCRIPT_DIR}/cellular_automata"
LAUNCHER
chmod +x "${BUNDLE}/Contents/MacOS/launch.sh"

# ── 8. Create Info.plist ──────────────────────────────────────────
cat > "${BUNDLE}/Contents/Info.plist" << PLIST
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
  "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleName</key>
    <string>${APP_NAME}</string>
    <key>CFBundleDisplayName</key>
    <string>Cellular Automata</string>
    <key>CFBundleIdentifier</key>
    <string>com.alexhugli.cellular-automata</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0.0</string>
    <key>CFBundleExecutable</key>
    <string>launch.sh</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>LSMinimumSystemVersion</key>
    <string>12.0</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
PLIST

# ── 9. Create DMG for distribution ────────────────────────────────
DMG_NAME="${APP_NAME}.dmg"
echo "  Creating ${DMG_NAME}..."
rm -f "${DMG_NAME}"
hdiutil create -volname "${APP_NAME}" -srcfolder "${BUNDLE}" -ov -format UDZO "${DMG_NAME}" -quiet

# ── Done ──────────────────────────────────────────────────────────
echo ""
echo "==> Release build complete!"
echo "    App bundle: ${BUNDLE}/"
echo "    Disk image: ${DMG_NAME}"
echo ""
echo "    To run directly: open ${BUNDLE}"
echo "    To distribute:   send ${DMG_NAME}"
