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
    local SEED=$(( $(date +%s) % 100000 )) # random seed
    local LOG_DIR="${ROOT}/logs"

    mkdir -p "${LOG_DIR}"

    local TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
    local FT_LOG="${LOG_DIR}/${TIMESTAMP}_${TARGET}_${TEST_TYPE}_ft.log"
    local STD_LOG="${LOG_DIR}/${TIMESTAMP}_${TARGET}_${TEST_TYPE}_std.log"
    local DIFF_LOG="${LOG_DIR}/${TIMESTAMP}_${TARGET}_${TEST_TYPE}_diff.log"

    local COLOR_RESET="\\033[0m"
    local COLOR_CYAN="\\033[96m"
    local COLOR_GREEN="\\033[92m"
    local COLOR_RED="\\033[91m"
    local COLOR_YELLOW="\\033[93m"
    local COLOR_GRAY="\\033[90m"

    echo -e "${COLOR_CYAN}==========================================${COLOR_RESET}"
    echo -e "${COLOR_CYAN} Running ${TEST_TYPE^^} test diff for ${TARGET}${COLOR_RESET}"
    echo -e "${COLOR_CYAN}------------------------------------------${COLOR_RESET}"
    echo -e "${COLOR_GRAY}Seed: ${SEED}${COLOR_RESET}"
    echo -e "${COLOR_GRAY}Makefile: ${MAKEFILE_PATH}${COLOR_RESET}"
    echo -e "${COLOR_GRAY}Logs: ${LOG_DIR}/${COLOR_RESET}"
    echo -e "${COLOR_GRAY}Timestamp: ${TIMESTAMP}${COLOR_RESET}"
    echo -e "${COLOR_CYAN}==========================================${COLOR_RESET}"

    echo -e "${COLOR_YELLOW}→ Cleaning old builds...${COLOR_RESET}"
    make -C "${MAKE_DIR}" -f "${MAKE_FILE}" fclean >/dev/null

    # === build helper function ===
    build_target() {
        local NAMESPACE_VAL=$1
        local STD_FLAG=""

        # Only std + utils → c++11
        if [ "${TARGET}" == "utils" ] && [ "${NAMESPACE_VAL}" == "std" ]; then
            STD_FLAG="CXXSTD=c++11"
            echo -e "${COLOR_YELLOW}→ Using CXXSTD=c++11 for std utils build${COLOR_RESET}"
        fi

        echo -e "${COLOR_YELLOW}→ Building ${NAMESPACE_VAL} version...${COLOR_RESET}"
        make -C "${MAKE_DIR}" -f "${MAKE_FILE}" ${TEST_TYPE}_${TARGET} NAMESPACE=${NAMESPACE_VAL} ${STD_FLAG} >/dev/null || {
            echo -e "${COLOR_RED}[ERROR]${COLOR_RESET} Failed to build ${NAMESPACE_VAL} version."
            exit 1
        }
    }

    # === Build ft and std versions ===
    build_target ft
    build_target std

    # === Verify binaries ===
    local FT_BIN="${BIN_DIR}/${NAME}_ft_${TEST_TYPE}_${TARGET}"
    local STD_BIN="${BIN_DIR}/${NAME}_std_${TEST_TYPE}_${TARGET}"

    if [ ! -x "${FT_BIN}" ]; then
        echo -e "${COLOR_RED}[ERROR]${COLOR_RESET} Missing ft binary: ${FT_BIN}"
        exit 1
    fi
    if [ ! -x "${STD_BIN}" ]; then
        echo -e "${COLOR_RED}[ERROR]${COLORRESET} Missing std binary: ${STD_BIN}"
        exit 1
    fi

    echo -e "${COLOR_YELLOW}→ Running tests...${COLOR_RESET}"
    echo -e "${COLOR_GRAY}FT run:  ${FT_BIN} ${SEED}${COLOR_RESET}"
    echo -e "${COLOR_GRAY}STD run: ${STD_BIN} ${SEED}${COLOR_RESET}"

    if [ "$TEST_TYPE" == "stress" ]; then
        "${FT_BIN}" "${SEED}" > "${FT_LOG}" 2>&1
        "${STD_BIN}" "${SEED}" > "${STD_LOG}" 2>&1
    else
        "${FT_BIN}" > "${FT_LOG}" 2>&1
        "${STD_BIN}" > "${STD_LOG}" 2>&1
    fi

    echo -e "${COLOR_YELLOW}→ Comparing outputs...${COLOR_RESET}"
    if diff -u "${STD_LOG}" "${FT_LOG}" > "${DIFF_LOG}"; then
        echo -e "${COLOR_GREEN}[PASS]${COLOR_RESET} ft and std outputs are identical for ${TEST_TYPE}_${TARGET}"
    else
        echo -e "${COLOR_RED}[FAIL]${COLOR_RESET} Differences found for ${TEST_TYPE}_${TARGET}"
        echo -e "${COLOR_GRAY}----- DIFF START -----${COLOR_RESET}"
        head -n 20 "${DIFF_LOG}"
        echo -e "${COLOR_GRAY}----- (truncated) -----${COLOR_RESET}"
    fi

    echo -e "${COLOR_CYAN}------------------------------------------${COLOR_RESET}"
    echo -e "${COLOR_GRAY}FT log:   ${FT_LOG}${COLOR_RESET}"
    echo -e "${COLOR_GRAY}STD log:  ${STD_LOG}${COLOR_RESET}"
    echo -e "${COLOR_GRAY}DIFF log: ${DIFF_LOG}${COLORRESET}"
    echo -e "${COLOR_CYAN}==========================================${COLORRESET}"
}
