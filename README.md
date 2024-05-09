# Captain Holetooth 



## Getting started



```
cd existing_repo
git init
git remote add origin https://gitlab.com/kenandervisagic/captain-holetooth.git
git pull
```
## Branches

### `master`

The `master` branch represents the stable version of the game. It contains code that has been thoroughly tested and is considered production-ready.

### `test`

The `test` branch serves as a staging area for testing new features. When a new feature is developed in a separate branch, it is merged into `test` for comprehensive testing before being merged into `master`.



## Workflow

1. **Feature Development:**
   - Create a new branch for a specific feature or bug fix.
   - Implement and test the feature in the feature branch.

2. **Testing:**
   - Merge the feature branch into the `test` branch for comprehensive testing.
   - Ensure that the new feature integrates well with the existing code.

3. **Stable Release:**
   - Once a set of features in the `test` branch is stable, merge it into the `master` branch for a new stable release.

4. **Documentation:**
   - Update the README to reflect changes in branches and provide context for other developers.


**Important:**
- Do not push to `master` ever.
- Do not push to `test` unless you know what you are doing.
