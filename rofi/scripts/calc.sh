#!/usr/bin/env bash

# Simple calculator script
# Use it by adding a new mode, e.g.:
# rofi -show run -modi calc:../Examples/rofi-calculator.sh
ROFI_CALC_HISTORY_FILE=~/.local/share/rofi/rofi_calc_history
ROFI_CALC_HISTORY_MAXCOUNT=1 # maximum number of history entries

# Create the directory for the files of the script
if [ ! -d $(dirname "${ROFI_CALC_HISTORY_FILE}") ]
then
    mkdir -p "$(dirname "${ROFI_CALC_HISTORY_FILE}")"
fi

if [[ "$1" == =* ]]; then
    FORMULA="${1#=} ${@:2}"
elif [[ "$1" == "=" ]]; then
    FORMULA="${@:2}"
else
    exit 0
fi

RESULT=$(echo "$FORMULA" | bc -l 2>&1)

if echo "$RESULT" | grep -q "syntax error"; then
    echo "$(echo $FORMULA | xargs) = ERROR"
else
    echo "$(echo $FORMULA | xargs) = $RESULT"
fi

# if [ -n "${FORMULA}" ]; then
#     if [[ "${FORMULA}" =~ "=" ]]; then
#         OUTPUT=${FORMULA}
#     else
#         RESULT=`echo "${FORMULA}" | bc -l`
#         OUTPUT="${FORMULA} = ${RESULT}"
#     fi

#     echo -e "${OUTPUT}\n$(cat ${ROFI_CALC_HISTORY_FILE})" > ${ROFI_CALC_HISTORY_FILE}

#     if [ $( wc -l < "${ROFI_CALC_HISTORY_FILE}" ) -gt ${ROFI_CALC_HISTORY_MAXCOUNT} ]; then
#         echo "$(head -n ${ROFI_CALC_HISTORY_MAXCOUNT} ${ROFI_CALC_HISTORY_FILE})" > ${ROFI_CALC_HISTORY_FILE}
#     fi

#     cat ${ROFI_CALC_HISTORY_FILE}
# fi
