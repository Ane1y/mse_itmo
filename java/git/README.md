# Git Version Control System

This project implements a version control system inspired by Git, along with a Command Line Interface (CLI) for interacting with the system.

## Features
The Git Version Control System provides the following capabilities:

- `init`: Initializes a new repository.
- `add <files>`: Adds files to the repository.
- `rm <files>`: Removes files from the repository (physically, they are retained).
- `status`: Displays the status of modified, deleted, and untracked files.
- `commit <message>`: Creates a commit with a specified message, along with the date and time.
- `reset <to_revision>`: Resets the repository to a specific revision. The behavior is similar to `git reset --hard`.
- `log [from_revision]`: Displays the commit history, starting from the specified revision (optional, defaults to the latest commit).
- `checkout <revision>`: Switches to a different revision of the repository.
  - Possible values for `<revision>`:
    - Commit hash: The hash of a specific commit.
    - master: Returns the branch to its initial state.
    - HEAD N: Refers to the Nth commit before the HEAD (HEAD 0 == HEAD).
- `checkout -- <files>`: Discards changes made to the specified files.
- `branch-create <branch>`: Creates a new branch with the given name.
- `branch-remove <branch>`: Removes the specified branch.
- `show-branches`: Displays all existing branches.
- `merge <branch>`: Merges the specified branch into the current branch.

## Implementation Details
The Git Version Control System stores its objects (Blob, Commit, and Tree) in the `objects` directory, using their respective hash calculated by the SHA-256 algorithm. When performing operations like `reset` and `checkout`, the objects are accessed from the objects directory based on their hashes.

### Asymptotic Complexity

The system has the following time complexity constraints:
- The main requirement is to ensure the time complexity is not O(n), where n is the total number of commits in the current branch.
- For `reset`, `log`, and `checkout`, the maximum complexity is the number of commits between the current and specified revisions.
- For other operations, the optimal complexity is O(1), but it can be higher.
