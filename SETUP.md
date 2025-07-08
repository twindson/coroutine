# Hg-Watchman Integration Setup Guide

This guide provides detailed instructions for setting up the integration between Mercurial (hg) and Watchman.

## Installation

### Installing Mercurial

**On Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install mercurial
```

**On macOS:**
```bash
brew install mercurial
```

**On Windows:**
Download from https://www.mercurial-scm.org/downloads

### Installing Watchman

**On Ubuntu/Debian:**
```bash
# Install dependencies
sudo apt-get install build-essential python3-dev libssl-dev libffi-dev

# Download and install watchman
git clone https://github.com/facebook/watchman.git
cd watchman
./autogen.sh
./configure
make
sudo make install
```

**On macOS:**
```bash
brew install watchman
```

**On Windows:**
Download from https://facebook.github.io/watchman/docs/install.html

## Configuration

### 1. Initialize Mercurial Repository

```bash
mkdir my-project
cd my-project
hg init
```

### 2. Copy Configuration Files

Copy the following files from this repository:
- `.watchmanconfig` - Watchman configuration
- `.hgignore` - Mercurial ignore patterns
- `scripts/` - Example automation scripts

### 3. Set Up Watchman

```bash
# Start watching the current directory
watchman watch .

# Verify watchman is watching
watchman watch-list
```

## Common Workflows

### 1. Continuous Building

Set up a trigger to build automatically when source files change:

```bash
watchman -- trigger . build-on-change \
  --append-files \
  --expression-language python \
  --stdout=log \
  --stderr=log \
  '*.cpp' '*.h' '*.c' \
  -- ./scripts/build.sh
```

### 2. Continuous Testing

Run tests automatically when code changes:

```bash
watchman -- trigger . test-on-change \
  --append-files \
  --stdout=log \
  --stderr=log \
  '*.cpp' '*.h' '*.test' \
  -- ./scripts/run-tests.sh
```

### 3. Development Auto-commit

**Warning: Use carefully, primarily for development branches**

```bash
watchman -- trigger . auto-commit \
  --append-files \
  --stdout=log \
  --stderr=log \
  '*.cpp' '*.h' '*.py' '*.js' \
  -- ./scripts/auto-commit.sh
```

## Advanced Configuration

### Custom Trigger with Filters

```bash
# Only trigger on files in src/ directory
watchman -- trigger . src-only-build \
  --expression-language python \
  'allof ["match", "src/**", "wholename"] ["suffix", "cpp"]' \
  -- ./scripts/build.sh
```

### Excluding Directories

Modify `.watchmanconfig` to exclude additional directories:

```json
{
  "ignore_dirs": [
    ".hg",
    "node_modules",
    "build",
    "dist",
    "tmp",
    "custom_ignore_dir"
  ]
}
```

## Monitoring and Debugging

### Check Active Triggers

```bash
watchman trigger-list .
```

### Monitor File Changes

```bash
# Subscribe to file changes for debugging
watchman -- subscribe . debug-subscription \
  --fields name,exists,mode \
  '*.cpp' '*.h'
```

### View Logs

```bash
# Set debug logging
watchman log-level debug

# View logs (location varies by OS)
tail -f ~/.watchman/log
```

## Best Practices

1. **Start Simple**: Begin with basic build/test triggers
2. **Test Triggers**: Validate triggers with sample files before full deployment
3. **Monitor Performance**: Watch for excessive trigger frequency
4. **Use Specific Patterns**: Avoid overly broad file patterns
5. **Backup Before Auto-commit**: Always backup before using auto-commit workflows

## Troubleshooting

### Common Issues

1. **Watchman not starting**: Check system resources and permissions
2. **Triggers not firing**: Verify file patterns and expressions
3. **Performance issues**: Review ignore patterns and trigger frequency
4. **Permission errors**: Ensure scripts are executable

### Useful Commands

```bash
# Stop watching a directory
watchman watch-del .

# Clear all triggers
watchman trigger-del . trigger-name

# Restart watchman
watchman shutdown-server
```