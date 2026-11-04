#!/bin/sh
# Shared by .githooks/post-merge and .githooks/post-commit.
# Detects whether a commit message denotes a merge of a stable-* branch
# (the NC release base) and, if so, re-runs the STRATO/IONOS translation
# merge (translation_scripts/merge_translation.py) against it so
# translations/client_*.ts stay in sync. On success, the result is staged
# and a commit message is suggested via .githooks/prepare-commit-msg.
#
# Exception: a merge of the plain release branch (stable-x.y) into our
# develop_stable-x.y branch runs the script with --auto-commit, so the
# re-merge lands as its own commits instead of being left staged. See the
# comment at the auto_commit_arg decision below for why only that direction.

TRANSLATION_MERGE_SUGGESTED_MSG_FILE_NAME=TRANSLATION_MERGE_SUGGESTED_MSG

translation_merge_run_if_stable() {
    hook_name="$1"
    merge_msg="$2"
    repo_root=$(git rev-parse --show-toplevel)
    log_file="$repo_root/.githooks/post-merge.log"

    # Matches "Merge branch 'stable-33.0'" or "Merge remote-tracking branch 'origin/stable-33.0'"
    branch=$(printf '%s' "$merge_msg" | grep -oE "Merge (branch|remote-tracking branch) '[^']*stable-[0-9]+\.[0-9]+[^']*'" | head -1 | sed -E "s/.*'([^']*)'.*/\1/")

    if [ -z "$branch" ]; then
        return 0
    fi

    current_branch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null)

    # GUI clients (GitKraken etc.) often don't surface hook stdout anywhere visible,
    # so mirror everything into a log file as well - that's the reliable record.
    {
        echo ""
        echo "===== $(date '+%Y-%m-%d %H:%M:%S') $hook_name: detected merge of '$branch' into '$current_branch' ====="
    } >> "$log_file"

    echo "$hook_name: detected merge of '$branch' into '$current_branch' - re-running STRATO/IONOS translation merge..."
    echo "$hook_name: full log also written to .githooks/post-merge.log"

    # Decide whether the re-merge may commit its own result.
    #
    # Only one direction gets --auto-commit: a plain 'stable-x.y' (optionally
    # remote-prefixed, e.g. 'origin/stable-33.0') merged into our
    # 'develop_stable-x.y' branch. Taking over a new NC release base is
    # exactly the case the re-merge exists for, so its Step 0..5 commits
    # belong on the branch right away.
    #
    # Every other direction keeps the old behaviour - staged plus a suggested
    # commit message. That is mainly 'develop_stable-x.y' merged down into a
    # feature branch: that source name contains 'stable-x.y' too and
    # therefore matches the branch pattern above, but what lands in a feature
    # branch's history is the developer's call. (A feature branch merged back
    # into develop_stable-x.y doesn't match the pattern at all and never gets
    # this far.)
    auto_commit_arg=""
    if printf '%s' "$branch" | grep -qE '^([^/]+/)?stable-[0-9]+\.[0-9]+$' &&
       printf '%s' "$current_branch" | grep -qE '^develop[_-]stable-[0-9]+\.[0-9]+$'; then
        # merge_translation.py commits per step with a plain `git commit`,
        # which would sweep along anything else that is already staged - so
        # only hand it the flag when the index holds nothing else.
        if git diff --cached --quiet; then
            auto_commit_arg="--auto-commit"
            msg="$hook_name: stable-x.y -> develop_stable-x.y merge - running with --auto-commit."
        else
            msg="$hook_name: stable-x.y -> develop_stable-x.y merge, but unrelated changes are already staged - running WITHOUT --auto-commit so they aren't swept into a step commit."
        fi
    else
        msg="$hook_name: not a stable-x.y -> develop_stable-x.y merge - running without --auto-commit, the result is only staged."
    fi
    echo "$msg"
    echo "$msg" >> "$log_file"

    # command -v only checks PATH presence, not that the command actually
    # works. On Windows, `python3`/`python` often exist as Microsoft Store
    # app-execution-alias stubs that resolve fine but fail at runtime (exit
    # 49, "Python wurde nicht gefunden...") when no Store Python is
    # installed - even though a real interpreter is reachable under the
    # other name. Verify with --version instead of trusting command -v.
    if command -v python3 >/dev/null 2>&1 && python3 --version >/dev/null 2>&1; then
        PY=python3
    elif command -v python >/dev/null 2>&1 && python --version >/dev/null 2>&1; then
        PY=python
    else
        echo "$hook_name: no working python/python3 interpreter found on PATH - skipping translation merge" >&2
        return 0
    fi

    # post-merge/post-commit only fire once the merge is already finished,
    # but .git/MERGE_HEAD can apparently still be lingering at that exact
    # moment. `git commit` auto-detects MERGE_HEAD and silently adds it as
    # an extra parent to whatever it commits next - so the *first* commit
    # merge_translation.py makes below (Step 0) would otherwise turn into a
    # spurious second merge of the same stable branch tip. Clear it: the
    # merge already completed, this bookkeeping has served its purpose.
    git_dir=$(git rev-parse --git-dir)
    rm -f "$git_dir/MERGE_HEAD" "$git_dir/MERGE_MSG" "$git_dir/MERGE_MODE"

    # 'auto' runs steps 1-5 directly against the already-merged working tree
    # (which now contains both the NC base and our STRATO source) - unlike
    # 'all', it skips step 0's git-worktree checkout of the clean NC branch,
    # which only feeds a "which keys are ours vs NC's" diagnostic in the final
    # report and isn't needed for the merge itself. No worktree also means no
    # more Windows temp-dir races.
    #
    # Capture via command substitution (not a pipe) so $? below is the real
    # python exit status - portable across dash/bash.
    # $auto_commit_arg unquoted on purpose: when empty it has to expand to no
    # argument at all rather than to an empty one.
    output=$(cd "$repo_root/translation_scripts" && "$PY" merge_translation.py auto $auto_commit_arg 2>&1)
    status=$?

    echo "$output"
    echo "$output" >> "$log_file"

    if [ "$status" -ne 0 ]; then
        msg="$hook_name: translation merge script reported problems (exit $status). Review translations/ before committing."
        echo "$msg" >&2
        echo "$msg" >> "$log_file"
        return 0
    fi

    if [ -n "$auto_commit_arg" ]; then
        # The script already committed every step that changed something, so
        # there is nothing left to stage or to title via prepare-commit-msg.
        msg="$hook_name: translation merge finished and committed (one commit per changed step). Review with 'git log'/'git show' before pushing."
        echo "$msg"
        echo "$msg" >> "$log_file"
        return 0
    fi

    # Stage the result so it shows up ready-to-commit in any client. Only
    # suggest a commit message (via prepare-commit-msg) if that staging
    # actually produced a diff - an empty re-run shouldn't hijack the next
    # unrelated commit's message.
    git -C "$repo_root" add -- translations/client_*.ts

    if git -C "$repo_root" diff --cached --quiet -- translations/; then
        msg="$hook_name: translation merge finished, no changes to translations/."
        echo "$msg"
        echo "$msg" >> "$log_file"
        return 0
    fi

    echo "[Git-Hook] run translations script after merge" > "$repo_root/.git/$TRANSLATION_MERGE_SUGGESTED_MSG_FILE_NAME"

    msg="$hook_name: translation merge finished and staged. Suggested commit message will be pre-filled on your next commit - review 'git diff --cached' in translations/ first."
    echo "$msg"
    echo "$msg" >> "$log_file"
}
