#!/bin/sh
# Shared by .githooks/post-merge and .githooks/post-commit.
# Detects whether a commit message denotes a merge of a stable-* branch
# (the NC release base) and, if so, re-runs the STRATO/IONOS translation
# merge (translation_scripts/merge_translation.py) against it so
# translations/client_*.ts stay in sync. On success, the result is staged
# and a commit message is suggested via .githooks/prepare-commit-msg.

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

    # GUI clients (GitKraken etc.) often don't surface hook stdout anywhere visible,
    # so mirror everything into a log file as well - that's the reliable record.
    {
        echo ""
        echo "===== $(date '+%Y-%m-%d %H:%M:%S') $hook_name: detected merge of '$branch' ====="
    } >> "$log_file"

    echo "$hook_name: detected merge of '$branch' - re-running STRATO/IONOS translation merge..."
    echo "$hook_name: full log also written to .githooks/post-merge.log"

    command -v python3 >/dev/null 2>&1 && PY=python3 || PY=python

    # 'auto' runs steps 1-5 directly against the already-merged working tree
    # (which now contains both the NC base and our STRATO source) - unlike
    # 'all', it skips step 0's git-worktree checkout of the clean NC branch,
    # which only feeds a "which keys are ours vs NC's" diagnostic in the final
    # report and isn't needed for the merge itself. No worktree also means no
    # more Windows temp-dir races.
    #
    # Capture via command substitution (not a pipe) so $? below is the real
    # python exit status - portable across dash/bash.
    output=$(cd "$repo_root/translation_scripts" && "$PY" merge_translation.py auto 2>&1)
    status=$?

    echo "$output"
    echo "$output" >> "$log_file"

    if [ "$status" -ne 0 ]; then
        msg="$hook_name: translation merge script reported problems (exit $status). Review translations/ before committing."
        echo "$msg" >&2
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
