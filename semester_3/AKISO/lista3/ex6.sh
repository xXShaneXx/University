#!/bin/bash

declare -A cells
declare -a stack
HEIGHT=$1
WIDTH=$2
player_x=0
player_y=0

# Function to pop an element from the stack
pop() {
  local top=${stack[-1]}
  unset stack[-1]
  echo "$top"
}

# Initialize cells as walls
initialize_cells() {
  #echo "Initializing cells..."
  for ((i=0; i<WIDTH; i++)); do
    for ((j=0; j<HEIGHT; j++)); do
      cells["$i,$j"]="█:0"  # '█' for wall, 0 for unvisited
    done
  done
}

# Set cell as visited with a specific value
set_cell() {
  local x=$1
  local y=$2
  local value=$3
  local isVisited=$4
  cells["$x,$y"]="$value:$isVisited"
  #echo "Set cell ($x, $y) to $value:$isVisited"
}


# Generate the maze using Depth-First Search
generate_maze() {
  #echo "Generating maze..."
  stack=()
  stack+=("0 0")
  set_cell 0 0 " " "1"

  while [ ${#stack[@]} -gt 0 ]; do
    current=${stack[-1]}
    stack=("${stack[@]:0:${#stack[@]}-1}")
    cx=${current% *}
    cy=${current#* }

    #echo "Current cell: ($cx, $cy)"
    
    # Find unvisited neighbors
    neighbors=()
    if [ "$cy" -gt 1 ] && [ "${cells["$cx,$((cy-2))"]}" == "█:0" ]; then
      neighbors+=("$cx $((cy-2))")
    fi
    if [ "$cx" -gt 1 ] && [ "${cells["$((cx-2)),$cy"]}" == "█:0" ]; then
      neighbors+=("$((cx-2)) $cy")
    fi
    if [ "$cy" -lt $((HEIGHT-2)) ] && [ "${cells["$cx,$((cy+2))"]}" == "█:0" ]; then
      neighbors+=("$cx $((cy+2))")
    fi
    if [ "$cx" -lt $((WIDTH-2)) ] && [ "${cells["$((cx+2)),$cy"]}" == "█:0" ]; then
      neighbors+=("$((cx+2)) $cy")
    fi

    if [ ${#neighbors[@]} -gt 0 ]; then
      # Push the current cell to the stack
      stack+=("$cx $cy")
      # Choose a random neighbor
      random_index=$((RANDOM % ${#neighbors[@]}))
      next=${neighbors[$random_index]}
      nx=${next% *}
      ny=${next#* }

      #echo "Next cell: ($nx, $ny)"

      # Remove the wall between the current cell and the chosen cell
      if [ "$nx" -eq "$cx" ]; then
        set_cell "$cx" "$(( (cy+ny)/2 ))" " " "1"
      else
        set_cell "$(( (cx+nx)/2 ))" "$cy" " " "1"
      fi

      # Mark the chosen cell as visited and push it to the stack
      set_cell "$nx" "$ny" " " "1"
      stack+=("$nx $ny")
    fi
  done

  # Ensure entrance and exit
  set_cell 0 0 "@" "1"  # Entrance at top-left
  set_cell $((HEIGHT-1)) $((WIDTH-1)) " " "1"  # Exit at bottom-right
}

# Function to draw the labyrinth
draw_labyrinth() {
  #echo "Drawing labyrinth..."
  for ((i=0; i<HEIGHT; i++)); do
    for ((j=0; j<WIDTH; j++)); do
      cell=${cells["$i,$j"]}
      value=${cell%:*}
      if [ $i -eq $((HEIGHT-1)) ] && [ $j -eq $((WIDTH-1)) ]; then
        echo -ne "\e[48;5;46m$value\e[0m"  # Green background for the exit
      elif [ $i -eq 0 ] && [ $j -eq 0 ]; then
        echo -ne "\e[48;5;39m$value\e[0m"  # Blue background for the entrance
      else
        echo -ne "$value"
      fi
    done
    echo
  done
}

move_player() {
  direction=$1

  case "$direction" in
    "UP")
      if [ "${cells["$((player_x-1)),$player_y"]}" == " :1" ] && [ $((player_x-1)) -ge 0 ]; then
        set_cell $player_x $player_y " " "1"
        ((player_x--))
        set_cell $player_x $player_y "@" "1"
      fi
      ;;
    "DOWN")
      if [ "${cells["$((player_x+1)),$player_y"]}" == " :1" ] && [ $((player_x+1)) -lt $HEIGHT ]; then
        set_cell $player_x $player_y " " "1"
        ((player_x++))
        set_cell $player_x $player_y "@" "1"
      fi
      ;;
    "LEFT")
      if [ "${cells["$player_x,$((player_y-1))"]}" == " :1" ] && [ $((player_y-1)) -ge 0 ]; then
        set_cell $player_x $player_y " " "1"
        ((player_y--))
        set_cell $player_x $player_y "@" "1"
      fi
      ;;
    "RIGHT")
      if [ "${cells["$player_x,$((player_y+1))"]}" == " :1" ] && [ $((player_y+1)) -lt $WIDTH ]; then
        set_cell $player_x $player_y " " "1"
        ((player_y++))
        set_cell $player_x $player_y "@" "1"
      fi
      ;;
    *)
      echo "Invalid direction"
      ;;
  esac
}

check_win() {
  if [ $player_x -eq $((WIDTH-1)) ] && [ $player_y -eq $((HEIGHT-1)) ]; then
    echo "true"
  else
    echo "false"
  fi
}

# Main function to initialize and generate the maze
main() {
  # Get terminal size if no dimensions are provided
  if [ -z "$HEIGHT" ] || [ -z "$WIDTH" ]; then
    TERMINAL_HEIGHT=$(tput lines)
    TERMINAL_WIDTH=$(tput cols)

    # Reduce size to fit terminal and ensure odd dimensions
    HEIGHT=$((TERMINAL_HEIGHT - 2))
    WIDTH=$((TERMINAL_WIDTH - 1))
  fi

  # Ensure dimensions are odd for proper maze generation
  if (( HEIGHT % 2 == 0 )); then HEIGHT=$((HEIGHT + 1)); fi
  if (( WIDTH % 2 == 0 )); then WIDTH=$((WIDTH + 1)); fi



  initialize_cells
  generate_maze
  while [ "$w" != "q" ]; do
    clear
    draw_labyrinth

    if [ "$(check_win)" == "true" ]; then
      echo "You Win!"
      break
    fi
    
    read -n 1 -s w
    case "$w" in
        [wW] ) move_player "UP" ;;
        [sS] ) move_player "DOWN" ;;
        [dD] ) move_player "RIGHT" ;;
        [aA] ) move_player "LEFT" ;;
    esac
  done
}

main