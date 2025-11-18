# vcpkg and SFML Installation Guide (Windows)

Complete steps to install vcpkg and SFML on Windows.

## 1. Check if vcpkg is Already Installed

Open a terminal in VS Code and run:

```bash
vcpkg version
```

**If the version appears**: You already have it installed ✓

**If an error appears**: You need to install it ⬇️

## 2. Install vcpkg (if you don't have it)

```bash
# Navigate to a folder where you want to install it (e.g., C:\dev)
cd C:\
mkdir dev
cd dev

# Clone the repository
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Run the bootstrap
.\bootstrap-vcpkg.bat
```

## 3. Add vcpkg to PATH (optional but recommended)

```bash
# Copy the path where vcpkg is located, for example: C:\dev\vcpkg
# Then add it to the system environment variables
```

Or use the full path in each command.

## 4. Install SFML

```bash
# If vcpkg is in PATH:
vcpkg install sfml:x64-windows

# If not in PATH (use full path):
C:\dev\vcpkg\vcpkg install sfml:x64-windows
```

## 5. Integrate with Your Project

```bash
vcpkg integrate install

# Set the variable for this session
$env:VCPKG_ROOT = "C:\dev\vcpkg"

# Configure (this will activate the manifest and prepare SFML)
cmake --preset x64-windows

# Build
cmake --build --preset x64-windows
```

This command will give you a path to use in CMake.

## 6. Verify SFML Installation

### Option A: Using CMake Preset (Recommended)

```powershell
# Asegurar variable de entorno
$env:VCPKG_ROOT = "C:\dev\vcpkg"

# Configurar (esto verifica que encuentra SFML)
cmake --preset x64-windows
```

**What to look for in the output:**
- ✅ `-- Running vcpkg install - done` (vcpkg installed dependencies)
- ✅ `-- Found SFML 3.0.2 in ...` (CMake found SFML)
- ✅ `-- Configuring done` (configuration successful)
- ❌ If it says `Could not find SFML` → there's a problem

### Option B: Using vcpkg list

```bash
vcpkg list | findstr sfml
```

You should see something like: `sfml:x64-windows`

**Note:** When using manifest mode, this command may show "No packages are installed" from your project directory. This is normal - use Option A instead.

## Notes

- The project uses **manifest mode** with `vcpkg.json`, so packages are installed locally per project.
- When running `vcpkg list` from your project directory, packages may not appear if using manifest mode. Use `cmake --preset` to verify integration instead.
- `CMakePresets.json` automatically configures the vcpkg toolchain when you use the preset.