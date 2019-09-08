
mkdir -p ~/.hm/

hm_history_db=$(echo "~/.hm/history.db")
hm_session_id=$(hm-insert --db $hm_history_db -t 0 --sess_name "Not defined")

export PROMPT_COMMAND='if [ $(id -u) -e 0 ]; then hm-insert --db $hm_history_db -t 1 --sess_id $hm_session_id --pwd "$(pwd)" --cmd "$(history 1 | sed -E "s/^ *[0-9]* *//")"; fi'

