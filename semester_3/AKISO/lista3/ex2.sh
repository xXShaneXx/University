#!/bin/bash

format_speed() {
    local speed=$1
    if (( speed < 1024 )); then
        printf "%d B" "$speed"
    elif (( speed < 1048576 )); then
        printf "%.2f KB" "$(awk "BEGIN {print $speed / 1024}")"
    else
        printf "%.2f MB" "$(awk "BEGIN {print $speed / 1048576}")"
    fi
}

draw_bar() {
    local value=$1
    local max_value=$2
    local length=50
    local scaled_value=$((value * length / max_value))
    for ((i=0; i<scaled_value; i++)); do printf "█"; done
    for ((i=scaled_value; i<length; i++)); do printf " "; done
}

prev_recv=0
prev_send=0


while true; do
    clear
    echo "===== Monitorowanie systemu ====="

    # Prędkość wysyłania i odbierania danych
    net_data=$(awk '/^[^ ]+: / {print $2, $10}' /proc/net/dev | grep -v 'lo')
    recv=$(echo "$net_data" | awk '{sum+=$1} END {print sum}')
    send=$(echo "$net_data" | awk '{sum+=$2} END {print sum}')

    if [ "$prev_recv" -gt 0 ]; then
        recv_rate=$((recv - prev_recv))
        send_rate=$((send - prev_send))

        echo -n "Odbieranie: "
        format_speed "$recv_rate"
        echo -n " | Wysyłanie: "
        format_speed "$send_rate"
        echo

        echo -n "Wykres prędkości odbierania: ["
        draw_bar "$recv_rate" 1048576
        echo "]"

        echo -n "Wykres prędkości wysyłania: ["
        draw_bar "$send_rate" 1048576
        echo "]"
    fi

    prev_recv=$recv
    prev_send=$send

    # Wykorzystanie rdzeni CPU
    echo "===== Wykorzystanie CPU ====="
    cpu_stats=$(awk '/^cpu[0-9]+ /' /proc/stat)
    i=0
    while read -r cpu_line; do
        user=$(echo "$cpu_line" | awk '{print $2}')
        nice=$(echo "$cpu_line" | awk '{print $3}')
        system=$(echo "$cpu_line" | awk '{print $4}')
        idle=$(echo "$cpu_line" | awk '{print $5}')
        total=$((user + nice + system + idle))

        if [ "$total" -ne 0 ]; then
            usage=$((100 * (total - idle) / total))
        else
            usage=0
        fi

        freq=$(cat /sys/devices/system/cpu/cpu$i/cpufreq/scaling_cur_freq 2>/dev/null || echo "0")
        freq=$((freq / 1000)) 
        if (( usage > 80 )); then
            echo -e "CPU $i: \033[0;31m$usage%\033[0m przy $freq MHz" 
        else
            echo "CPU $i: $usage% przy $freq MHz"
        fi
        ((i++))
    done <<< "$cpu_stats"

    # Czas działania systemu
    uptime_seconds=$(awk '{print int($1)}' /proc/uptime)
    days=$((uptime_seconds / 86400))
    hours=$(( (uptime_seconds % 86400) / 3600 ))
    minutes=$(( (uptime_seconds % 3600) / 60 ))
    echo "Uptime: $days dni, $hours godz., $minutes min."

    # Stan baterii
    if ls /sys/class/power_supply/BAT* 1>/dev/null 2>&1; then
        for bat in /sys/class/power_supply/BAT*; do
            capacity=$(grep 'POWER_SUPPLY_CAPACITY=' "$bat/uevent" | cut -d= -f2)
            echo "$(basename "$bat"): $capacity%"
        done
    else
        echo "Bateria: Brak informacji"
    fi

    # Obciążenie systemu
    loadavg=$(cat /proc/loadavg | awk '{print $1, $2, $3}')
    echo "Obciążenie systemu (1, 5, 15 min): $loadavg"

    # Wykorzystanie pamięci
    mem_total=$(awk '/MemTotal/ {print $2}' /proc/meminfo)
    mem_free=$(awk '/MemFree/ {print $2}' /proc/meminfo)
    mem_available=$(awk '/MemAvailable/ {print $2}' /proc/meminfo)
    echo "Pamięć całkowita: $((mem_total / 1024)) MB"
    echo "Pamięć wolna: $((mem_free / 1024)) MB"
    echo "Pamięć dostępna: $((mem_available / 1024)) MB"

    sleep 1
done

