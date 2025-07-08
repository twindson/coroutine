# Hg (Mercurial) + Watchman Integration

This repository demonstrates how to combine hg (Mercurial) and watchman for efficient file watching and automated workflows.

## Overview

Watchman is a file watching service that can monitor file system changes and trigger actions. When combined with Mercurial (hg), it enables powerful automation workflows such as:

- Automatic builds when source files change
- Continuous testing on file modifications
- Auto-commit workflows
- Linting and formatting on save

## Prerequisites

- [Mercurial (hg)](https://www.mercurial-scm.org/) installed
- [Watchman](https://facebook.github.io/watchman/) installed
- Basic understanding of both tools

## Quick Start

1. **Initialize a Mercurial repository:**
   ```bash
   hg init my-project
   cd my-project
   ```

2. **Set up watchman configuration:**
   ```bash
   # Copy the .watchmanconfig to your project
   cp .watchmanconfig ./
   ```

3. **Start watching your repository:**
   ```bash
   watchman watch .
   ```

4. **Set up triggers (see examples below)**

## Configuration Files

### .watchmanconfig
The `.watchmanconfig` file configures watchman behavior for the project.

### .hgignore
Mercurial ignore file that excludes watchman and other temporary files.

## Integration Examples

### Example 1: Auto-build on Source Changes

```bash
# Watch for changes in source files and trigger build
watchman -- trigger . build-trigger '*.cpp' '*.h' -- ./scripts/build.sh
```

### Example 2: Auto-test on Code Changes

```bash
# Run tests when source or test files change
watchman -- trigger . test-trigger '*.cpp' '*.h' '*.test' -- ./scripts/run-tests.sh
```

### Example 3: Auto-commit Development Changes

```bash
# Auto-commit changes during development (use with caution)
watchman -- trigger . auto-commit-trigger '*.cpp' '*.h' -- ./scripts/auto-commit.sh
```

## Best Practices

1. **Use specific file patterns** to avoid unnecessary triggers
2. **Test your triggers thoroughly** before deploying
3. **Consider performance impact** of frequent operations
4. **Use .hgignore** to exclude build artifacts and temporary files
5. **Backup your repository** before setting up auto-commit workflows

## Troubleshooting

- Check watchman logs: `watchman log-level debug`
- Verify triggers: `watchman trigger-list .`
- Monitor file changes: `watchman subscribe . mysub '*.cpp'`

## Scripts

See the `scripts/` directory for example automation scripts that can be used with watchman triggers.