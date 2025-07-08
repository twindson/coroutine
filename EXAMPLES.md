# Hg-Watchman Integration Examples

This file contains practical examples of how to integrate Mercurial (hg) with Watchman for various development workflows.

## Example 1: C++ Development Workflow

### Setup
```bash
# Initialize project
hg init cpp-project
cd cpp-project

# Set up watchman
watchman watch .

# Configure build trigger
watchman -- trigger . cpp-build \
  '*.cpp' '*.hpp' '*.h' \
  -- make clean && make
```

### File Structure
```
cpp-project/
├── .hg/
├── .watchmanconfig
├── .hgignore
├── Makefile
├── src/
│   ├── main.cpp
│   └── utils.hpp
└── tests/
    └── test_main.cpp
```

## Example 2: Web Development with Auto-reload

### Setup
```bash
# Set up triggers for web development
watchman -- trigger . web-build \
  --append-files \
  '*.js' '*.css' '*.html' \
  -- npm run build

watchman -- trigger . web-reload \
  --append-files \
  '*.js' '*.css' '*.html' \
  -- curl -X POST http://localhost:3000/reload
```

## Example 3: Python Development with Testing

### Setup
```bash
# Python project with automatic testing
watchman -- trigger . python-test \
  --append-files \
  --expression-language python \
  'allof ["suffix", "py"] ["not", ["match", "__pycache__/**", "wholename"]]' \
  -- python -m pytest tests/

# Linting trigger
watchman -- trigger . python-lint \
  --append-files \
  '*.py' \
  -- flake8 src/
```

## Example 4: Documentation Generation

### Setup
```bash
# Auto-generate documentation when markdown files change
watchman -- trigger . docs-build \
  --append-files \
  '*.md' '*.rst' \
  -- mkdocs build

# Commit documentation changes
watchman -- trigger . docs-commit \
  --append-files \
  'docs/*.html' \
  -- hg add docs/ && hg commit -m "Auto-update documentation"
```

## Example 5: Multi-language Project

### Setup
```bash
# Different triggers for different languages
watchman -- trigger . backend-build \
  'src/backend/*.go' \
  -- go build -o bin/backend src/backend/main.go

watchman -- trigger . frontend-build \
  'src/frontend/*.js' 'src/frontend/*.vue' \
  -- npm run build:frontend

watchman -- trigger . api-test \
  'api/*.py' \
  -- python -m pytest api/tests/
```

## Example 6: Advanced Trigger with Conditions

### Setup with Complex Expression
```bash
# Only trigger during business hours (requires additional scripting)
watchman -- trigger . business-hours-build \
  --expression-language python \
  'allof [
    "suffix", "cpp",
    "not", ["match", "test/**", "wholename"],
    "not", ["match", "vendor/**", "wholename"]
  ]' \
  -- ./scripts/conditional-build.sh
```

### Conditional Script (scripts/conditional-build.sh)
```bash
#!/bin/bash
# Only build during business hours (9 AM to 5 PM)
hour=$(date +%H)
if [ $hour -ge 9 ] && [ $hour -lt 17 ]; then
    echo "Building during business hours..."
    make build
else
    echo "Outside business hours, skipping build"
fi
```

## Example 7: Git-Hg Bridge with Watchman

### Setup for Mixed VCS Environment
```bash
# Watch for changes and sync between git and hg repositories
watchman -- trigger . git-to-hg-sync \
  --append-files \
  '*.cpp' '*.h' '*.py' '*.js' \
  -- ./scripts/sync-git-to-hg.sh
```

### Sync Script (scripts/sync-git-to-hg.sh)
```bash
#!/bin/bash
# Sync changes from git to hg repository
if [ -d "../hg-mirror" ]; then
    rsync -av --exclude='.git' --exclude='.hg' ./ ../hg-mirror/
    cd ../hg-mirror
    hg addremove
    hg commit -m "Sync from git: $(date)"
    cd -
fi
```

## Example 8: Performance Optimized Configuration

### Optimized .watchmanconfig
```json
{
  "root_restrict_files": [".hg"],
  "settle": 100,
  "fsevents_latency": 0.1,
  "ignore_dirs": [
    ".hg", "node_modules", "build", "dist", "tmp",
    ".git", "__pycache__", "*.egg-info",
    "target", "vendor", ".idea", ".vscode"
  ],
  "ignore_vcs": [".hg", ".git"],
  "enforce_root_files": true
}
```

## Example 9: Debugging and Monitoring

### Setup Monitoring Triggers
```bash
# Log all file changes for debugging
watchman -- trigger . debug-logger \
  --append-files \
  --stdout=debug.log \
  '**' \
  -- echo "File changed: $WATCHMAN_FILES at $(date)"

# Monitor trigger execution
watchman -- trigger . execution-monitor \
  --append-files \
  --stdout=execution.log \
  --stderr=execution.log \
  '*.trigger' \
  -- ./scripts/monitor-execution.sh
```

## Tips for Effective Integration

1. **Use specific file patterns** to avoid unnecessary triggers
2. **Test triggers with sample files** before full deployment
3. **Monitor system resources** during heavy file operations
4. **Use .hgignore appropriately** to exclude build artifacts
5. **Consider using settlement delays** for batch operations
6. **Implement proper error handling** in trigger scripts
7. **Log trigger executions** for debugging purposes