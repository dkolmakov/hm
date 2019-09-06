
export PROMPT_COMMAND='if [ "$(id -u)" -ne 0 ]; then /home/dmitry/projects/hm/build/hm-insert /home/dmitry/projects/hm/build/history.db "$(pwd)" "$(history 1)"; fi'

