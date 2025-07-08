#!/bin/bash
# Auto-commit script for hg-watchman integration
# This script is triggered by watchman when files change
# WARNING: Use with caution in production environments

set -e

echo "Auto-commit triggered..."
echo "Timestamp: $(date)"

# Check if we're in a Mercurial repository
if [ ! -d ".hg" ]; then
    echo "Not a Mercurial repository. Skipping auto-commit."
    exit 0
fi

# Check if there are changes to commit
if hg status | grep -q "^M\|^A\|^R\|^!"; then
    echo "Changes detected, committing..."
    
    # Add all changes
    hg addremove
    
    # Commit with timestamp
    commit_message="Auto-commit: $(date '+%Y-%m-%d %H:%M:%S')"
    hg commit -m "$commit_message"
    
    echo "Auto-commit completed: $commit_message"
else
    echo "No changes to commit"
fi

echo "Auto-commit process finished at $(date)"