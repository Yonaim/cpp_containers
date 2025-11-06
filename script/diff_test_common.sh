#!/bin/bash

run_diff_test() {
    # === Always resolve ROOT as the script's parent directory ===
    local SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    local ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

    local TEST_TYPE=$1   # unit or stress
    local TARGET=$2      # map, vector, stack, utils, all
    local MAKEFILE_PATH=${3:-${ROOT}/Makefile}
    local MAKE_DIR=$(dirname "${MAKEFILE_PATH}")
    local MAKE_FILE=$(basename "${MAKEFILE_PATH}")
    local BIN_DIR="${ROOT}/bin"
    local NAME=containers
    local SEED=$(( $(date +%s) % 100000 )) # random seed from current time
    local LOG_DIR="${ROOT}/logs"

    mkdir -p "${LOG_DIR}"

    # === Timestamp prefix ===
    local TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

    local FT_LOG="${LOG_DIR}/${TIMESTAMP}_${TARGET}_${TEST_TYPE}_ft.log"
    local STD_LOG="${LOG_DIR}/${TIMESTAMP}_${TARGET}_${TEST_TYPE}_std.log"
    local DIFF_LOG="${LOG_DIR}/${TIMESTAMP}_${TARGET}_${TEST_TYPE}_diff.log"

    # === Color codes ===
    local COLOR_RESET="\\033[0m"
    local COLOR_CYAN="\\033[96m"
    local COLOR_GREEN="\\033[92m"
    local COLOR_RED="\\033[91m"
    local COLOR_YELLOW="\\033[93m"
    local COLOR_GRAY="\\033[90m"

    echo -e "${COLOR_CYAN}==========================================${COLOR_RESET}"
    echo -e "${COLOR_CYAN}Running ${TEST_TYPE^^} test diff for ${TARGET} (seed=${SEED})${COLOR_RESET}"
    echo -e "${COLOR_CYAN}Using Makefile: ${MAKEFILE_PATH}${COLOR_RESET}"
    echo -e "${COLOR_GRAY}Logs stored in: ${LOG_DIR}/ ${COLOR_RESET}"
    echo -e "${COLOR_GRAY}Timestamp: ${TIMESTAMP}${COLOR_RESET}"
    echo -e "${COLOR_CYAN}==========================================${COLOR_RESET}"

    echo -e "${COLOR_YELLOW}Cleaning old builds...${COLOR_RESET}"
    make -C "${MAKE_DIR}" -f "${MAKE_FILE}" fclean >/dev/null

    echo -e "${COLOR_YELLOW}Building ft version...${COLOR_RESET}"
    make -C "${MAKE_DIR}" -f "${MAKE_FILE}" ${TEST_TYPE}_${TARGET} NAMESPACE=ft >/dev/null || exit 1

    echo -e "${COLOR_YELLOW}Building std version...${COLOR_RESET}"
    make -C "${MAKE_DIR}" -f "${MAKE_FILE}" ${TEST_TYPE}_${TARGET} NAMESPACE=std >/dev/null || exit 1

    echo -e "${COLOR_YELLOW}Running tests...${COLOR_RESET}"
    if [ "$TEST_TYPE" == "stress" ]; then
        "${BIN_DIR}/${NAME}_ft_${TEST_TYPE}_${TARGET}" "${SEED}" > "${FT_LOG}" 2>&1
        "${BIN_DIR}/${NAME}_std_${TEST_TYPE}_${TARGET}" "${SEED}" > "${STD_LOG}" 2>&1
    else
        "${BIN_DIR}/${NAME}_ft_${TEST_TYPE}_${TARGET}" > "${FT_LOG}" 2>&1
        "${BIN_DIR}/${NAME}_std_${TEST_TYPE}_${TARGET}" > "${STD_LOG}" 2>&1
    fi

    echo -e "${COLOR_YELLOW}Comparing outputs...${COLOR_RESET}"
    if diff -u "${STD_LOG}" "${FT_LOG}" > "${DIFF_LOG}"; then
        echo -e "${COLOR_GREEN}[PASS]${COLOR_RESET} ft and std outputs are identical for ${TEST_TYPE}_${TARGET}"
    else
        echo -e "${COLOR_RED}[FAIL]${COLOR_RESET} Differences found for ${TEST_TYPE}_${TARGET}"
        cat "${DIFF_LOG}"
    fi

    echo -e "${COLOR_GRAY}Logs:${COLOR_RESET}"
    echo -e "${COLOR_GRAY}- ${FT_LOG}${COLOR_RESET}"
    echo -e "${COLOR_GRAY}- ${STD_LOG}${COLOR_RESET}"
    echo -e "${COLOR_GRAY}- ${DIFF_LOG}${COLOR_RESET}"
}
