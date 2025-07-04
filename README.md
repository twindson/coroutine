# coroutine

## Sapling/hg Empty Commit

To create an empty commit in Sapling's hg (Mercurial), use the following command:

```bash
hg commit --message "Your commit message" --empty
```

Or using the short form:

```bash
hg commit -m "Your commit message" --empty
```

The `--empty` flag allows you to create a commit without any changes to the working directory. This can be useful for:
- Creating milestone markers
- Triggering CI/CD pipelines
- Adding notes or documentation to the commit history